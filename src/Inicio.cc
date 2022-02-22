
#include <string.h>
#include <omnetpp.h>
#include <math.h>
#include "Aluno.h"
using namespace omnetpp;


class Inicio : public cSimpleModule {
  private:
    int portaSaida = 0;
    int tamanhoTurma = 40;
    int numeroExecucoes = 1000;
    bool executarPorTempoIndeterminado = false;
//    simsignal_t totalAlunos;
//    simsignal_t totalTurmas;
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void enviarTurma(int indice);
};

Define_Module(Inicio);

void Inicio::initialize() {
//    totalAlunos = registerSignal("totalAlunos");
//    totalTurmas = registerSignal("totalTurmas");

    if(executarPorTempoIndeterminado){
        enviarTurma(0);
    } else {
        for (int i = 0; i < numeroExecucoes; i++) {
            enviarTurma(i);
        }
    }

}

void Inicio::handleMessage(cMessage *msg) {

    Aluno *aluno = dynamic_cast<Aluno*>(msg);

    if(portaSaida == (tamanhoTurma - 1)){
        portaSaida = 0;
        if(executarPorTempoIndeterminado){
            enviarTurma(((int)aluno->getEntrada()/6)+1);
        }
    } else {
        portaSaida++;
    }

    send(aluno, "saida", portaSaida);

}

void Inicio::enviarTurma(int tempo) {
    for (int i = 0; i < tamanhoTurma; i++) {
        Aluno *aluno = new Aluno();
        aluno->setNovato(true);
        aluno->setDuracaoVinculo(0);
        aluno->setEntrada(tempo * 6.0);
        aluno->setEntradaPeriodo(0, tempo * 6.0);
        scheduleAt((SimTime)(tempo * 6.0), aluno);
//        emit(totalAlunos, 1);
    }

//    emit(totalTurmas, 1);
}
