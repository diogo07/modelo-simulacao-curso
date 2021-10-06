#include "Periodo.h"
#include <string.h>
#include <omnetpp.h>
#include <math.h>
#include <random>
#include <time.h>

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
    totalMatriculas = registerSignal("totalMatriculas");

    registerSignalArray();

    capacidadeTurma = 199;
    portaSaidaInicialRetencao = par("capacidadeTurma");
    numeroPeriodos = par("numeroPeriodos");
    periodoAtual = par("numero");
    tempo = periodoAtual * 6.0;
    portaSaida = 0;
    contadorDeAlunosNaTurma = 0;

    retidosGeral = 0;
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

    aluno->setNovato(false);
    adicionarNaTurma(aluno);

    if (turma.getLength() > 0) {

        Aluno *alunoDaTurma = check_and_cast<Aluno*>(turma.pop());

        contadorDeAlunosNaTurma++;
        int quantidadeSemestres = (int) (tempo.dbl()
                - alunoDaTurma->getEntrada()) / 6;
//        bool possibilidadeDeGraduar =
//                ((alunoDaTurma->getQuantidadeSemestresCursados(tempo.dbl())
//                        - periodoAtual + 10) <= 21);
//        if (possibilidadeDeGraduar) {
            avaliarAlunoPorEvasaoEretencao(alunoDaTurma);
//        } else {
//            evadidosGeral++;
//            emit(evadidosPorPeriodo[periodoAtual - 1], 1);
//            emit(evadidosPorSemestre[quantidadeSemestres - 1], 1);
//            emit(evadidosSemestrePeriodo[quantidadeSemestres - 1][periodoAtual - 1], 1);
//            emit(totalEvadidos, 1);
//            cancelAndDelete(alunoDaTurma);
//        }
    }
}

void Periodo::avaliarAluno(Aluno *aluno) {
    aluno->setNovato(false);
    int quantidadeSemestres = (int) (tempo.dbl() - aluno->getEntrada()) / 6;
    if(aluno->getEntradaPeriodo(periodoAtual - 1) == 0.0){
        aluno->setEntradaPeriodo(periodoAtual - 1,  (int) tempo.dbl());
    }
    if (reter(quantidadeSemestres)) {
        retidosGeral++;
        emit(retidosPorPeriodo[periodoAtual - 1], 1);
        emit(retidosPorSemestre[quantidadeSemestres - 1], 1);
        emit(retidosSemestrePeriodo[quantidadeSemestres - 1][periodoAtual - 1], 1);
        aluno->setReprovacoes(periodoAtual - 1, aluno->getReprovacoes(periodoAtual - 1) + 1);
//        if (aluno->getReprovacoes(periodoAtual - 1) >= 4) {
//            evadidos++;
//            emit(totalEvadidos, 1);
//            emit(evadidosPorSemestre[quantidadeSemestres - 1], 1);
//            emit(evadidosSemestrePeriodo[quantidadeSemestres - 1][periodoAtual - 1], 1);
//        } else {
//        send(aluno, "saida", portaSaidaInicialRetencao + capacidadeTurma);
//        }

        if (evadir(quantidadeSemestres)) {
            evadidosGeral++;
            emit(evadidosPorPeriodo[periodoAtual - 1], 1);
            emit(evadidosPorSemestre[quantidadeSemestres - 1], 1);
            emit(evadidosSemestrePeriodo[quantidadeSemestres - 1][periodoAtual - 1], 1);
            emit(totalEvadidos, 1);
            cancelAndDelete(aluno);
        } else {
            send(aluno, "saida", portaSaidaInicialRetencao + capacidadeTurma);
        }
    } else {

        if (evadir(quantidadeSemestres)) {
            evadidosGeral++;
            emit(evadidosPorPeriodo[periodoAtual - 1], 1);
            emit(evadidosPorSemestre[quantidadeSemestres - 1], 1);
            emit(evadidosSemestrePeriodo[quantidadeSemestres - 1][periodoAtual - 1],
                    1);
            emit(totalEvadidos, 1);
            cancelAndDelete(aluno);
        } else {
            aluno->setSaidaPeriodo(periodoAtual - 1, (int) tempo.dbl());
            int duracao = (int) (aluno->getSaidaPeriodo(periodoAtual - 1) - aluno->getEntradaPeriodo(periodoAtual - 1)/6);
            emit(totalSemestrePeriodo[quantidadeSemestres - 1][periodoAtual - 1], 1);
            emit(duracaoTransicaoPeriodo[duracao][periodoAtual - 1], 1);
            if(periodoAtual == numeroPeriodos) {
                emit(graduadosPorSemestre[quantidadeSemestres - 1], 1);
            }
            send(aluno, "saida", portaSaida);
        }

    }
}

