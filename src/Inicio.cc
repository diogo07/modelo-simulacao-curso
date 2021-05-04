#include <string.h>
#include <omnetpp.h>
#include <math.h>
#include "Aluno.h"
using namespace omnetpp;


class Inicio : public cSimpleModule {
  private:
    int portaSaida = 0;
    int contador = 0;
    int tamanhoTurma = 40;
    int numeroExecucoes = 10000;
    simsignal_t totalAlunos;
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void enviarTurma();
};

Define_Module(Inicio);

void Inicio::initialize() {
    totalAlunos = registerSignal("totalAlunos");
    enviarTurma();
}

void Inicio::handleMessage(cMessage *msg) {

    Aluno *aluno = dynamic_cast<Aluno*>(msg);


    if (portaSaida == tamanhoTurma - 1) {

        if((contador / tamanhoTurma) < numeroExecucoes){

            portaSaida = 0;
            enviarTurma();
        }
    } else {
        portaSaida++;
    }

    send(aluno, "saida", portaSaida);

}

void Inicio::enviarTurma() {
    for (int i = 0; i < tamanhoTurma; i++) {
        Aluno *aluno = new Aluno(contador, "aluno "+std::to_string(contador));
        aluno->setEvadido(0);
        aluno->setFaltas(0);
        aluno->setRaca(2);
        aluno->setNovato(true);
        aluno->setEntrada(simTime().dbl());

        contador++;

        SimTime time = simTime();

        if(contador < (tamanhoTurma + 1)){
            scheduleAt(time, aluno);
        } else {
            scheduleAt(time + 1.0, aluno);
        }

        emit(totalAlunos, 1);
    }
}
