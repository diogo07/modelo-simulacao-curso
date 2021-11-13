#include "Periodo.h"
#include <string.h>
#include <omnetpp.h>
#include <math.h>
#include <random>
#include <time.h>

using namespace omnetpp;

Periodo::~Periodo() {}

Define_Module(Periodo);

void Periodo::initialize() {
    tamanhoFilaEspera = registerSignal("tamanhoFilaEspera");
    tamanhoTurma = registerSignal("tamanhoTurma");
    totalMatriculas = registerSignal("totalMatriculas");

    registerSignalArray();

    capacidadeTurma = par("capacidadeTurma");
    numeroPeriodos = par("numeroPeriodos");
    periodoAtual = par("numero");
    tempo = periodoAtual * 6.0;
    portaSaida = 0;
    contadorDeAlunosNaTurma = 0;


    srand((int)time(NULL));
}

void Periodo::handleMessage(cMessage *msg) {
    SimTime tempoAtual = simTime();

    Aluno *aluno = dynamic_cast<Aluno*>(msg);

    if (tempo != tempoAtual) {
        if(capacidadeTurma > contadorDeAlunosNaTurma && filaEspera.getLength() > 0){

            int vagasDisponiveis = capacidadeTurma - contadorDeAlunosNaTurma;
            int vagasParaPreencher = (vagasDisponiveis > filaEspera.getLength()) ? filaEspera.getLength() : vagasDisponiveis;

            for(int i = 0; i < vagasParaPreencher; i++){
                portaSaida++;
                Aluno *alunoDaFila = check_and_cast<Aluno*>(filaEspera.pop());
                turma.insert(alunoDaFila);
                Aluno *alunoDaTurma = check_and_cast<Aluno*>(turma.pop());
                contadorDeAlunosNaTurma++;
                avaliarAlunoPorEvasaoEreprovacao(alunoDaTurma);
            }
        }
        emitirDadosDoPeriodo();
        tempo = tempoAtual;
        portaSaida = 0;
    } else {
        portaSaida++;
    }

    processarAluno(aluno);

}

void Periodo::processarAluno(Aluno *aluno) {

    adicionarNaTurma(aluno);

    if (turma.getLength() > 0) {
        Aluno *alunoDaTurma = check_and_cast<Aluno*>(turma.pop());
        contadorDeAlunosNaTurma++;
        int quantidadeSemestres = (int) (tempo.dbl() - alunoDaTurma->getEntrada()) / 6;
        avaliarAlunoPorEvasaoEreprovacao(alunoDaTurma);
    }
}

void Periodo::adicionarNaTurma(Aluno *aluno) {

    if (contadorDeAlunosNaTurma == capacidadeTurma) { // turma cheia, adiciona na fila de espera
        filaEspera.insert(aluno);
    } else if (filaEspera.getLength() > 0) { // turma tem capacidade, ent�o verifica se tem aluno na fila de espera

        if (compare(aluno, check_and_cast<Aluno*>(filaEspera.front()))) { // compara o aluno que chegou com o primeiro
            turma.insert(aluno);                                          // da fila de espera
        } else {
            EV << "pegando aluno da fila de espera" << endl;
            Aluno *alunoDaFila = check_and_cast<Aluno*>(filaEspera.pop());
            turma.insert(alunoDaFila);
            filaEspera.insert(aluno);
        }

    } else { // fila de espera vazia, adiciona o aluno na turma
        turma.insert(aluno);
    }
}

