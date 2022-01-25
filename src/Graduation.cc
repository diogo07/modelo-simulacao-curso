#include <string.h>
#include <omnetpp.h>
#include <math.h>

#include "Student.h"


using namespace omnetpp;
class Graduation : public cSimpleModule {

//    simsignal_t totalFormados;
//    simsignal_t quantidadePeriodos;

  protected:
    virtual void handleMessage(cMessage *msg) override;
    virtual void initialize() override;
    virtual void finish() override;
};

Define_Module(Graduation);

void Graduation::initialize(){
//    totalFormados = registerSignal("totalFormados");
//    quantidadePeriodos = registerSignal("quantidadePeriodos");
}

void Graduation::handleMessage(cMessage *msg) {

    Student *aluno = dynamic_cast<Student*>(msg);
    double time = simTime().dbl();
    aluno->setExitTime(time-6);
//    emit(totalFormados, 1);
//    emit(quantidadePeriodos, aluno->getQuantidadeSemestresCursados(time - 6));
    cancelAndDelete(msg);
}

void Graduation::finish(){
}


