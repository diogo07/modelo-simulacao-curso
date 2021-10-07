#ifndef __CURSO_PERIODO_H
#define __CURSO_PERIODO_H

#include <omnetpp.h>
#include "Aluno.h"

using namespace omnetpp;

class Periodo : public cSimpleModule {
    private:


//      CONF DA TURMA
        simsignal_t tamanhoFilaEspera;
        simsignal_t tamanhoTurma;

//      INFO DO PERIODO
        simsignal_t totalMatriculas;
        simsignal_t evadidosPorPeriodo[10];
        simsignal_t retidosPorPeriodo[10];

        simsignal_t evadidosPorSemestre[21];
        simsignal_t retidosPorSemestre[21];
        simsignal_t graduadosPorSemestre[21];

        simsignal_t evadidosSemestrePeriodo[21][10];
        simsignal_t retidosSemestrePeriodo[21][10];
        simsignal_t totalSemestrePeriodo[21][10];
        simsignal_t duracaoTransicaoPeriodo[21][10];


        simsignal_t totalEvadidos;
        simsignal_t quantidadeEvadidosGeral;
        simsignal_t quantidadeRetidosGeral;

        virtual bool compare(Aluno * aluno1, Aluno * aluno2);
        virtual void adicionarNaTurma(Aluno *aluno);
        virtual void avaliarAluno(Aluno *aluno);
        virtual void avaliarAlunoPorEvasaoEretencao(Aluno *aluno);
        virtual bool evadir(int semestre);
        virtual bool reter(int semestre);
        virtual void emitirDadosDoPeriodo();
        virtual void processarAluno(Aluno *aluno);
        virtual void registerSignalArray();
        virtual float randomValue();

    public:
        virtual ~Periodo() override;

    protected:

//      ATRIBUTOS DE CONFIGURACAO
        SimTime tempo;
        cQueue turma;
        cQueue filaEspera;
        int capacidadeTurma;
        int periodoAtual; // O INDICE EQUIVALE AO PERIODO, EX: INDICE 1 = 1 PERIODO
        int numeroPeriodos; // INDICA A QUANTIDADE DE PERIODOS QUE O FORMATO DE CURSO POSSUI
        int portaSaida;
        int portaSaidaInicialRetencao;
        int analiseCurso = 2; // 0 - GERAL, 1 - BSI, 2 - BCC
        int analiseTipo = 2; // 0 - POR PERIODO, 1 - POR SEMESTRE, 2 - SEMESTRE E PERIODO

//      ESPECIFICOS
        int contadorDeAlunosNaTurma;

//      GERAL
        int retidosGeral;
        int evadidosGeral;


//      TESTES
        bool evadido = false;

/**
 *
 * METRICAS DE EVAS�O E RETEN��O USADOS NO MODELO
 *
 */


//      UFRPE


        //      POR PERIODO
        double probDeEvasaoPeriodo [3][10] = {
               {0.3327, 0.1305, 0.0932, 0.0537, 0.0375, 0.043, 0.0347, 0.0191, 0.014, 0.0086},
               {0.3767, 0.1493, 0.0828, 0.0654, 0.0385, 0.0421, 0.0382, 0.0109, 0.0045, 0.0196},
               {0.3636, 0.1747, 0.072, 0.0287, 0.0194, 0.0283, 0.0376, 0.0075, 0.0056, 0.0}
        };


