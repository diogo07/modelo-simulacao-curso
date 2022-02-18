#include <string.h>
#include <omnetpp.h>
#include <math.h>
#include <random>
#include <time.h>
#include "Period.h"

using namespace omnetpp;

Period::~Period() {}

Define_Module(Period);

void Period::initialize() {

    queueWaitSize = registerSignal("queueWaitSize");
    sizeClass = registerSignal("sizeClass");

    registerSignalArray();

    classCapacity = par("classCapacity");
    numberOfPeriods = par("numberOfPeriods");
    currentPeriod = par("currentPeriod");
    timing = currentPeriod * 6.0;
    outPort = 0;
    studentsClassCount = 0;


    srand((int)time(NULL));
}

void Period::handleMessage(cMessage *msg) {
    SimTime currentTime = simTime();

    Student *student = dynamic_cast<Student*>(msg);

    //    tempo mudou, semestre novo
    if (timing != currentTime) {
        //      verifica se a quantidade de alunos na turma é menor que a capacidade suportada
        //      e se existe algum aluno na fila de espera
        //      caso positivo, avalia os alunos
        if(classCapabilities[currentPeriod - 1] > studentsClassCount && queueWaiting.getLength() > 0){

            int availableVacancies = classCapabilities[currentPeriod - 1] - studentsClassCount;
            int vacanciesToFill = (availableVacancies > queueWaiting.getLength()) ? queueWaiting.getLength() : availableVacancies;

            for(int i = 0; i < vacanciesToFill; i++){
                outPort++;
                Student *queueStudent = check_and_cast<Student*>(queueWaiting.pop());
                studentsClassCount++;
                evaluate(queueStudent);
            }

            //  avaliar evasão para alunos da fila de espera
            for(int j = 0; j < queueWaiting.getLength(); j++){
                Student *al = check_and_cast<Student*>(queueWaiting.get(j));
                int bondDuration = (timing.dbl() - al->getTimeEntry()) / 6;
                if(dropout(bondDuration)){
                    emit(dropoutsPerSemester[bondDuration > 21 ? 20 : bondDuration - 1], 1);
                    queueWaiting.remove(al);
                    cancelAndDelete(al);
               }
           }
        }
        emitStatisticsPeriod();
        timing = currentTime;
        outPort = 0;
    } else {
        outPort++;
    }
    processStudent(student);

}

void Period::processStudent(Student *student) {

    addToClass(student);

    if (schoolClass.getLength() > 0) {
        Student *classStudent = check_and_cast<Student*>(schoolClass.pop());
        studentsClassCount++;
        int semestersCount = (int) (timing.dbl() - classStudent->getTimeEntry()) / 6;
        evaluate(classStudent);
    }
}

void Period::addToClass(Student *student) {

    if (studentsClassCount == classCapabilities[currentPeriod - 1]) { // turma cheia, adiciona na fila de espera
//        queueWaiting.insert(student);
        addToQueueWaiting(student);
    } else if (queueWaiting.getLength() > 0) { // turma tem capacidade, entao verifica se tem student na fila de espera

        if (compare(student, check_and_cast<Student*>(queueWaiting.front()))) { // compara o student que chegou com o primeiro
            schoolClass.insert(student);                                          // da fila de espera
        } else {
            Student *queueStudent = check_and_cast<Student*>(queueWaiting.pop());
            schoolClass.insert(queueStudent);
//            queueWaiting.insert(student);
            addToQueueWaiting(student);
        }

    } else { // fila de espera vazia, adiciona o student na turma
        schoolClass.insert(student);
    }
}

void Period::evaluate(Student *student) {

    student->setBeginner(false);
    int bondDuration = (timing.dbl() - student->getTimeEntry()) / 6;

    if (student->getEntryPeriod(currentPeriod - 1) == 0) {
        student->setEntryPeriod(currentPeriod - 1, (int) timing.dbl());
    }

    if (dropout(bondDuration)) {
        emit(dropoutsPerSemester[bondDuration > 21 ? 20 : bondDuration - 1], 1);
        cancelAndDelete(student);
    } else {
//        emit(totalPerSemester[bondDuration - 1], 1);
        if (disapprove(bondDuration)) {
            emit(disapprovalsPerSemester[bondDuration > 21 ? 20 : bondDuration - 1], 1);
            student->setDisapprovals(currentPeriod - 1, student->getDisapprovals(currentPeriod - 1) + 1);
            send(student, "out", classCapacity + outPort);

        } else {

            if (graduate(bondDuration)) {
                emit(graduatesPerSemester[bondDuration > 21 ? 20 : bondDuration - 1], 1);
                student->setExitPeriod(currentPeriod - 1, (int) timing.dbl());
                emit(approvedsPerSemester[bondDuration > 21 ? 20 : bondDuration - 1], 1);
                cancelAndDelete(student);
            } else {
                if(currentPeriod == numberOfPeriods){
                    emit(disapprovalsPerSemester[bondDuration > 21 ? 20 : bondDuration - 1], 1);
                    student->setDisapprovals(currentPeriod - 1, student->getDisapprovals(currentPeriod - 1) + 1);
                    send(student, "out", classCapacity + outPort);
                } else {
                    student->setExitPeriod(currentPeriod - 1, (int) timing.dbl());
                    emit(approvedsPerSemester[bondDuration > 21 ? 20 : bondDuration - 1], 1);
                    send(student, "out", outPort);
                }


            }
        }
    }

}


