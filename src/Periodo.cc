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

float Periodo::randomValue(){
    return ((float)(std::rand() % 1000)) / 1000;
}

bool Periodo::evadir(int semestre) {

    float rNumber = randomValue();

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

    float rNumber = randomValue();

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


    //    INICIA VARIÁVEIS DE STATISTICS DE EVASÃO E RETENÇÃO POR SEMESTRE
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



    //    INICIA VARIÁVEIS DE STATISTICS DE EVASÃO E RETENÇÃO POR PERÍODO E SEMESTRE
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
