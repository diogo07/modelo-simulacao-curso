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
    quantidadeEvadidosGeral = registerSignal("quantidadeEvadidosGeral");
    quantidadeRetidosGeral = registerSignal("quantidadeRetidosGeral");
    totalEvadidos = registerSignal("totalEvadidos");

    registerSignalArray();

    capacidadeTurma = 199;
    portaSaidaInicialRetencao = par("capacidadeTurma");
    indice = par("numero");
    tempo = indice * 6.0;
    portaSaida = 0;
    contadorDeAlunosNaTurma = 0;

    retidosGeral = 0;
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

void Periodo::processarAluno(Aluno *aluno) {

    aluno->setNovato(false);
    adicionarNaTurma(aluno);

    if (turma.getLength() > 0) {

        Aluno *alunoDaTurma = check_and_cast<Aluno*>(turma.pop());

        contadorDeAlunosNaTurma++;
        int quantidadeSemestres = (int) (tempo.dbl()
                - alunoDaTurma->getEntrada()) / 6;
//        bool possibilidadeDeGraduar =
//                ((alunoDaTurma->getQuantidadeSemestresCursados(tempo.dbl())
//                        - indice + 10) <= 21);
//        if (possibilidadeDeGraduar) {
            avaliarAluno(alunoDaTurma);
//        } else {
//            evadidosGeral++;
//            emit(evadidosPorPeriodo[indice - 1], 1);
//            emit(evadidosPorSemestre[quantidadeSemestres - 1], 1);
//            emit(evadidosSemestrePeriodo[quantidadeSemestres - 1][indice - 1], 1);
//            emit(totalEvadidos, 1);
//            cancelAndDelete(alunoDaTurma);
//        }
    }
}

void Periodo::avaliarAluno(Aluno *aluno) {
    aluno->setNovato(false);
    int quantidadeSemestres = (int) (tempo.dbl() - aluno->getEntrada()) / 6;

    if (reter(quantidadeSemestres)) {
        retidosGeral++;
        emit(retidosPorPeriodo[indice - 1], 1);
        emit(retidosPorSemestre[quantidadeSemestres - 1], 1);
        emit(retidosSemestrePeriodo[quantidadeSemestres - 1][indice - 1], 1);
        aluno->setReprovacoes(indice - 1, aluno->getReprovacoes(indice - 1) + 1);
//        if (aluno->getReprovacoes(indice - 1) >= 4) {
//            evadidos++;
//            emit(totalEvadidos, 1);
//            emit(evadidosPorSemestre[quantidadeSemestres - 1], 1);
//            emit(evadidosSemestrePeriodo[quantidadeSemestres - 1][indice - 1], 1);
//        } else {
//        send(aluno, "saida", portaSaidaInicialRetencao + capacidadeTurma);
//        }

        if (evadir(quantidadeSemestres)) {
            evadidosGeral++;
            emit(evadidosPorPeriodo[indice - 1], 1);
            emit(evadidosPorSemestre[quantidadeSemestres - 1], 1);
            emit(evadidosSemestrePeriodo[quantidadeSemestres - 1][indice - 1], 1);
            emit(totalEvadidos, 1);
            cancelAndDelete(aluno);
        } else {
            send(aluno, "saida", portaSaidaInicialRetencao + capacidadeTurma);
        }
    } else {

        if (evadir(quantidadeSemestres)) {
            evadidosGeral++;
            emit(evadidosPorPeriodo[indice - 1], 1);
            emit(evadidosPorSemestre[quantidadeSemestres - 1], 1);
            emit(evadidosSemestrePeriodo[quantidadeSemestres - 1][indice - 1],
                    1);
            emit(totalEvadidos, 1);
            cancelAndDelete(aluno);
        } else {
            send(aluno, "saida", portaSaida);
        }

    }
}

