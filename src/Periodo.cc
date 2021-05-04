#include "Periodo.h"
#include <string.h>
#include <omnetpp.h>
#include <math.h>
#include <random>

using namespace omnetpp;


Periodo::~Periodo() {

}

Define_Module(Periodo);


void Periodo::initialize() {
    tamanhoFilaEspera = registerSignal("tamanhoFilaEspera");
    tamanhoTurma = registerSignal("tamanhoTurma");
    quantidadeEvadidos = registerSignal("quantidadeEvadidos");
    quantidadeAprovados = registerSignal("quantidadeAprovados");
    quantidadeReprovados = registerSignal("quantidadeReprovados");
    totalEvadidos = registerSignal("totalEvadidos");

    capacidadeTurma = 99;
    portaSaidaInicialReprovacao = par("capacidadeTurma");
    indice = par("numero");
    tempo = indice * 1.0;
    portaSaida = 0;
    contadorDeAlunosNaTurma = 0;
    aprovados = 0;
    reprovados = 0;
}


void Periodo::handleMessage(cMessage *msg) {
    SimTime tempoAtual = simTime();

    Aluno *aluno = dynamic_cast<Aluno*>(msg);

    if(tempo != tempoAtual){
        emitirDadosDoPeriodo();
        tempo = tempoAtual;
        portaSaida = 0;
    } else {
        portaSaida++;
    }

    processarAluno(aluno);
}

bool Periodo::compare(Aluno * aluno1, Aluno * aluno2) {
    if (aluno1->getNovato() && aluno2->getNovato()){
        return aluno1->getReprovacoes(indice - 1) < aluno2->getReprovacoes(indice - 1);
    } else if (aluno1->getNovato()){
        return true;
    } else if (aluno2->getNovato()){
        return false;
    } else {
        return aluno1->getReprovacoes(indice - 1) < aluno2->getReprovacoes(indice - 1);
    }
}


void Periodo::adicionarNaTurma(Aluno *aluno) {

    if (contadorDeAlunosNaTurma == capacidadeTurma) {
        filaEspera.insert(aluno);
    } else if (filaEspera.getLength() > 0){

        if(compare(aluno, check_and_cast<Aluno *>(filaEspera.front()))){
           turma.insert(aluno);
        } else {
            Aluno *alunoDaFila = check_and_cast<Aluno *>(filaEspera.pop());
            turma.insert(alunoDaFila);
            filaEspera.insert(aluno);
        }

    } else {
        turma.insert(aluno);
    }
}

bool Periodo::evadir(){

    float taxa = (std::rand() % 100);

    if((taxa / 100) <= taxasDeEvasao[indice - 1]) {
        return true;
    } else {
        return false;
    }
}


bool Periodo::reter(){
    float taxa = (std::rand() % 100);

    if((taxa / 100) <= taxasDeRetencao[indice - 1]) {
        return true;
    } else {
        return false;
    }
}


double Periodo::notaAleatoria() {
    double f =  (((double) rand() / RAND_MAX) * 0.4) + 0.6;
    return f * (100.0);
}


void Periodo::emitirDadosDoPeriodo(){
    emit(tamanhoTurma, contadorDeAlunosNaTurma);
    emit(quantidadeEvadidos, evadidos.getLength());
    emit(quantidadeAprovados, aprovados);
    emit(quantidadeReprovados, reprovados);
    emit(tamanhoFilaEspera, filaEspera.getLength());
    contadorDeAlunosNaTurma = 0;
    aprovados = 0;
    reprovados = 0;
    evadidos.clear();
}


void Periodo::processarAluno(Aluno *aluno){

    aluno->setNovato(false);
    adicionarNaTurma(aluno);

    if (turma.getLength() > 0 && contadorDeAlunosNaTurma < capacidadeTurma){

        Aluno *alunoAvaliado = check_and_cast<Aluno *>(turma.pop());
        contadorDeAlunosNaTurma++;

        if(evadir()){
            evadidos.insert(alunoAvaliado);
            emit(totalEvadidos, 1);
        } else {
            avaliarAluno(alunoAvaliado);
        }
    }
}


void Periodo::avaliarAluno(Aluno *aluno){
    aluno->setNovato(false);

    if (reter()) {
        reprovados++;
        aluno->setReprovacoes(indice - 1, aluno->getReprovacoes(indice - 1)+1);
        if(aluno->getReprovacoes(indice - 1) >= 4 || aluno->getQuantidadeSemestresCursados() > 21){
            emit(totalEvadidos, 1);
            evadidos.insert(aluno);
        } else {
            send(aluno, "saida", portaSaidaInicialReprovacao + capacidadeTurma);
        }


    } else {
        aprovados++;
        send(aluno, "saida", portaSaida);
    }
}

void Periodo::finish(){}