        double probDeRetencaoPeriodo [3][10] = {
                {0.9367, 0.6425, 0.6671, 0.601, 0.6188, 0.6199, 0.6358, 0.6485, 0.7256, 0.7806},
                {0.8602, 0.7289, 0.5495, 0.5639, 0.521, 0.5561, 0.5534, 0.5628, 0.7602, 0.6569},
                {0.9091, 0.7823, 0.5387, 0.4467, 0.5465, 0.5283, 0.6573, 0.6165, 0.7095, 0.7679}
        };


//      POR SEMESTRE
        double probDeEvasaoSemestre [3][21] = {
                {0.1309, 0.127, 0.0869, 0.0987, 0.0727, 0.0691, 0.0462, 0.0349, 0.0368, 0.0353, 0.0286, 0.0386, 0.0629, 0.0276, 0.0833, 0.069, 0.1207, 0.0789, 0.1304, 0.2, 0.0},
                {0.1388, 0.1542, 0.1409, 0.1543, 0.0916, 0.1176, 0.0714, 0.0359, 0.1087, 0.0629, 0.0569, 0.0538, 0.0933, 0.0185, 0.0465, 0.0789, 0.069, 0.0952, 0.0769, 0.125, 0.0},
                {0.1481, 0.1481, 0.0936, 0.1587, 0.1228, 0.1, 0.0778, 0.0488, 0.0338, 0.0385, 0.0094, 0.0602, 0.0536, 0.0238, 0.1304, 0.0833, 0.1667, 0.0, 0.3333, 0.0, 0.0}
        };

