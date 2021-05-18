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
    quantidadeEvadidosGeral = registerSignal("quantidadeEvadidosGeral");
    quantidadeAprovadosGeral = registerSignal("quantidadeAprovadosGeral");
    quantidadeReprovadosGeral = registerSignal("quantidadeReprovadosGeral");
    totalEvadidos = registerSignal("totalEvadidos");

    iniciarEstatisticasEmArray();

    capacidadeTurma = 99;
    portaSaidaInicialReprovacao = par("capacidadeTurma");
    indice = par("numero");
    tempo = indice * 6.0;
    portaSaida = 0;
    contadorDeAlunosNaTurma = 0;
    aprovados = 0;
    reprovados = 0;

    aprovadosGeral = 0;
    reprovadosGeral = 0;
    evadidosGeral = 0;
}

void Periodo::handleMessage(cMessage *msg) {
    SimTime tempoAtual = simTime();

    Aluno *aluno = dynamic_cast<Aluno*>(msg);

    if (tempo != tempoAtual) {
        emitirDadosDoPeriodo();
        tempo = tempoAtual;
        portaSaida = 0;
    } else {
        portaSaida++;
    }

    processarAluno(aluno);
}

bool Periodo::compare(Aluno *aluno1, Aluno *aluno2) {
    if (aluno1->getNovato() && aluno2->getNovato()) {
        return aluno1->getReprovacoes(indice - 1)
                < aluno2->getReprovacoes(indice - 1);
    } else if (aluno1->getNovato()) {
        return true;
    } else if (aluno2->getNovato()) {
        return false;
    } else {
        return aluno1->getReprovacoes(indice - 1)
                < aluno2->getReprovacoes(indice - 1);
    }
}

void Periodo::adicionarNaTurma(Aluno *aluno) {

    if (contadorDeAlunosNaTurma == capacidadeTurma) {
        filaEspera.insert(aluno);
    } else if (filaEspera.getLength() > 0) {

        if (compare(aluno, check_and_cast<Aluno*>(filaEspera.front()))) {
            turma.insert(aluno);
        } else {
            Aluno *alunoDaFila = check_and_cast<Aluno*>(filaEspera.pop());
            turma.insert(alunoDaFila);
            filaEspera.insert(aluno);
        }

    } else {
        turma.insert(aluno);
    }
}

bool Periodo::evadir() {

    float taxa = (std::rand() % 100);

    if ((taxa / 100) <= taxasDeEvasao[indice - 1]) {
        return true;
    } else {
        return false;
    }
}

bool Periodo::reter() {
    float taxa = (std::rand() % 100);

    if ((taxa / 100) <= taxasDeRetencao[indice - 1]) {
        return true;
    } else {
        return false;
    }
}

double Periodo::notaAleatoria() {
    double f = (((double) rand() / RAND_MAX) * 0.4) + 0.6;
    return f * (100.0);
}

