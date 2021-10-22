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
    quantidadeEvadidosGeral = registerSignal("quantidadeEvadidosGeral");
    quantidadeReprovadosGeral = registerSignal("quantidadeReprovadosGeral");
    totalEvadidos = registerSignal("totalEvadidos");
    totalMatriculas = registerSignal("totalMatriculas");

    registerSignalArray();

    capacidadeTurma = 199;
    portaSaidaInicialReprovacao = par("capacidadeTurma");
    numeroPeriodos = par("numeroPeriodos");
    periodoAtual = par("numero");
    tempo = periodoAtual * 6.0;
    portaSaida = 0;
    contadorDeAlunosNaTurma = 0;

    reprovadosGeral = 0;
    evadidosGeral = 0;

    srand((int)time(NULL));
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

    adicionarNaTurma(aluno);

    if (turma.getLength() > 0) {
        Aluno *alunoDaTurma = check_and_cast<Aluno*>(turma.pop());
        contadorDeAlunosNaTurma++;
        int quantidadeSemestres = (int) (tempo.dbl() - alunoDaTurma->getEntrada()) / 6;
        avaliarAlunoPorEvasaoEreprovacao(alunoDaTurma);
    }
}


void Periodo::avaliarAlunoPorEvasaoEreprovacao(Aluno *aluno) {
    aluno->setNovato(false);
    int duracaoVinculo = (int) (tempo.dbl() - aluno->getEntrada()) / 6;

    if (aluno->getEntradaPeriodo(periodoAtual - 1) == 0) {
        aluno->setEntradaPeriodo(periodoAtual - 1, (int) tempo.dbl());
    }

//    if(reprovar(duracaoVinculo)){
//        reprovadosGeral++;
//        emit(reprovadosPorSemestre[duracaoVinculo - 1], 1);
//        aluno->setReprovacoes(periodoAtual - 1, aluno->getReprovacoes(periodoAtual - 1) + 1);
//
//        if(duracaoVinculo > 21 || evadir(duracaoVinculo)){
//            evadidosGeral++;
//            emit(evadidosPorSemestre[duracaoVinculo - 1], 1);
//            emit(totalEvadidos, 1);
//            cancelAndDelete(aluno);
//        } else {
//            send(aluno, "saida", portaSaidaInicialReprovacao + capacidadeTurma);
//        }
//
//    } else {
//        if (duracaoVinculo > 21 || evadir(duracaoVinculo)) {
//            evadidosGeral++;
//            emit(evadidosPorSemestre[duracaoVinculo - 1], 1);
//            emit(totalEvadidos, 1);
//            cancelAndDelete(aluno);
//        } else {
//            aluno->setSaidaPeriodo(periodoAtual - 1, (int) tempo.dbl());
//            emit(totalSemestre[duracaoVinculo - 1], 1);
//            int duracao = (int) ((aluno->getSaidaPeriodo(periodoAtual - 1) - aluno->getEntradaPeriodo(periodoAtual - 1)) / 6);
//            emit(duracaoTransicaoPeriodo[duracao], 1);
//            if (periodoAtual == numeroPeriodos) {
//                emit(graduadosPorSemestre[duracaoVinculo - 1], 1);
//            }
//            send(aluno, "saida", portaSaida);
//        }
//    }

    if (duracaoVinculo > 21 || evadir(duracaoVinculo)) {
        evadidosGeral++;
        emit(evadidosPorSemestre[duracaoVinculo - 1], 1);
        emit(totalEvadidos, 1);
        cancelAndDelete(aluno);
    } else {

        if (reprovar(duracaoVinculo)) {
            reprovadosGeral++;
            emit(reprovadosPorSemestre[duracaoVinculo - 1], 1);
            aluno->setReprovacoes(periodoAtual - 1, aluno->getReprovacoes(periodoAtual - 1) + 1);
            send(aluno, "saida", portaSaidaInicialReprovacao + capacidadeTurma);
        } else {
            aluno->setSaidaPeriodo(periodoAtual - 1, (int) tempo.dbl());
            emit(totalPorSemestre[duracaoVinculo - 1], 1);
            int duracao = (int) ((aluno->getSaidaPeriodo(periodoAtual - 1) - aluno->getEntradaPeriodo(periodoAtual - 1))/6);
            emit(duracaoTransicaoPeriodo[duracao], 1);
            if (periodoAtual == numeroPeriodos) {
                emit(graduadosPorSemestre[duracaoVinculo - 1], 1);
            }
            send(aluno, "saida", portaSaida);
        }
    }

}

void Periodo::adicionarNaTurma(Aluno *aluno) {
    turma.insert(aluno);
    emit(totalMatriculas, 1);
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
        return rNumber <= probEvasao;

    } else if (probsTipo == 1) {
        float probEvasao = (float) probsEvasaoDuracaoVinculo[curso][duracaoVinculo - 1];
        return rNumber <= probEvasao;

    } else {
        float probEvasao = (float) probsEvasaoPeriodoDuracaoVinculo[curso][periodoAtual - 1][duracaoVinculo - 1];
        return rNumber <= probEvasao;
    }

}