bool Period::compare(Student *student1, Student *student2) {
    int bondDuration1 = (timing.dbl() - student1->getTimeEntry()) / 6;
    int bondDuration2 = (timing.dbl() - student2->getTimeEntry()) / 6;

    return bondDuration1 > bondDuration2;

//    if (student1->getBeginner() && student2->getBeginner()) {
//        return student1->getDisapprovals(currentPeriod - 1)
//                < student2->getDisapprovals(currentPeriod - 1);
//    } else if (student1->getBeginner()) {
//        return true;
//    } else if (student2->getBeginner()) {
//        return false;
//    } else {
//        return student1->getDisapprovals(currentPeriod - 1)
//                < student2->getDisapprovals(currentPeriod - 1);
//    }
}

void Period::addToQueueWaiting(Student *student) {

    int bondDuration1 = (timing.dbl() - student->getTimeEntry()) / 6;
    int queueLength = queueWaiting.getLength();

    for(int i = 0; i < queueWaiting.getLength(); i++){
        Student *studentQueue = check_and_cast<Student*>(queueWaiting.get(i));
        int bondDuration2 = (timing.dbl() - studentQueue->getTimeEntry()) / 6;

        if(bondDuration1 > bondDuration2){
            queueWaiting.insertBefore(studentQueue, student);
            break;
        }
    }

    if(queueLength == queueWaiting.getLength()){
        queueWaiting.insert(student);
    }

    EV << "------------------- INIT -----------------" << endl;

    for(int i = 0; i < queueWaiting.getLength(); i++){
        Student *studentQueue = check_and_cast<Student*>(queueWaiting.get(i));
        int bondDuration2 = (timing.dbl() - studentQueue->getTimeEntry()) / 6;
        EV << "duracao: " << bondDuration2 << endl;
    }

    EV << "------------------- FINISH -----------------" << endl;


//    SimTime t = simTime();
//
//
//    int bondDuration1 = (t.dbl() - student1->getTimeEntry()) / 6;
//    int bondDuration2 = (t.dbl() - student2->getTimeEntry()) / 6;
//
//    if(bondDuration1 < bondDuration2){
//        return -1;
//    } else if (bondDuration1 == bondDuration2){
//        return 0;
//    } else {
//        return 1;

//    return 0;
//    }
}

float Period::randomValue(){
    return (float) rand() / (float) RAND_MAX;
}

bool Period::dropout(int bondDuration) {

    float rNumber = randomValue();

    if (probsType == 0) {
        float probDropout = (float) probsDropoutPeriod[course][currentPeriod - 1];
        return rNumber < probDropout;

    } else if (probsType == 1) {
        float probDropout = (float) probsDropoutBondDuration[course][bondDuration - 1];
        return rNumber < probDropout;

    } else {
        float probDropout = (float) probsDropoutPeriodBondDuration[course][currentPeriod - 1][bondDuration - 1];
        return rNumber < probDropout;
    }

}

bool Period::disapprove(int bondDuration) {

    float rNumber = randomValue();

    if (probsType == 0) {

        float probDisapproval = (float) probsDisapprovalPeriod[course][currentPeriod - 1];
        return rNumber < probDisapproval;

    } else if (probsType == 1) {

        float probDisapproval = (float) probsDisapprovalBondDuration[course][bondDuration - 1];
        return rNumber < probDisapproval;

    } else {

        float probDisapproval = (float) probsDisapprovalPeriodBondDuration[course][currentPeriod - 1][bondDuration - 1];
        return rNumber < probDisapproval;

    }
}

bool Period::graduate(int bondDuration) {
    float rNumber = randomValue();
    if (probsType == 1) {
        float probGraduation = (float) probsGraduationBondDuration[course][bondDuration - 1];
        return rNumber < probGraduation;

    } else {
        float probGraduation = (float) probsGraduationPeriodBondDuration[course][currentPeriod - 1][bondDuration - 1];
        return rNumber < probGraduation;
    }

}

