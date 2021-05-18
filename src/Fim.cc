#include <string.h>
#include <omnetpp.h>
#include <math.h>
#include "Aluno.h"


using namespace omnetpp;
class Fim : public cSimpleModule {

    simsignal_t totalFormados;
    simsignal_t quantidadePeriodos;

  protected:
    virtual void handleMessage(cMessage *msg) override;
    virtual void initialize() override;
    virtual void finish() override;
};

Define_Module(Fim);

void Fim::initialize(){
    totalFormados = registerSignal("totalFormados");
    quantidadePeriodos = registerSignal("quantidadePeriodos");
}

void Fim::handleMessage(cMessage *msg) {

    Aluno *aluno = dynamic_cast<Aluno*>(msg);
    double time = simTime().dbl();
    aluno->setSaida(time-6);
    emit(totalFormados, 1);
    emit(quantidadePeriodos, aluno->getQuantidadeSemestresCursados(time - 6));
    if(aluno->getQuantidadeSemestresCursados(time - 6) > 21){
        EV << "Quantidade: " << aluno->getQuantidadeSemestresCursados(time - 6) << endl;
    }

    cancelAndDelete(msg);
}

void Fim::finish(){
}