void Periodo::avaliarAlunoPorEvasaoEretencao(Aluno *aluno) {
    aluno->setNovato(false);
    int duracaoVinculo = (int) (tempo.dbl() - aluno->getEntrada()) / 6;
    duracaoVinculo = duracaoVinculo > 21 ? 21 : duracaoVinculo;

    if (aluno->getEntradaPeriodo(periodoAtual - 1) == 0) {
        aluno->setEntradaPeriodo(periodoAtual - 1, (int) tempo.dbl());
    }

    if (evadir(duracaoVinculo)) {
        evadidosGeral++;
        emit(evadidosPorPeriodo[periodoAtual - 1], 1);
        emit(evadidosPorSemestre[duracaoVinculo - 1], 1);
        emit(evadidosSemestrePeriodo[duracaoVinculo - 1][periodoAtual - 1], 1);
        emit(totalEvadidos, 1);
        cancelAndDelete(aluno);
    } else {

        if (reter(duracaoVinculo)) {
            retidosGeral++;
            emit(retidosPorPeriodo[periodoAtual - 1], 1);
            emit(retidosPorSemestre[duracaoVinculo - 1], 1);
            emit(retidosSemestrePeriodo[duracaoVinculo - 1][periodoAtual - 1], 1);
            aluno->setReprovacoes(periodoAtual - 1, aluno->getReprovacoes(periodoAtual - 1) + 1);
            send(aluno, "saida", portaSaidaInicialRetencao + capacidadeTurma);
        } else {
            aluno->setSaidaPeriodo(periodoAtual - 1, (int) tempo.dbl());
            emit(totalSemestrePeriodo[duracaoVinculo - 1][periodoAtual - 1], 1);
            int duracao = (int) ((aluno->getSaidaPeriodo(periodoAtual - 1) - aluno->getEntradaPeriodo(periodoAtual - 1))/6);
            duracao = duracao > 20 ? 20 : duracao;

            emit(duracaoTransicaoPeriodo[duracao][periodoAtual - 1], 1);
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

    if (analiseTipo == 0) {
        float probEvasao = (float) probDeEvasaoPeriodo[analiseCurso][periodoAtual - 1];
        return rNumber <= probEvasao;

    } else if (analiseTipo == 1) {
        float probEvasao = (float) probDeEvasaoSemestre[analiseCurso][duracaoVinculo - 1];
        return rNumber <= probEvasao;

    } else {
        float probEvasao = (float) probDeEvasaoPeriodoSemestre[analiseCurso][periodoAtual - 1][duracaoVinculo - 1];
        return rNumber <= probEvasao;
    }

}

bool Periodo::reter(int duracaoVinculo) {

    float rNumber = randomValue();

    if (analiseTipo == 0) {

        float probRetencao = (float) probDeRetencaoPeriodo[analiseCurso][periodoAtual - 1];
        return rNumber <= probRetencao;

    } else if (analiseTipo == 1) {

        float probRetencao = (float) probDeRetencaoSemestre[analiseCurso][duracaoVinculo - 1];
        return rNumber <= probRetencao;

    } else {

        float probRetencao = (float) probDeRetencaoPeriodoSemestre[analiseCurso][periodoAtual - 1][duracaoVinculo - 1];
        return rNumber <= probRetencao;

    }
}

void Periodo::registerSignalArray() {

    int semestres = 21;
    int periodos = 10;

    //    INICIA VARIÁVEIS DE STATISTICS DE EVASÃO E RETENÇÃO POR PERÍODO
    for (int pr = 0; pr < semestres; ++pr) {
        for (int prd = 0; prd < periodos; ++prd) {
            char signalNameDuracaoTransicaoPeriodo[32];

            sprintf(signalNameDuracaoTransicaoPeriodo, "duracaoTransicaoPeriodo%d_%d", pr, prd);

            simsignal_t signalDuracaoTransicaoPeriodo = registerSignal(
                    signalNameDuracaoTransicaoPeriodo);

            cProperty *statisticTemplateDuracaoTransicaoPeriodo = getProperties()->get(
                    "statisticTemplate", "duracaoTransicaoPeriodoTemplate");

            getEnvir()->addResultRecorders(this, signalDuracaoTransicaoPeriodo,
                    signalNameDuracaoTransicaoPeriodo,
                    statisticTemplateDuracaoTransicaoPeriodo);

            duracaoTransicaoPeriodo[pr][prd] = signalDuracaoTransicaoPeriodo;
        }
    }

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


    //    INICIA VARIÁVEIS DE STATISTICS DE EVASÃO, GRADUAÇÃO E RETENÇÃO POR SEMESTRE
    for (int s = 0; s < semestres; ++s) {
        char signalNameEvadidosSemestre[32];
        char signalNameRetidosSemestre[32];
        char signalNameGraduadosSemestre[32];

        sprintf(signalNameEvadidosSemestre, "evadidosPorSemestre%d", s);
        sprintf(signalNameRetidosSemestre, "retidosPorSemestre%d", s);
        sprintf(signalNameGraduadosSemestre, "graduadosPorSemestre%d", s);

        simsignal_t signalEvadidosSemestre = registerSignal(signalNameEvadidosSemestre);
        simsignal_t signalRetidosSemestre = registerSignal(signalNameRetidosSemestre);
        simsignal_t signalGraduadosSemestre = registerSignal(signalNameGraduadosSemestre);

        cProperty *statisticTemplateEvadidosSemestre = getProperties()->get("statisticTemplate", "evadidosPorSemestreTemplate");
        cProperty *statisticTemplateRetidosSemestre = getProperties()->get("statisticTemplate", "retidosPorSemestreTemplate");
        cProperty *statisticTemplateGraduadosSemestre = getProperties()->get("statisticTemplate", "graduadosPorSemestreTemplate");

        getEnvir()->addResultRecorders(this, signalEvadidosSemestre, signalNameEvadidosSemestre, statisticTemplateEvadidosSemestre);
        getEnvir()->addResultRecorders(this, signalRetidosSemestre, signalNameRetidosSemestre, statisticTemplateRetidosSemestre);
        getEnvir()->addResultRecorders(this, signalGraduadosSemestre, signalNameGraduadosSemestre, statisticTemplateGraduadosSemestre);

        evadidosPorSemestre[s] = signalEvadidosSemestre;
        retidosPorSemestre[s] = signalRetidosSemestre;
        graduadosPorSemestre[s] = signalGraduadosSemestre;
   }



    //    INICIA VARIÁVEIS DE STATISTICS DE EVASÃO E RETENÇÃO POR PERÍODO E SEMESTRE, E TOTAL DE ALUNOS POR SEMESTRE E PERIODO
   for (int sem = 0; sem < semestres; ++sem) {

        for (int pe = 0; pe < periodos; ++pe) {

            char signalNameEvadidosSemestrePeriodo[32];
            char signalNameRetidosSemestrePeriodo[32];
            char signalNameTotalSemestrePeriodo[32];

            sprintf(signalNameEvadidosSemestrePeriodo, "evadidosSemestrePeriodo%d_%d", sem, pe);
            sprintf(signalNameRetidosSemestrePeriodo, "retidosSemestrePeriodo%d_%d", sem, pe);
            sprintf(signalNameTotalSemestrePeriodo, "totalSemestrePeriodo%d_%d", sem, pe);

            simsignal_t signalEvadidosSemestrePeriodo = registerSignal(signalNameEvadidosSemestrePeriodo);
            simsignal_t signalRetidosSemestrePeriodo = registerSignal(signalNameRetidosSemestrePeriodo);
            simsignal_t signalTotalSemestrePeriodo = registerSignal(signalNameTotalSemestrePeriodo);

            cProperty *statisticTemplateEvadidosSemestrePeriodo = getProperties()->get("statisticTemplate", "evadidosSemestrePeriodoTemplate");
            cProperty *statisticTemplateRetidosSemestrePeriodo = getProperties()->get("statisticTemplate", "retidosSemestrePeriodoTemplate");
            cProperty *statisticTemplateTotalSemestrePeriodo = getProperties()->get("statisticTemplate", "totalSemestrePeriodoTemplate");

            getEnvir()->addResultRecorders(this, signalEvadidosSemestrePeriodo, signalNameEvadidosSemestrePeriodo, statisticTemplateEvadidosSemestrePeriodo);
            getEnvir()->addResultRecorders(this, signalRetidosSemestrePeriodo, signalNameRetidosSemestrePeriodo, statisticTemplateRetidosSemestrePeriodo);
            getEnvir()->addResultRecorders(this, signalTotalSemestrePeriodo, signalNameTotalSemestrePeriodo, statisticTemplateTotalSemestrePeriodo);

            evadidosSemestrePeriodo[sem][pe] = signalEvadidosSemestrePeriodo;
            retidosSemestrePeriodo[sem][pe] = signalRetidosSemestrePeriodo;
            totalSemestrePeriodo[sem][pe] = signalTotalSemestrePeriodo;

        }
    }

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