void Period::registerSignalArray() {


//    for(int i = 0; i < 71; ++i){
//        char signalNameSizeClass[32];
//        sprintf(signalNameSizeClass, "sizeClass%d", i);
//        simsignal_t signalSizeClass = registerSignal(signalNameSizeClass);
//        cProperty *statisticTemplateSizeClass = getProperties()->get("statisticTemplate", "sizeClassTemplate");
//        getEnvir()->addResultRecorders(this, signalSizeClass, signalNameSizeClass, statisticTemplateSizeClass);
//        sizeClass[i] = signalSizeClass;
//    }
//
//    for (int i = 0; i < 9000; ++i) {
//        char signalNameQueueWaitSize[32];
//        sprintf(signalNameQueueWaitSize, "queueWaitSize%d", i);
//        simsignal_t signalQueueWaitSize = registerSignal(signalNameQueueWaitSize);
//        cProperty *statisticTemplateQueueWaitSize = getProperties()->get(
//                "statisticTemplate", "queueWaitSizeTemplate");
//        getEnvir()->addResultRecorders(this, signalQueueWaitSize,
//                signalNameQueueWaitSize, statisticTemplateQueueWaitSize);
//        queueWaitSize[i] = signalQueueWaitSize;
//    }



    int semesters = 21;

    //    INICIA VARIAVEIS DE STATISTICS DE EVASAO, GRADUACAO E REPROVACAO POR SEMESTRE
    for (int s = 0; s < semesters; ++s) {

//        char signalNameTotalSemester[32];
        char signalNameDropoutsSemester[32];
        char signalNameGraduatesSemester[32];
        char signalNameDisapprovalsSemester[32];
        char signalNameApprovedsSemester[32];

//        sprintf(signalNameTotalSemester, "totalPerSemester%d", s);
        sprintf(signalNameDropoutsSemester, "dropoutsPerSemester%d", s);
        sprintf(signalNameGraduatesSemester, "graduatesPerSemester%d", s);
        sprintf(signalNameDisapprovalsSemester, "disapprovalsPerSemester%d", s);
        sprintf(signalNameApprovedsSemester, "approvedsPerSemester%d", s);

//        simsignal_t signalTotalSemester = registerSignal(signalNameTotalSemester);
        simsignal_t signalDropoutsSemester = registerSignal(signalNameDropoutsSemester);
        simsignal_t signalGraduatesSemester = registerSignal(signalNameGraduatesSemester);
        simsignal_t signalDisapprovalsSemester = registerSignal(signalNameDisapprovalsSemester);
        simsignal_t signalApprovedsSemester = registerSignal(signalNameApprovedsSemester);

//        cProperty *statisticTemplateTotalSemester = getProperties()->get("statisticTemplate", "totalPerSemesterTemplate");
        cProperty *statisticTemplateDropoutsSemester = getProperties()->get("statisticTemplate", "dropoutsPerSemesterTemplate");
        cProperty *statisticTemplateGraduatesSemester = getProperties()->get("statisticTemplate", "graduatesPerSemesterTemplate");
        cProperty *statisticTemplateDisapprovalsSemester = getProperties()->get("statisticTemplate", "disapprovalsPerSemesterTemplate");
        cProperty *statisticTemplateApprovedsSemester = getProperties()->get("statisticTemplate", "approvedsPerSemesterTemplate");

//        getEnvir()->addResultRecorders(this, signalTotalSemester, signalNameTotalSemester, statisticTemplateTotalSemester);
        getEnvir()->addResultRecorders(this, signalDropoutsSemester, signalNameDropoutsSemester, statisticTemplateDropoutsSemester);
        getEnvir()->addResultRecorders(this, signalGraduatesSemester, signalNameGraduatesSemester, statisticTemplateGraduatesSemester);
        getEnvir()->addResultRecorders(this, signalDisapprovalsSemester, signalNameDisapprovalsSemester, statisticTemplateDisapprovalsSemester);
        getEnvir()->addResultRecorders(this, signalApprovedsSemester, signalNameApprovedsSemester, statisticTemplateApprovedsSemester);

//        totalPerSemester[s] = signalTotalSemester;
        dropoutsPerSemester[s] = signalDropoutsSemester;
        graduatesPerSemester[s] = signalGraduatesSemester;
        disapprovalsPerSemester[s] = signalDisapprovalsSemester;
        approvedsPerSemester[s] = signalApprovedsSemester;
   }


}

void Period::emitStatisticsPeriod() {

    emit(sizeClass, studentsClassCount);
    emit(queueWaitSize, queueWaiting.getLength());

    char queueSize[40];
    sprintf(queueSize, "Fila de espera: %d", queueWaiting.getLength());
    bubble(queueSize);
    studentsClassCount = 0;
}

void Period::finish() {
}