void Periodo::avaliarAlunoPorEvasaoEreprovacao(Aluno *aluno) {

    if(periodoAtual == 1){
        EV << filaEspera.getLength() << " - " << capacidadeTurma << " - " << portaSaida << " - " << contadorDeAlunosNaTurma << endl;
    }

    aluno->setNovato(false);
    aluno->setDuracaoVinculo(aluno->getDuracaoVinculo() + 1);
    int duracaoVinculo = (tempo.dbl() - aluno->getEntrada()) / 6;

    if (aluno->getEntradaPeriodo(periodoAtual - 1) == 0) {
        aluno->setEntradaPeriodo(periodoAtual - 1, (int) tempo.dbl());
    }

    if (evadir(aluno->getDuracaoVinculo()) || (aluno->getDuracaoVinculo() > 21)) {
//        EV << "evadido " << duracaoVinculo << " - " << aluno->getEntrada() << " - " << tempo << endl;
        emit(evadidosPorSemestre[aluno->getDuracaoVinculo() - 1], 1);
        cancelAndDelete(aluno);
    } else {
        emit(totalPorSemestre[aluno->getDuracaoVinculo() - 1], 1);
        emit(totalMatriculas, 1);
        if (reprovar(aluno->getDuracaoVinculo())) {
            emit(reprovadosPorSemestre[aluno->getDuracaoVinculo() - 1], 1);
            aluno->setReprovacoes(periodoAtual - 1, aluno->getReprovacoes(periodoAtual - 1) + 1);
            send(aluno, "saida", capacidadeTurma + portaSaida);

        } else {

            if (graduar(aluno->getDuracaoVinculo())) {
                emit(graduadosPorSemestre[aluno->getDuracaoVinculo() - 1], 1);
                aluno->setSaidaPeriodo(periodoAtual - 1, (int) tempo.dbl());
                emit(aprovadosPorSemestre[aluno->getDuracaoVinculo() - 1], 1);
//                int duracao = (int) ((aluno->getSaidaPeriodo(periodoAtual - 1) - aluno->getEntradaPeriodo(periodoAtual - 1))/6);
//                emit(duracaoTransicaoPeriodo[duracao], 1);
                cancelAndDelete(aluno);
            } else {
                if(periodoAtual == numeroPeriodos){
                    emit(reprovadosPorSemestre[aluno->getDuracaoVinculo() - 1], 1);
                    aluno->setReprovacoes(periodoAtual - 1, aluno->getReprovacoes(periodoAtual - 1) + 1);
                    send(aluno, "saida", capacidadeTurma + portaSaida);
                } else {
                    aluno->setSaidaPeriodo(periodoAtual - 1, (int) tempo.dbl());
                    emit(aprovadosPorSemestre[aluno->getDuracaoVinculo() - 1], 1);
//                    int duracao = (int) ((aluno->getSaidaPeriodo(periodoAtual - 1) - aluno->getEntradaPeriodo(periodoAtual - 1))/6);
//                    emit(duracaoTransicaoPeriodo[duracao], 1);
                    send(aluno, "saida", portaSaida);
                }


            }
        }
    }

}


bool Periodo::compare(Aluno *aluno1, Aluno *aluno2) {
    if (aluno1->getNovato() && aluno2->getNovato()) {
        return aluno1->getReprovacoes(periodoAtual - 1)
                < aluno2->getReprovacoes(periodoAtual - 1);
    } else if (aluno1->getNovato()) {
        return true;
    } else if (aluno2->getNovato()) {
        return false;
    } else {
        return aluno1->getReprovacoes(periodoAtual - 1)
                < aluno2->getReprovacoes(periodoAtual - 1);
    }
}

float Periodo::randomValue(){
    return (float) rand() / (float) RAND_MAX;
}

bool Periodo::evadir(int duracaoVinculo) {

    float rNumber = randomValue();

    if (probsTipo == 0) {
        float probEvasao = (float) probsEvasaoPeriodo[curso][periodoAtual - 1];
        return rNumber < probEvasao;

    } else if (probsTipo == 1) {
        float probEvasao = (float) probsEvasaoDuracaoVinculo[curso][duracaoVinculo - 1];
        return rNumber < probEvasao;

    } else {
        float probEvasao = (float) probsEvasaoPeriodoDuracaoVinculo[curso][periodoAtual - 1][duracaoVinculo - 1];
        return rNumber < probEvasao;
    }

}

bool Periodo::reprovar(int duracaoVinculo) {

    float rNumber = randomValue();

    if (probsTipo == 0) {

        float probReprovacao = (float) probsReprovacaoPeriodo[curso][periodoAtual - 1];
        return rNumber < probReprovacao;

    } else if (probsTipo == 1) {

        float probReprovacao = (float) probsReprovacaoDuracaoVinculo[curso][duracaoVinculo - 1];
        return rNumber < probReprovacao;

    } else {

        float probReprovacao = (float) probsReprovacaoPeriodoDuracaoVinculo[curso][periodoAtual - 1][duracaoVinculo - 1];
        return rNumber < probReprovacao;

    }
}

bool Periodo::graduar(int duracaoVinculo) {
    float rNumber = randomValue();
    if (probsTipo == 1) {
        float probGraduacao = (float) probsGraduacaoDuracaoVinculo[curso][duracaoVinculo - 1];
        return rNumber < probGraduacao;

    } else {
        float probGraduacao = (float) probsGraduacaoPeriodoDuracaoVinculo[curso][periodoAtual - 1][duracaoVinculo - 1];
        return rNumber < probGraduacao;
    }

}

