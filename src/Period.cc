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
//    tamanhoFilaEspera = registerSignal("tamanhoFilaEspera");
//    tamanhoTurma = registerSignal("tamanhoTurma");
//    totalMatriculas = registerSignal("totalMatriculas");

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

    if (timing != currentTime) {
        if(classCapabilities[currentPeriod - 1] > studentsClassCount && queueWaiting.getLength() > 0){

            int availableVacancies = classCapabilities[currentPeriod - 1] - studentsClassCount;
            int vacanciesToFill = (availableVacancies > queueWaiting.getLength()) ? queueWaiting.getLength() : availableVacancies;

            for(int i = 0; i < vacanciesToFill; i++){
               outPort++;
                Student *queueStudent = check_and_cast<Student*>(queueWaiting.pop());
                studentsClassCount++;
                evaluate(queueStudent);
            }

//           for(int j = 0; j < queueWaiting.getLength(); j++){
//               Student *al = check_and_cast<Student*>(queueWaiting.get(j));
//               int bondDuration = (timing.dbl() - al->getEntrada()) / 6;
//               if(dropout(bondDuration)){
//                   emit(dropoutsPerSemester[bondDuration - 1], 1);
//                   queueWaiting.remove(al);
//                   cancelAndDelete(al);
//               }
//           }
        }
        emitPeriodData();
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
        queueWaiting.insert(student);
    } else if (queueWaiting.getLength() > 0) { // turma tem capacidade, ent�o verifica se tem student na fila de espera

        if (compare(student, check_and_cast<Student*>(queueWaiting.front()))) { // compara o student que chegou com o primeiro
            schoolClass.insert(student);                                          // da fila de espera
        } else {
            Student *queueStudent = check_and_cast<Student*>(queueWaiting.pop());
            schoolClass.insert(queueStudent);
            queueWaiting.insert(student);
        }

    } else { // fila de espera vazia, adiciona o student na turma
        schoolClass.insert(student);
    }
}

void Period::evaluate(Student *student) {

//    if(currentPeriod == 1){
//    }

    student->setBeginner(false);
//    student->setBondDuration(student->getBondDuration() + 1);
    int bondDuration = (timing.dbl() - student->getTimeEntry()) / 6;

    if (student->getEntryPeriod(currentPeriod - 1) == 0) {
        student->setEntryPeriod(currentPeriod - 1, (int) timing.dbl());
    }

    if (dropout(bondDuration) || (bondDuration > 21)) {
        emit(dropoutsPerSemester[bondDuration - 1], 1);
        cancelAndDelete(student);
    } else {
//        emit(totalPerSemester[bondDuration - 1], 1);
//        emit(totalMatriculas, 1);
        if (disapprove(bondDuration)) {
            emit(disapprovalsPerSemester[bondDuration - 1], 1);
            student->setDisapprovals(currentPeriod - 1, student->getDisapprovals(currentPeriod - 1) + 1);
            send(student, "out", classCapacity + outPort);

        } else {

            if (graduate(bondDuration)) {
                emit(graduatesPerSemester[bondDuration - 1], 1);
                student->setExitPeriod(currentPeriod - 1, (int) timing.dbl());
//                emit(approvedsPerSemester[bondDuration - 1], 1);
//                int duration = (int) ((student->getExitPeriod(currentPeriod - 1) - student->getEntryPeriod(currentPeriod - 1))/6);
//                emit(duracaoTransicaoPeriod[duration], 1);
                cancelAndDelete(student);
            } else {
                if(currentPeriod == numberOfPeriods){
                    emit(disapprovalsPerSemester[bondDuration - 1], 1);
                    student->setDisapprovals(currentPeriod - 1, student->getDisapprovals(currentPeriod - 1) + 1);
                    send(student, "out", classCapacity + outPort);
                } else {
                    student->setExitPeriod(currentPeriod - 1, (int) timing.dbl());
                    emit(approvedsPerSemester[bondDuration - 1], 1);
//                    int duration = (int) ((student->getExitPeriod(currentPeriod - 1) - student->getEntryPeriod(currentPeriod - 1))/6);
//                    emit(duracaoTransicaoPeriod[duration], 1);
                    send(student, "out", outPort);
                }


            }
        }
    }

}