void Periodo::adicionarNaTurma(Aluno *aluno) {
    turma.insert(aluno);
//    if (contadorDeAlunosNaTurma == capacidadeTurma) {
//        filaEspera.insert(aluno);
//    } else if (filaEspera.getLength() > 0) {
//
//        if (compare(aluno, check_and_cast<Aluno*>(filaEspera.front()))) {
//            turma.insert(aluno);
//        } else {
//            Aluno *alunoDaFila = check_and_cast<Aluno*>(filaEspera.pop());
//            turma.insert(alunoDaFila);
//            filaEspera.insert(aluno);
//        }
//
//    } else {
//        turma.insert(aluno);
//    }
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

bool Periodo::evadir(int semestre) {

    float rNumber = (std::rand() % 1000) * 0.001;

    if (analiseTipo == 0) {
        float probEvasao = (float) probDeEvasaoPeriodo[analiseCurso][indice - 1];
        return rNumber <= probEvasao;

    } else if (analiseTipo == 1) {

        float probEvasao = (float) probDeEvasaoSemestre[analiseCurso][semestre - 1];
        return rNumber <= probEvasao;

    } else {

        float probEvasao = (float) probDeEvasaoPeriodoSemestre[analiseCurso][indice - 1][semestre - 1];
        return rNumber <= probEvasao;
    }

}

bool Periodo::reter(int semestre) {

    float rNumber = (std::rand() % 1000) * 0.001;

    if (analiseTipo == 0) {

        float probRetencao = (float) probDeRetencaoPeriodo[analiseCurso][indice - 1];
        return rNumber <= probRetencao;

    } else if (analiseTipo == 1) {

        float probRetencao = (float) probDeRetencaoSemestre[analiseCurso][semestre - 1];
        return rNumber <= probRetencao;

    } else {

        float probRetencao = (float) probDeRetencaoPeriodoSemestre[analiseCurso][indice - 1][semestre - 1];
        return rNumber <= probRetencao;

    }
}

void Periodo::registerSignalArray() {

    int semestres = 21;
    int periodos = 10;


//    INICIA VARIÁVEIS DE STATISTICS DE EVASÃO E RETENÇÃO POR PERÍODO
    for (int p = 0; p < periodos; ++p) {

        char signalNameEvadidosPeriodo[32];
        char signalNameRetidosPeriodo[32];

        sprintf(signalNameEvadidosPeriodo, "evadidosPorPeriodo%d", p);
        sprintf(signalNameRetidosPeriodo, "retidosPorPeriodo%d", p);

        simsignal_t signalEvadidosPeriodo = registerSignal(signalNameEvadidosPeriodo);
        simsignal_t signalRetidosPeriodo = registerSignal(signalNameRetidosPeriodo);

        cProperty *statisticTemplateEvadidosPeriodo = getProperties()->get("statisticTemplate", "retidosPorPeriodoTemplate");
        cProperty *statisticTemplateRetidosPeriodo = getProperties()->get("statisticTemplate", "retidosPorPeriodoTemplate");

        getEnvir()->addResultRecorders(this, signalEvadidosPeriodo, signalNameEvadidosPeriodo, statisticTemplateEvadidosPeriodo);
        getEnvir()->addResultRecorders(this, signalRetidosPeriodo, signalNameRetidosPeriodo, statisticTemplateRetidosPeriodo);

        evadidosPorPeriodo[p] = signalEvadidosPeriodo;
        retidosPorPeriodo[p] = signalRetidosPeriodo;
    }


    for (int s = 0; s < semestres; ++s) {
        char signalNameEvadidosSemestre[32];
        char signalNameRetidosSemestre[32];

        sprintf(signalNameEvadidosSemestre, "evadidosPorSemestre%d", s);
        sprintf(signalNameRetidosSemestre, "retidosPorSemestre%d", s);

        simsignal_t signalEvadidosSemestre = registerSignal(signalNameEvadidosSemestre);
        simsignal_t signalRetidosSemestre = registerSignal(signalNameRetidosSemestre);

        cProperty *statisticTemplateEvadidosSemestre = getProperties()->get("statisticTemplate", "evadidosPorSemestreTemplate");
        cProperty *statisticTemplateRetidosSemestre = getProperties()->get("statisticTemplate", "retidosPorSemestreTemplate");

        getEnvir()->addResultRecorders(this, signalEvadidosSemestre, signalNameEvadidosSemestre, statisticTemplateEvadidosSemestre);
        getEnvir()->addResultRecorders(this, signalRetidosSemestre, signalNameRetidosSemestre, statisticTemplateRetidosSemestre);

        evadidosPorSemestre[s] = signalEvadidosSemestre;
        retidosPorSemestre[s] = signalRetidosSemestre;
   }



   for (int sem = 0; sem < semestres; ++sem) {

        for (int pe = 0; pe < periodos; ++pe) {

            char signalNameEvadidosSemestrePeriodo[32];
            char signalNameRetidosSemestrePeriodo[32];

            sprintf(signalNameEvadidosSemestrePeriodo, "evadidosSemestrePeriodo%d_%d", sem, pe);
            sprintf(signalNameRetidosSemestrePeriodo, "retidosSemestrePeriodo%d_%d", sem, pe);

            simsignal_t signalEvadidosSemestrePeriodo = registerSignal(signalNameEvadidosSemestrePeriodo);
            simsignal_t signalRetidosSemestrePeriodo = registerSignal(signalNameRetidosSemestrePeriodo);

            cProperty *statisticTemplateEvadidosSemestrePeriodo = getProperties()->get("statisticTemplate", "evadidosSemestrePeriodoTemplate");
            cProperty *statisticTemplateRetidosSemestrePeriodo = getProperties()->get("statisticTemplate", "retidosSemestrePeriodoTemplate");

            getEnvir()->addResultRecorders(this, signalEvadidosSemestrePeriodo, signalNameEvadidosSemestrePeriodo, statisticTemplateEvadidosSemestrePeriodo);
            getEnvir()->addResultRecorders(this, signalRetidosSemestrePeriodo, signalNameRetidosSemestrePeriodo, statisticTemplateRetidosSemestrePeriodo);

            evadidosSemestrePeriodo[sem][pe] = signalEvadidosSemestrePeriodo;
            retidosSemestrePeriodo[sem][pe] = signalRetidosSemestrePeriodo;

        }
    }




//    for (int g = 0; g < periodos; ++g) {
//        char signalNameG[32];
//        sprintf(signalNameG, "retidosPorPeriodo%d", g);
//        simsignal_t signalG = registerSignal(signalNameG);
//        cProperty *statisticTemplateG = getProperties()->get(
//                "statisticTemplate", "retidosPorPeriodoTemplate");
//        getEnvir()->addResultRecorders(this, signalG, signalNameG,
//                statisticTemplateG);
//        retidosPorPeriodo[g] = signalG;
//    }
//
//
//    for (int h = 0; h < periodos; ++h) {
//        char signalNameH[32];
//        sprintf(signalNameH, "evadidosPorPeriodo%d", h);
//        simsignal_t signalH = registerSignal(signalNameH);
//        cProperty *statisticTemplateH = getProperties()->get(
//                "statisticTemplate", "evadidosPorPeriodoTemplate");
//        getEnvir()->addResultRecorders(this, signalH, signalNameH,
//                statisticTemplateH);
//        evadidosPorPeriodo[h] = signalH;
//    }
//
//
//    for (int i = 0; i < semestres; ++i) {
//        char signalNameI[32];
//        sprintf(signalNameI, "evadidosPorSemestre%d", i);
//        simsignal_t signalI = registerSignal(signalNameI);
//        cProperty *statisticTemplateI = getProperties()->get(
//                "statisticTemplate", "evadidosPorSemestreTemplate");
//        getEnvir()->addResultRecorders(this, signalI, signalNameI,
//                statisticTemplateI);
//        evadidosPorSemestre[i] = signalI;
//    }
//
//    for (int j = 0; j < semestres; ++j) {
//
//        char signalNameJ[32];
//        sprintf(signalNameJ, "retidosPorSemestre%d", j);
//        simsignal_t signalJ = registerSignal(signalNameJ);
//        cProperty *statisticTemplateJ = getProperties()->get(
//                "statisticTemplate", "retidosPorSemestreTemplate");
//        getEnvir()->addResultRecorders(this, signalJ, signalNameJ,
//                statisticTemplateJ);
//        retidosPorSemestre[j] = signalJ;
//    }
//
//    for (int k = 0; k < semestres; ++k) {
//
//        char signalNameK[32];
//        sprintf(signalNameK, "aprovadosPorSemestre%d", k);
//        simsignal_t signalK = registerSignal(signalNameK);
//        cProperty *statisticTemplateK = getProperties()->get(
//                "statisticTemplate", "aprovadosPorSemestreTemplate");
//        getEnvir()->addResultRecorders(this, signalK, signalNameK,
//                statisticTemplateK);
//        aprovadosPorSemestre[k] = signalK;
//    }
//
//    for (int t = 0; t < semestres; ++t) {
//
//        char signalNameT[32];
//        sprintf(signalNameT, "graduadosPorSemestre%d", t);
//        simsignal_t signalT = registerSignal(signalNameT);
//        cProperty *statisticTemplateT = getProperties()->get(
//                "statisticTemplate", "graduadosPorSemestreTemplate");
//        getEnvir()->addResultRecorders(this, signalT, signalNameT,
//                statisticTemplateT);
//        aprovadosPorSemestre[t] = signalT;
//    }
//
//    for (int l = 0; l < semestres; ++l) {
//
//        for (int m = 0; m < periodos; ++m) {
//
//            char signalNameLM[32];
//            sprintf(signalNameLM, "evadidos_Semestre_periodo%d_%d", l, m);
//            simsignal_t signalLM = registerSignal(signalNameLM);
//            cProperty *statisticTemplateLM = getProperties()->get(
//                    "statisticTemplate", "evadidos_Semestre_periodoTemplate");
//            getEnvir()->addResultRecorders(this, signalLM, signalNameLM,
//                    statisticTemplateLM);
//            evadidos_Semestre_periodo[l][m] = signalLM;
//        }
//    }
//
//    for (int n = 0; n < semestres; ++n) {
//
//        for (int o = 0; o < periodos; ++o) {
//
//            char signalNameNO[32];
//            sprintf(signalNameNO, "aprovados_Semestre_periodo%d_%d", n, o);
//            simsignal_t signalNO = registerSignal(signalNameNO);
//            cProperty *statisticTemplateNO = getProperties()->get(
//                    "statisticTemplate", "aprovados_Semestre_periodoTemplate");
//            getEnvir()->addResultRecorders(this, signalNO, signalNameNO,
//                    statisticTemplateNO);
//            aprovados_Semestre_periodo[n][o] = signalNO;
//        }
//    }
//
//    for (int p = 0; p < semestres; ++p) {
//
//        for (int q = 0; q < periodos; ++q) {
//
//            char signalNamePQ[32];
//            sprintf(signalNamePQ, "retidos_Semestre_periodo%d_%d", p, q);
//            simsignal_t signalPQ = registerSignal(signalNamePQ);
//            cProperty *statisticTemplatePQ = getProperties()->get(
//                    "statisticTemplate", "retidos_Semestre_periodoTemplate");
//            getEnvir()->addResultRecorders(this, signalPQ, signalNamePQ,
//                    statisticTemplatePQ);
//            retidos_Semestre_periodo[p][q] = signalPQ;
//        }
//    }
//
//    for (int r = 0; r < semestres; ++r) {
//
//        for (int s = 0; s < periodos; ++s) {
//
//            char signalNameRS[32];
//            sprintf(signalNameRS, "turma_Semestre_periodo%d_%d", r, s);
//            simsignal_t signalRS = registerSignal(signalNameRS);
//            cProperty *statisticTemplateRS = getProperties()->get(
//                    "statisticTemplate", "turma_Semestre_periodoTemplate");
//            getEnvir()->addResultRecorders(this, signalRS, signalNameRS,
//                    statisticTemplateRS);
//            turma_Semestre_periodo[r][s] = signalRS;
//        }
//    }
}

void Periodo::emitirDadosDoPeriodo() {
    emit(tamanhoTurma, contadorDeAlunosNaTurma);
    emit(tamanhoFilaEspera, filaEspera.getLength());
    contadorDeAlunosNaTurma = 0;
}

void Periodo::finish() {
    emit(quantidadeEvadidosGeral, evadidosGeral);
    emit(quantidadeRetidosGeral, retidosGeral);
}