bool Periodo::reprovar(int duracaoVinculo) {

    float rNumber = randomValue();

    if (probsTipo == 0) {

        float probReprovacao = (float) probsReprovacaoPeriodo[curso][periodoAtual - 1];
        return rNumber <= probReprovacao;

    } else if (probsTipo == 1) {

        float probReprovacao = (float) probsReprovacaoDuracaoVinculo[curso][duracaoVinculo - 1];
        return rNumber <= probReprovacao;

    } else {

        float probReprovacao = (float) probsReprovacaoPeriodoDuracaoVinculo[curso][periodoAtual - 1][duracaoVinculo - 1];
        return rNumber <= probReprovacao;

    }
}

void Periodo::registerSignalArray() {

    int semestres = 21;
    int periodos = 10;

    //    INICIA VARIÁVEIS DE STATISTICS DE EVASÃO E REPROVAÇÃO POR PERÍODO
//    for (int pr = 0; pr < semestres; ++pr) {
//        for (int prd = 0; prd < periodos; ++prd) {
//            char signalNameDuracaoTransicaoPeriodo[32];
//            sprintf(signalNameDuracaoTransicaoPeriodo, "duracaoTransicaoPeriodo%d_%d", pr, prd);
//            simsignal_t signalDuracaoTransicaoPeriodo = registerSignal(signalNameDuracaoTransicaoPeriodo);
//            cProperty *statisticTemplateDuracaoTransicaoPeriodo = getProperties()->get("statisticTemplate", "duracaoTransicaoPeriodoTemplate");
//            getEnvir()->addResultRecorders(this, signalDuracaoTransicaoPeriodo, signalNameDuracaoTransicaoPeriodo, statisticTemplateDuracaoTransicaoPeriodo);
//            duracaoTransicaoPeriodo[pr][prd] = signalDuracaoTransicaoPeriodo;
//        }
//    }

//    INICIA VARIÁVEIS DE STATISTICS DE EVASÃO E RETENÇÃO POR PERÍODO
//    for (int p = 0; p < periodos; ++p) {
//
//        char signalNameEvadidosPeriodo[32];
//        char signalNameReprovadosPeriodo[32];
//
//        sprintf(signalNameEvadidosPeriodo, "evadidosPorPeriodo%d", p);
//        sprintf(signalNameReprovadosPeriodo, "reprovadosPorPeriodo%d", p);
//
//        simsignal_t signalEvadidosPeriodo = registerSignal(signalNameEvadidosPeriodo);
//        simsignal_t signalReprovadosPeriodo = registerSignal(signalNameReprovadosPeriodo);
//
//        cProperty *statisticTemplateEvadidosPeriodo = getProperties()->get("statisticTemplate", "reprovadosPorPeriodoTemplate");
//        cProperty *statisticTemplateReprovadosPeriodo = getProperties()->get("statisticTemplate", "reprovadosPorPeriodoTemplate");
//
//        getEnvir()->addResultRecorders(this, signalEvadidosPeriodo, signalNameEvadidosPeriodo, statisticTemplateEvadidosPeriodo);
//        getEnvir()->addResultRecorders(this, signalReprovadosPeriodo, signalNameReprovadosPeriodo, statisticTemplateReprovadosPeriodo);
//
//        evadidosPorPeriodo[p] = signalEvadidosPeriodo;
//        reprovadosPorPeriodo[p] = signalReprovadosPeriodo;
//    }


    //    INICIA VARIÁVEIS DE STATISTICS DE EVASÃO, GRADUAÇÃO E RETENÇÃO POR SEMESTRE
    for (int s = 0; s < semestres; ++s) {

        char signalNameTotalSemestre[32];
        char signalNameEvadidosSemestre[32];
        char signalNameReprovadosSemestre[32];
        char signalNameGraduadosSemestre[32];
        char signalNameDuracaoTransicaoPeriodo[32];

        sprintf(signalNameTotalSemestre, "totalPorSemestre%d", s);
        sprintf(signalNameEvadidosSemestre, "evadidosPorSemestre%d", s);
        sprintf(signalNameReprovadosSemestre, "reprovadosPorSemestre%d", s);
        sprintf(signalNameGraduadosSemestre, "graduadosPorSemestre%d", s);
        sprintf(signalNameDuracaoTransicaoPeriodo, "duracaoTransicaoPeriodo%d", s);

        simsignal_t signalTotalSemestre = registerSignal(signalNameTotalSemestre);
        simsignal_t signalEvadidosSemestre = registerSignal(signalNameEvadidosSemestre);
        simsignal_t signalReprovadosSemestre = registerSignal(signalNameReprovadosSemestre);
        simsignal_t signalGraduadosSemestre = registerSignal(signalNameGraduadosSemestre);
        simsignal_t signalDuracaoTransicaoPeriodo = registerSignal(signalNameDuracaoTransicaoPeriodo);

        cProperty *statisticTemplateTotalSemestre = getProperties()->get("statisticTemplate", "totalPorSemestreTemplate");
        cProperty *statisticTemplateEvadidosSemestre = getProperties()->get("statisticTemplate", "evadidosPorSemestreTemplate");
        cProperty *statisticTemplateReprovadosSemestre = getProperties()->get("statisticTemplate", "reprovadosPorSemestreTemplate");
        cProperty *statisticTemplateGraduadosSemestre = getProperties()->get("statisticTemplate", "graduadosPorSemestreTemplate");
        cProperty *statisticTemplateDuracaoTransicaoPeriodo = getProperties()->get("statisticTemplate", "duracaoTransicaoPeriodoTemplate");

        getEnvir()->addResultRecorders(this, signalTotalSemestre, signalNameTotalSemestre, statisticTemplateTotalSemestre);
        getEnvir()->addResultRecorders(this, signalEvadidosSemestre, signalNameEvadidosSemestre, statisticTemplateEvadidosSemestre);
        getEnvir()->addResultRecorders(this, signalReprovadosSemestre, signalNameReprovadosSemestre, statisticTemplateReprovadosSemestre);
        getEnvir()->addResultRecorders(this, signalGraduadosSemestre, signalNameGraduadosSemestre, statisticTemplateGraduadosSemestre);
        getEnvir()->addResultRecorders(this, signalDuracaoTransicaoPeriodo, signalNameDuracaoTransicaoPeriodo, statisticTemplateDuracaoTransicaoPeriodo);

        totalPorSemestre[s] = signalTotalSemestre;
        evadidosPorSemestre[s] = signalEvadidosSemestre;
        reprovadosPorSemestre[s] = signalReprovadosSemestre;
        graduadosPorSemestre[s] = signalGraduadosSemestre;
        duracaoTransicaoPeriodo[s] = signalDuracaoTransicaoPeriodo;
   }

//
//
//    //    INICIA VARIÁVEIS DE STATISTICS DE EVASÃO E RETENÇÃO POR PERÍODO E SEMESTRE, E TOTAL DE ALUNOS POR SEMESTRE E PERIODO
//   for (int sem = 0; sem < semestres; ++sem) {
//
//        for (int pe = 0; pe < periodos; ++pe) {
//
//            char signalNameEvadidosSemestrePeriodo[32];
//            char signalNameReprovadosSemestrePeriodo[32];
//            char signalNameTotalSemestrePeriodo[32];
//
//            sprintf(signalNameEvadidosSemestrePeriodo, "evadidosSemestrePeriodo%d_%d", sem, pe);
//            sprintf(signalNameReprovadosSemestrePeriodo, "reprovadosSemestrePeriodo%d_%d", sem, pe);
//            sprintf(signalNameTotalSemestrePeriodo, "totalSemestrePeriodo%d_%d", sem, pe);
//
//            simsignal_t signalEvadidosSemestrePeriodo = registerSignal(signalNameEvadidosSemestrePeriodo);
//            simsignal_t signalReprovadosSemestrePeriodo = registerSignal(signalNameReprovadosSemestrePeriodo);
//            simsignal_t signalTotalSemestrePeriodo = registerSignal(signalNameTotalSemestrePeriodo);
//
//            cProperty *statisticTemplateEvadidosSemestrePeriodo = getProperties()->get("statisticTemplate", "evadidosSemestrePeriodoTemplate");
//            cProperty *statisticTemplateReprovadosSemestrePeriodo = getProperties()->get("statisticTemplate", "reprovadosSemestrePeriodoTemplate");
//            cProperty *statisticTemplateTotalSemestrePeriodo = getProperties()->get("statisticTemplate", "totalSemestrePeriodoTemplate");
//
//            getEnvir()->addResultRecorders(this, signalEvadidosSemestrePeriodo, signalNameEvadidosSemestrePeriodo, statisticTemplateEvadidosSemestrePeriodo);
//            getEnvir()->addResultRecorders(this, signalReprovadosSemestrePeriodo, signalNameReprovadosSemestrePeriodo, statisticTemplateReprovadosSemestrePeriodo);
//            getEnvir()->addResultRecorders(this, signalTotalSemestrePeriodo, signalNameTotalSemestrePeriodo, statisticTemplateTotalSemestrePeriodo);
//
//            evadidosSemestrePeriodo[sem][pe] = signalEvadidosSemestrePeriodo;
//            reprovadosSemestrePeriodo[sem][pe] = signalReprovadosSemestrePeriodo;
//            totalSemestrePeriodo[sem][pe] = signalTotalSemestrePeriodo;
//
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
    emit(quantidadeReprovadosGeral, reprovadosGeral);
}
