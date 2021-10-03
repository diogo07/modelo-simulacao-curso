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

//      ATRIBUTOS DE CONFIGURA��O
        SimTime tempo;
        cQueue turma;
        cQueue filaEspera;
        int capacidadeTurma;
        int periodoAtual; // O �NDICE EQUIVALE AO PER�ODO, EX: INDICE 1 = 1� PERIODO
        int numeroPeriodos; // INDICA A QUANTIDADE DE PER�ODOS QUE O FORMATO DE CURSO POSSUI
        int portaSaida;
        int portaSaidaInicialRetencao;
        int analiseCurso = 1; // 0 - GERAL, 1 - BSI, 2 - BCC
        int analiseTipo = 1; // 0 - POR PERIODO, 1 - POR SEMESTRE, 2 - SEMESTRE E PERIODO

//      ESPEC�FICOS
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
               {0.3372, 0.1258, 0.108, 0.0745, 0.0644, 0.0421, 0.044, 0.0283, 0.0345, 0.0086},
               {0.3275, 0.1359, 0.0773, 0.0264, 0.018, 0.0439, 0.0244, 0.0065, 0.0, 0.0}
        };


        double probDeRetencaoPeriodo [3][10] = {
                {0.9367, 0.6425, 0.6671, 0.601, 0.6188, 0.6199, 0.6358, 0.6485, 0.7256, 0.7806},
                {0.9598, 0.6403, 0.7326, 0.6619, 0.6436, 0.6168, 0.6154, 0.6368, 0.6379, 0.7806},
                {0.9105, 0.6449, 0.5967, 0.5208, 0.6007, 0.6228, 0.6585, 0.6645, 0.7852, 0.0}
        };