void Periodo::emitirDadosDoPeriodo() {
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

void Periodo::processarAluno(Aluno *aluno) {

    aluno->setNovato(false);
    adicionarNaTurma(aluno);

    if (turma.getLength() > 0 && contadorDeAlunosNaTurma < capacidadeTurma) {

        Aluno *alunoDaTurma = check_and_cast<Aluno*>(turma.pop());
        contadorDeAlunosNaTurma++;
        int quantidadeSemestres = (int) (tempo.dbl()
                - alunoDaTurma->getEntrada()) / 6;
        bool possibilidadeDeGraduar =
                ((alunoDaTurma->getQuantidadeSemestresCursados(tempo.dbl())
                        - indice + 10) <= 21);
        if (possibilidadeDeGraduar) {
            if (evadir()) {
                evadidos.insert(alunoDaTurma);
                evadidosGeral++;

                emit(
                        evadidosPorSemestre[alunoDaTurma->getQuantidadeSemestresCursados(
                                tempo.dbl()) - 1], 1);
                emit(
                        evadidos_Semestre_periodo[alunoDaTurma->getQuantidadeSemestresCursados(
                                tempo.dbl()) - 1][indice - 1], 1);
                emit(totalEvadidos, 1);
            } else {
                avaliarAluno(alunoDaTurma);
            }
        } else {
            evadidos.insert(alunoDaTurma);
            evadidosGeral++;
            emit(
                    evadidosPorSemestre[alunoDaTurma->getQuantidadeSemestresCursados(
                            tempo.dbl()) - 1], 1);
            emit(
                    evadidos_Semestre_periodo[alunoDaTurma->getQuantidadeSemestresCursados(
                            tempo.dbl()) - 1][indice - 1], 1);
            emit(totalEvadidos, 1);

        }
    }
}

void Periodo::avaliarAluno(Aluno *aluno) {
    aluno->setNovato(false);
    if (reter()) {
        reprovados++;
        reprovadosGeral++;
        emit(
                retidosPorSemestre[aluno->getQuantidadeSemestresCursados(
                        tempo.dbl()) - 1], 1);

        aluno->setReprovacoes(indice - 1, aluno->getReprovacoes(indice - 1) + 1);
        if (aluno->getReprovacoes(indice - 1) >= 4) {
            emit(totalEvadidos, 1);
            evadidos.insert(aluno);
            evadidosGeral++;
            ;

            emit(
                    evadidosPorSemestre[aluno->getQuantidadeSemestresCursados(
                            tempo.dbl()) - 1], 1);
            emit(
                    evadidos_Semestre_periodo[aluno->getQuantidadeSemestresCursados(
                            tempo.dbl()) - 1][indice - 1], 1);
        } else {
            send(aluno, "saida", portaSaidaInicialReprovacao + capacidadeTurma);
            emit(
                    retidos_Semestre_periodo[aluno->getQuantidadeSemestresCursados(
                                            tempo.dbl()) - 1][indice - 1], 1);
        }

    } else {
        aprovados++;
        aprovadosGeral++;
        emit(
                aprovadosPorSemestre[aluno->getQuantidadeSemestresCursados(
                        tempo.dbl()) - 1], 1);
        emit(
                aprovados_Semestre_periodo[aluno->getQuantidadeSemestresCursados(
                        tempo.dbl()) - 1][indice - 1], 1);
        send(aluno, "saida", portaSaida);
    }
}

void Periodo::finish() {
    emit(quantidadeAprovadosGeral, aprovadosGeral);
    emit(quantidadeReprovadosGeral, reprovadosGeral);
    emit(quantidadeEvadidosGeral, evadidosGeral);
}

void Periodo::iniciarEstatisticasEmArray() {
    for (int i = 0; i < 21; ++i) {
        char signalNameI[32];
        sprintf(signalNameI, "evadidosPorSemestre%d", i);
        simsignal_t signalI = registerSignal(signalNameI);
        cProperty *statisticTemplateI = getProperties()->get(
                "statisticTemplate", "evadidosPorSemestreTemplate");
        getEnvir()->addResultRecorders(this, signalI, signalNameI,
                statisticTemplateI);
        evadidosPorSemestre[i] = signalI;
    }

    for (int j = 0; j < 21; ++j) {

        char signalNameJ[32];
        sprintf(signalNameJ, "retidosPorSemestre%d", j);
        simsignal_t signalJ = registerSignal(signalNameJ);
        cProperty *statisticTemplateJ = getProperties()->get(
                "statisticTemplate", "retidosPorSemestreTemplate");
        getEnvir()->addResultRecorders(this, signalJ, signalNameJ,
                statisticTemplateJ);
        retidosPorSemestre[j] = signalJ;
    }

    for (int k = 0; k < 21; ++k) {

        char signalNameK[32];
        sprintf(signalNameK, "aprovadosPorSemestre%d", k);
        simsignal_t signalK = registerSignal(signalNameK);
        cProperty *statisticTemplateK = getProperties()->get(
                "statisticTemplate", "aprovadosPorSemestreTemplate");
        getEnvir()->addResultRecorders(this, signalK, signalNameK,
                statisticTemplateK);
        aprovadosPorSemestre[k] = signalK;
    }

    for (int l = 0; l < 21; ++l) {

        for (int m = 0; m < 10; ++m) {

            char signalNameLM[32];
            sprintf(signalNameLM, "evadidos_Semestre_periodo%d_%d", l, m);
            simsignal_t signalLM = registerSignal(signalNameLM);
            cProperty *statisticTemplateLM = getProperties()->get(
                    "statisticTemplate", "evadidos_Semestre_periodoTemplate");
            getEnvir()->addResultRecorders(this, signalLM, signalNameLM,
                    statisticTemplateLM);
            evadidos_Semestre_periodo[l][m] = signalLM;
        }
    }

    for (int n = 0; n < 21; ++n) {

        for (int o = 0; o < 10; ++o) {

            char signalNameNO[32];
            sprintf(signalNameNO, "aprovados_Semestre_periodo%d_%d", n, o);
            simsignal_t signalNO = registerSignal(signalNameNO);
            cProperty *statisticTemplateNO = getProperties()->get(
                    "statisticTemplate",
                    "aprovados_Semestre_periodoTemplate");
            getEnvir()->addResultRecorders(this, signalNO, signalNameNO,
                    statisticTemplateNO);
            aprovados_Semestre_periodo[n][o] = signalNO;
        }
    }

    for (int p = 0; p < 21; ++p) {

            for (int q = 0; q < 10; ++q) {

                char signalNamePQ[32];
                sprintf(signalNamePQ, "retidos_Semestre_periodo%d_%d", p, q);
                simsignal_t signalPQ = registerSignal(signalNamePQ);
                cProperty *statisticTemplatePQ = getProperties()->get(
                        "statisticTemplate",
                        "retidos_Semestre_periodoTemplate");
                getEnvir()->addResultRecorders(this, signalPQ, signalNamePQ,
                        statisticTemplatePQ);
                retidos_Semestre_periodo[p][q] = signalPQ;
            }
        }
}