bool Period::compare(Student *student1, Student *student2) {
    if (student1->getBeginner() && student2->getBeginner()) {
        return student1->getDisapprovals(currentPeriod - 1)
                < student2->getDisapprovals(currentPeriod - 1);
    } else if (student1->getBeginner()) {
        return true;
    } else if (student2->getBeginner()) {
        return false;
    } else {
        return student1->getDisapprovals(currentPeriod - 1)
                < student2->getDisapprovals(currentPeriod - 1);
    }
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


    for(int i = 0; i < 71; ++i){
        char signalNameSizeClass[32];
        sprintf(signalNameSizeClass, "sizeClass%d", i);
        simsignal_t signalSizeClass = registerSignal(signalNameSizeClass);
        cProperty *statisticTemplateSizeClass = getProperties()->get("statisticTemplate", "sizeClassTemplate");
        getEnvir()->addResultRecorders(this, signalSizeClass, signalNameSizeClass, statisticTemplateSizeClass);
        sizeClass[i] = signalSizeClass;
    }

    for (int i = 0; i < 10000; ++i) {
        char signalNameQueueWaitSize[32];
        sprintf(signalNameQueueWaitSize, "queueWaitSize%d", i);
        simsignal_t signalQueueWaitSize = registerSignal(signalNameQueueWaitSize);
        cProperty *statisticTemplateQueueWaitSize = getProperties()->get(
                "statisticTemplate", "queueWaitSizeTemplate");
        getEnvir()->addResultRecorders(this, signalQueueWaitSize,
                signalNameQueueWaitSize, statisticTemplateQueueWaitSize);
        queueWaitSize[i] = signalQueueWaitSize;
    }



    int semesters = 21;

    //    INICIA VARI�VEIS DE STATISTICS DE EVAS�O, GRADUA��O E REPROVA��O POR SEMESTRE
    for (int s = 0; s < semesters; ++s) {

//        char signalNameTotalSemester[32];
        char signalNameDropoutsSemester[32];
        char signalNameGraduatesSemester[32];
        char signalNameDisapprovalsSemester[32];
        char signalNameApprovedsSemester[32];
//        char signalNameDuracaoTransicaoPeriod[32];

//        sprintf(signalNameTotalSemester, "totalPerSemester%d", s);
        sprintf(signalNameDropoutsSemester, "dropoutsPerSemester%d", s);
        sprintf(signalNameGraduatesSemester, "graduatesPerSemester%d", s);
        sprintf(signalNameDisapprovalsSemester, "disapprovalsPerSemester%d", s);
        sprintf(signalNameApprovedsSemester, "approvedsPerSemester%d", s);
//        sprintf(signalNameDuracaoTransicaoPeriod, "duracaoTransicaoPeriod%d", s);

//        simsignal_t signalTotalSemester = registerSignal(signalNameTotalSemester);
        simsignal_t signalDropoutsSemester = registerSignal(signalNameDropoutsSemester);
        simsignal_t signalGraduatesSemester = registerSignal(signalNameGraduatesSemester);
        simsignal_t signalDisapprovalsSemester = registerSignal(signalNameDisapprovalsSemester);
        simsignal_t signalApprovedsSemester = registerSignal(signalNameApprovedsSemester);
//        simsignal_t signalDuracaoTransicaoPeriod = registerSignal(signalNameDuracaoTransicaoPeriod);

//        cProperty *statisticTemplateTotalSemester = getProperties()->get("statisticTemplate", "totalPerSemesterTemplate");
        cProperty *statisticTemplateDropoutsSemester = getProperties()->get("statisticTemplate", "dropoutsPerSemesterTemplate");
        cProperty *statisticTemplateGraduatesSemester = getProperties()->get("statisticTemplate", "graduatesPerSemesterTemplate");
        cProperty *statisticTemplateDisapprovalsSemester = getProperties()->get("statisticTemplate", "disapprovalsPerSemesterTemplate");
        cProperty *statisticTemplateApprovedsSemester = getProperties()->get("statisticTemplate", "approvedsPerSemesterTemplate");
//        cProperty *statisticTemplateDuracaoTransicaoPeriod = getProperties()->get("statisticTemplate", "duracaoTransicaoPeriodTemplate");

//        getEnvir()->addResultRecorders(this, signalTotalSemester, signalNameTotalSemester, statisticTemplateTotalSemester);
        getEnvir()->addResultRecorders(this, signalDropoutsSemester, signalNameDropoutsSemester, statisticTemplateDropoutsSemester);
        getEnvir()->addResultRecorders(this, signalGraduatesSemester, signalNameGraduatesSemester, statisticTemplateGraduatesSemester);
        getEnvir()->addResultRecorders(this, signalDisapprovalsSemester, signalNameDisapprovalsSemester, statisticTemplateDisapprovalsSemester);
        getEnvir()->addResultRecorders(this, signalApprovedsSemester, signalNameApprovedsSemester, statisticTemplateApprovedsSemester);
//        getEnvir()->addResultRecorders(this, signalDuracaoTransicaoPeriod, signalNameDuracaoTransicaoPeriod, statisticTemplateDuracaoTransicaoPeriod);

//        totalPerSemester[s] = signalTotalSemester;
        dropoutsPerSemester[s] = signalDropoutsSemester;
        graduatesPerSemester[s] = signalGraduatesSemester;
        disapprovalsPerSemester[s] = signalDisapprovalsSemester;
        approvedsPerSemester[s] = signalApprovedsSemester;
//        duracaoTransicaoPeriod[s] = signalDuracaoTransicaoPeriod;
   }


}

void Period::emitPeriodData() {
    emit(sizeClass[studentsClassCount], 1);
    emit(queueWaitSize[queueWaiting.getLength()], 1);

    studentsClassCount = 0;
}

void Period::finish() {
}
