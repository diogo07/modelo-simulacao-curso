#include <string.h>
#include <omnetpp.h>
#include <math.h>

#include "Student.h"
using namespace omnetpp;

class Ingress: public cSimpleModule {
private:
    int outPort = 0;
    int sizeClass = 40;
    int numberOfPlays = 1000;
    bool runUndefinitely = false;
//    simsignal_t totalStudents;
//    simsignal_t totalTurmas;
protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void sendClass(int indice);
};

Define_Module(Ingress);

void Ingress::initialize() {
//    totalStudents = registerSignal("totalStudents");
//    totalTurmas = registerSignal("totalTurmas");

    if (runUndefinitely) {
        sendClass(0);
    } else {
        for (int i = 0; i < numberOfPlays; i++) {
            sendClass(i);
        }
    }

}

void Ingress::handleMessage(cMessage *msg) {

    Student *student = dynamic_cast<Student*>(msg);

    if (outPort == (sizeClass - 1)) {
        outPort = 0;
        if (runUndefinitely) {
            sendClass(((int) student->getTimeEntry() / 6) + 1);
        }
    } else {
        outPort++;
    }

    send(student, "out", outPort);

}

void Ingress::sendClass(int timing) {
    for (int i = 0; i < sizeClass; i++) {
        Student *student = new Student();
        student->setBeginner(true);
        student->setBondDuration(0);
        student->setTimeEntry(timing * 6.0);
        student->setEntryPeriod(0, timing * 6.0);
        scheduleAt((SimTime) (timing * 6.0), student);
//        emit(totalStudents, 1);
    }

//    emit(totalTurmas, 1);
}