        double probDeRetencaoSemestre [3][21] = {
                {0.3185, 0.4443, 0.4584, 0.4701, 0.466, 0.4574, 0.4626, 0.4676, 0.5028, 0.5838, 0.7492, 0.812, 0.8601, 0.9392, 0.9917, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
                {0.3642, 0.6028, 0.6436, 0.6334, 0.6336, 0.6345, 0.6381, 0.6513, 0.7446, 0.8531, 0.8943, 0.957, 0.9867, 0.9815, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
                {0.4345, 0.5983, 0.6388, 0.6827, 0.6798, 0.665, 0.6611, 0.6829, 0.75, 0.8385, 0.9151, 0.9759, 0.9821, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0}
        };


//        POR PERIODO E SEMESTRE
        double probDeEvasaoPeriodoSemestre[3][10][22] = {
                {{0.2608, 0.3669, 0.3657, 0.45, 0.4103, 0.5, 0.5556, 0.25, 0.3333, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0321, 0.0888, 0.1341, 0.3246, 0.2836, 0.3953, 0.3478, 0.4286, 0.5, 0.0, 0.0, 0.5, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0163, 0.0364, 0.1, 0.141, 0.2545, 0.2571, 0.1579, 0.2857, 0.1111, 0.1429, 0.5, 0.3333, 0.5, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0114, 0.0429, 0.0581, 0.0755, 0.093, 0.0625, 0.25, 0.0833, 0.1, 0.0, 0.25, 0.0, 0.0, 0.5, 1.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.01, 0.0098, 0.0238, 0.0185, 0.0, 0.1333, 0.1429, 0.1, 0.5, 0.25, 0.5, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0286, 0.0426, 0.0638, 0.1333, 0.0417, 0.0625, 0.375, 0.0, 0.3333, 0.5, 1.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0156, 0.0, 0.0, 0.025, 0.0312, 0.08, 0.1765, 0.2, 0.0, 0.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0159, 0.0351, 0.0244, 0.0345, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.3333, 0.5, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0175, 0.0143, 0.0169, 0.0, 0.0513, 0.0, 0.0, 0.0909, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.037, 0.05, 0.0769, 0.125, 0.0}},
                {{0.3052, 0.3986, 0.4375, 0.4524, 0.4762, 0.5, 0.4, 0.3333, 0.5, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0247, 0.0952, 0.1867, 0.3091, 0.1765, 0.4444, 0.2143, 0.1818, 0.375, 0.2, 0.0, 0.5, 0.0, 0.5, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0084, 0.0263, 0.2245, 0.0811, 0.1935, 0.3333, 0.0833, 0.5, 0.2, 0.25, 0.0, 0.6667, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0189, 0.1613, 0.1304, 0.05, 0.0, 0.2667, 0.25, 0.4, 0.3333, 0.0, 0.0, 0.0, 0.5, 1.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0323, 0.0909, 0.1, 0.2667, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0286, 0.0, 0.0, 0.0, 0.1429, 0.2, 0.25, 0.3333, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0286, 0.1429, 0.1111, 0.1333, 0.0, 0.125, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0909, 0.0, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0417, 0.0, 0.0, 0.2, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.3333, 0.2, 0.0, 0.0}},
                {{0.3068, 0.3981, 0.2698, 0.5116, 0.45, 0.5455, 0.8, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0614, 0.1071, 0.1077, 0.3396, 0.3636, 0.2632, 0.3846, 0.5, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0536, 0.0244, 0.1724, 0.4091, 0.2727, 0.375, 0.3333, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.02, 0.025, 0.069, 0.0, 0.1538, 0.25, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0204, 0.0, 0.0, 0.0, 0.0, 0.0, 0.1429, 0.0, 0.25, 0.5, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.2727, 0.25, 0.0, 0.0, 0.5, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0833, 0.0, 0.0, 0.2222, 0.3333, 0.0, 0.5, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}}

        };


        double probDeRetencaoPeriodoSemestre [3][10][22] = {
                {{0.551, 0.5403, 0.597, 0.4875, 0.5128, 0.45, 0.4444, 0.75, 0.6667, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.3355, 0.6023, 0.689, 0.5877, 0.6418, 0.5349, 0.6087, 0.5714, 0.5, 0.75, 0.6667, 0.5, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.4106, 0.6364, 0.7091, 0.7051, 0.6364, 0.5429, 0.7368, 0.6429, 0.7778, 0.8571, 0.5, 0.6667, 0.5, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.4615, 0.3829, 0.5357, 0.5581, 0.7547, 0.7442, 0.625, 0.6, 0.8333, 0.6, 0.6667, 0.5, 1.0, 1.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.3478, 0.46, 0.6765, 0.5238, 0.7222, 0.6444, 0.7, 0.4762, 0.4, 1.0, 0.5, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.2368, 0.4935, 0.6933, 0.5, 0.8085, 0.5532, 0.8, 0.6667, 0.5, 0.5, 0.75, 0.6667, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.2927, 0.5, 0.7925, 0.6346, 0.65, 0.7188, 0.68, 0.5882, 0.5, 1.0, 0.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.4091, 0.5672, 0.6984, 0.6316, 0.6341, 0.8966, 0.6429, 0.3333, 0.3333, 1.5, 1.3333, 0.75, 0.6667, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.4706, 0.6, 0.8596, 0.7286, 0.7797, 0.7872, 1.0, 0.4872, 0.5789, 0.1818, 1.0, 0.5, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.5758, 0.625, 0.9423, 0.7258, 0.86, 0.7727, 0.9143, 0.9062, 0.931, 0.7407, 0.65, 0.6154, 0.25, 0.0}},
                {{0.662, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.2469, 0.8571, 0.9867, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.2521, 0.8553, 0.9796, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.1944, 0.7925, 0.9032, 0.9565, 0.95, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.1538, 0.7115, 0.8387, 0.9545, 0.95, 0.9333, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.2093, 0.6857, 0.85, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.22, 0.6226, 0.7714, 0.9048, 0.8889, 0.8667, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.1333, 0.5938, 0.7143, 1.0, 1.0, 0.875, 0.9091, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.1042, 0.6735, 0.7091, 0.8936, 0.9787, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0476, 0.7273, 0.6667, 0.6667, 1.0, 0.875, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0}},
                {{0.7841, 0.9907, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.3596, 0.8571, 0.9846, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.4026, 0.9107, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.26, 0.775, 0.8621, 0.8, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.3265, 0.7907, 0.8205, 0.8095, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0222, 0.325, 0.6176, 0.6923, 0.7917, 0.8824, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0303, 0.4688, 0.7273, 0.7308, 0.8333, 0.9444, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.04, 0.3889, 0.7568, 0.6667, 0.9167, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.1364, 0.4762, 0.75, 0.7419, 0.913, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0909, 0.5143, 0.75, 0.8409, 0.9535, 0.9677, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0}}
        };


        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;
        virtual void finish() override;
};


#endif