void Periodo::registerSignalArray() {

    int semestres = 21;

    //    INICIA VARI�VEIS DE STATISTICS DE EVAS�O, GRADUA��O E REPROVA��O POR SEMESTRE
    for (int s = 0; s < semestres; ++s) {

        char signalNameTotalSemestre[32];
        char signalNameEvadidosSemestre[32];
        char signalNameGraduadosSemestre[32];
        char signalNameReprovadosSemestre[32];
        char signalNameAprovadosSemestre[32];
        char signalNameDuracaoTransicaoPeriodo[32];

        sprintf(signalNameTotalSemestre, "totalPorSemestre%d", s);
        sprintf(signalNameEvadidosSemestre, "evadidosPorSemestre%d", s);
        sprintf(signalNameGraduadosSemestre, "graduadosPorSemestre%d", s);
        sprintf(signalNameReprovadosSemestre, "reprovadosPorSemestre%d", s);
        sprintf(signalNameAprovadosSemestre, "aprovadosPorSemestre%d", s);
        sprintf(signalNameDuracaoTransicaoPeriodo, "duracaoTransicaoPeriodo%d", s);

        simsignal_t signalTotalSemestre = registerSignal(signalNameTotalSemestre);
        simsignal_t signalEvadidosSemestre = registerSignal(signalNameEvadidosSemestre);
        simsignal_t signalGraduadosSemestre = registerSignal(signalNameGraduadosSemestre);
        simsignal_t signalReprovadosSemestre = registerSignal(signalNameReprovadosSemestre);
        simsignal_t signalAprovadosSemestre = registerSignal(signalNameAprovadosSemestre);
        simsignal_t signalDuracaoTransicaoPeriodo = registerSignal(signalNameDuracaoTransicaoPeriodo);

        cProperty *statisticTemplateTotalSemestre = getProperties()->get("statisticTemplate", "totalPorSemestreTemplate");
        cProperty *statisticTemplateEvadidosSemestre = getProperties()->get("statisticTemplate", "evadidosPorSemestreTemplate");
        cProperty *statisticTemplateGraduadosSemestre = getProperties()->get("statisticTemplate", "graduadosPorSemestreTemplate");
        cProperty *statisticTemplateReprovadosSemestre = getProperties()->get("statisticTemplate", "reprovadosPorSemestreTemplate");
        cProperty *statisticTemplateAprovadosSemestre = getProperties()->get("statisticTemplate", "aprovadosPorSemestreTemplate");
        cProperty *statisticTemplateDuracaoTransicaoPeriodo = getProperties()->get("statisticTemplate", "duracaoTransicaoPeriodoTemplate");

        getEnvir()->addResultRecorders(this, signalTotalSemestre, signalNameTotalSemestre, statisticTemplateTotalSemestre);
        getEnvir()->addResultRecorders(this, signalEvadidosSemestre, signalNameEvadidosSemestre, statisticTemplateEvadidosSemestre);
        getEnvir()->addResultRecorders(this, signalGraduadosSemestre, signalNameGraduadosSemestre, statisticTemplateGraduadosSemestre);
        getEnvir()->addResultRecorders(this, signalReprovadosSemestre, signalNameReprovadosSemestre, statisticTemplateReprovadosSemestre);
        getEnvir()->addResultRecorders(this, signalAprovadosSemestre, signalNameAprovadosSemestre, statisticTemplateAprovadosSemestre);
        getEnvir()->addResultRecorders(this, signalDuracaoTransicaoPeriodo, signalNameDuracaoTransicaoPeriodo, statisticTemplateDuracaoTransicaoPeriodo);

        totalPorSemestre[s] = signalTotalSemestre;
        evadidosPorSemestre[s] = signalEvadidosSemestre;
        graduadosPorSemestre[s] = signalGraduadosSemestre;
        reprovadosPorSemestre[s] = signalReprovadosSemestre;
        aprovadosPorSemestre[s] = signalAprovadosSemestre;
        duracaoTransicaoPeriodo[s] = signalDuracaoTransicaoPeriodo;
   }


}

void Periodo::emitirDadosDoPeriodo() {
    emit(tamanhoTurma, contadorDeAlunosNaTurma);
    emit(tamanhoFilaEspera, filaEspera.getLength());
    contadorDeAlunosNaTurma = 0;
}

void Periodo::finish() {
}