//      POR SEMESTRE
        double probDeEvasaoSemestre [3][21] = {
                {0.143, 0.1515, 0.1195, 0.1564, 0.1061, 0.1096, 0.0744, 0.0418, 0.0753, 0.0513, 0.0349, 0.0568, 0.0763, 0.0208, 0.0758, 0.08, 0.0857, 0.08, 0.125, 0.125, 0.0},
                {0.1388, 0.1542, 0.1409, 0.1543, 0.0916, 0.1176, 0.0714, 0.0359, 0.1087, 0.0629, 0.0569, 0.0538, 0.0933, 0.0185, 0.0465, 0.0789, 0.069, 0.0952, 0.0769, 0.125, 0.0},
                {0.1481, 0.1481, 0.0936, 0.1587, 0.1228, 0.1, 0.0778, 0.0488, 0.0338, 0.0385, 0.0094, 0.0602, 0.0536, 0.0238, 0.1304, 0.0833, 0.1667, 0.0, 0.3333, 0.0, 0.0}
        };

        double probDeRetencaoSemestre [3][21] = {
                {0.2893, 0.5173, 0.6097, 0.6409, 0.6408, 0.6507, 0.6513, 0.6657, 0.7169, 0.8059, 0.8865, 0.9545, 0.9771, 0.9896, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
                {0.3421, 0.5397, 0.6464, 0.6592, 0.6489, 0.6639, 0.6714, 0.6872, 0.7283, 0.8252, 0.8862, 0.9355, 0.9733, 0.9815, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
                {0.2257, 0.49, 0.5652, 0.6199, 0.6316, 0.635, 0.6278, 0.6402, 0.7027, 0.7846, 0.8868, 0.9759, 0.9821, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0}
        };


//        POR PERIODO E SEMESTRE
        double probDeEvasaoPeriodoSemestre[3][10][22] = {
                {{0.2608, 0.3669, 0.3657, 0.45, 0.4103, 0.5, 0.5556, 0.25, 0.3333, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0321, 0.0888, 0.1341, 0.3246, 0.2836, 0.3953, 0.3478, 0.4286, 0.5, 0.0, 0.0, 0.5, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0163, 0.0364, 0.1, 0.141, 0.2545, 0.2571, 0.1579, 0.2857, 0.1111, 0.1429, 0.5, 0.3333, 0.5, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0114, 0.0429, 0.0581, 0.0755, 0.093, 0.0625, 0.25, 0.0833, 0.1, 0.0, 0.25, 0.0, 0.0, 0.5, 1.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.01, 0.0098, 0.0238, 0.0185, 0.0, 0.1333, 0.1429, 0.1, 0.5, 0.25, 0.5, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0286, 0.0426, 0.0638, 0.1333, 0.0417, 0.0625, 0.375, 0.0, 0.3333, 0.5, 1.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0156, 0.0, 0.0, 0.025, 0.0312, 0.08, 0.1765, 0.2, 0.0, 0.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0159, 0.0351, 0.0244, 0.0345, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.3333, 0.5, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0175, 0.0143, 0.0169, 0.0, 0.0513, 0.0, 0.0, 0.0909, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.037, 0.05, 0.0769, 0.125, 0.0}},
                {{0.2563, 0.3817, 0.4384, 0.4211, 0.35, 0.4545, 0.4, 0.3333, 0.5, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0309, 0.0816, 0.1765, 0.3269, 0.2333, 0.55, 0.25, 0.3333, 0.5, 0.0, 0.0, 0.5, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0323, 0.0357, 0.1887, 0.1389, 0.1667, 0.3, 0.0, 0.3333, 0.1667, 0.2, 0.25, 0.3333, 0.5, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0103, 0.0513, 0.0976, 0.16, 0.087, 0.0526, 0.3571, 0.125, 0.2, 0.0, 0.3333, 0.0, 0.0, 0.5, 1.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0192, 0.0217, 0.0741, 0.0769, 0.0, 0.3077, 0.25, 0.3333, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0667, 0.1538, 0.125, 0.1111, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.025, 0.0, 0.0, 0.0, 0.0714, 0.2, 0.3333, 0.2, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0238, 0.0741, 0.0769, 0.0769, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0323, 0.0333, 0.0714, 0.0, 0.25, 0.0, 0.0, 0.25, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.037, 0.05, 0.0769, 0.125, 0.0}},
                {{0.266, 0.3504, 0.2787, 0.4762, 0.4737, 0.5556, 0.75, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0335, 0.0982, 0.0886, 0.3226, 0.3243, 0.2609, 0.4, 0.5, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.037, 0.0175, 0.1429, 0.36, 0.2, 0.375, 0.2, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0128, 0.0323, 0.0222, 0.0, 0.1, 0.0769, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0769, 0.0, 0.3333, 0.25, 0.5, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0323, 0.1429, 0.0667, 0.125, 0.1667, 0.0, 0.3333, 0.5, 1.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0357, 0.0, 0.0, 0.0909, 0.2, 0.0, 0.3333, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}}
        };


        double probDeRetencaoPeriodoSemestre [3][10][22] = {
                {{0.551, 0.5403, 0.597, 0.4875, 0.5128, 0.45, 0.4444, 0.75, 0.6667, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.3355, 0.6023, 0.689, 0.5877, 0.6418, 0.5349, 0.6087, 0.5714, 0.5, 0.75, 0.6667, 0.5, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.4106, 0.6364, 0.7091, 0.7051, 0.6364, 0.5429, 0.7368, 0.6429, 0.7778, 0.8571, 0.5, 0.6667, 0.5, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.4615, 0.3829, 0.5357, 0.5581, 0.7547, 0.7442, 0.625, 0.6, 0.8333, 0.6, 0.6667, 0.5, 1.0, 1.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.3478, 0.46, 0.6765, 0.5238, 0.7222, 0.6444, 0.7, 0.4762, 0.4, 1.0, 0.5, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.2368, 0.4935, 0.6933, 0.5, 0.8085, 0.5532, 0.8, 0.6667, 0.5, 0.5, 0.75, 0.6667, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.2927, 0.5, 0.7925, 0.6346, 0.65, 0.7188, 0.68, 0.5882, 0.5, 1.0, 0.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.4091, 0.5672, 0.6984, 0.6316, 0.6341, 0.8966, 0.6429, 0.3333, 0.3333, 1.5, 1.3333, 0.75, 0.6667, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.4706, 0.6, 0.8596, 0.7286, 0.7797, 0.7872, 1.0, 0.4872, 0.5789, 0.1818, 1.0, 0.5, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.5758, 0.625, 0.9423, 0.7258, 0.86, 0.7727, 0.9143, 0.9062, 0.931, 0.7407, 0.65, 0.6154, 0.25, 0.0}},
                {{0.5504, 0.5573, 0.5205, 0.5263, 0.55, 0.4545, 0.6, 0.6667, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.3745, 0.5782, 0.6118, 0.5769, 0.6667, 0.4, 0.75, 0.6667, 0.5, 1.0, 1.0, 0.5, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.5242, 0.631, 0.6792, 0.8333, 0.6667, 0.55, 0.8182, 0.6667, 0.8333, 0.8, 0.75, 0.6667, 0.5, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.4231, 0.567, 0.5128, 0.5854, 0.92, 0.8261, 0.7368, 0.5714, 0.625, 0.6, 1.0, 0.6667, 1.0, 1.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.3043, 0.6731, 0.587, 0.4815, 1.2308, 0.8125, 0.6154, 0.375, 0.3333, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.2368, 0.7083, 0.6585, 0.4, 1.1538, 0.5, 1.0, 0.8889, 0.25, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.2927, 0.65, 0.5667, 0.6316, 1.1667, 0.7143, 0.6, 0.8333, 0.4, 1.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.4091, 0.7778, 0.5476, 0.4815, 0.9231, 0.6923, 0.9, 0.5556, 0.2, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.4412, 0.697, 0.7419, 0.4667, 0.6429, 0.7778, 0.875, 0.5714, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.5758, 0.625, 0.9423, 0.7258, 0.86, 0.7727, 0.9143, 0.9062, 0.931, 0.7407, 0.65, 0.6154, 0.25, 0.0}},
                {{0.5517, 0.5214, 0.6885, 0.4524, 0.4737, 0.4444, 0.25, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.2871, 0.6339, 0.7722, 0.5968, 0.6216, 0.6522, 0.5333, 0.5, 0.5, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.2951, 0.642, 0.7368, 0.5952, 0.6, 0.5333, 0.625, 0.6, 0.6667, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.1538, 0.5645, 0.5333, 0.6071, 0.65, 0.4615, 0.6667, 1.25, 0.6, 0.3333, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.2292, 0.75, 0.5439, 0.561, 0.5517, 0.7647, 0.5385, 0.4286, 1.3333, 0.5, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.1379, 0.7353, 0.575, 0.6765, 0.5806, 0.7143, 0.5333, 0.75, 0.6667, 0.75, 0.6667, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.25, 1.087, 0.6364, 0.4286, 0.7222, 0.7333, 0.4545, 0.6, 1.0, 0.3333, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.1364, 1.0, 0.7667, 0.5, 1.0625, 0.5, 0.1111, 1.0, 2.0, 1.5, 0.6667, 1.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.4118, 1.0, 0.925, 0.8222, 0.7895, 1.0323, 0.4688, 0.4667, 0.2857, 1.0, 0.5, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}}
        };


        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;
        virtual void finish() override;
};


#endif
