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


        simsignal_t totalEvadidos;
        simsignal_t quantidadeEvadidosGeral;
        simsignal_t quantidadeRetidosGeral;

        virtual bool compare(Aluno * aluno1, Aluno * aluno2);
        virtual void adicionarNaTurma(Aluno *aluno);
        virtual void avaliarAluno(Aluno *aluno);
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
        int indice; // O �NDICE EQUIVALE AO PER�ODO, EX: INDICE 1 = 1� PERIODO
        int portaSaida;
        int portaSaidaInicialRetencao;
        int analiseCurso = 0; // 0 - GERAL, 1 - BSI, 2 - BCC
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
                {0.1726, 0.1337, 0.0925, 0.0547, 0.0371, 0.0484, 0.0369, 0.019, 0.0138, 0.0085},
                {0.1728, 0.1252, 0.1071, 0.0734, 0.0631, 0.0531, 0.0481, 0.0282, 0.0343, 0.0085},
                {0.1723, 0.1429, 0.0767, 0.0299, 0.0179, 0.0437, 0.0242, 0.0064, 0.0, 0.0}
        };



//        {
//                {0.361, 0.1739, 0.0762, 0.0369, 0.0197, 0.0241, 0.0143, 0.0077, 0.0066, 0.0044},
//                {0.3554, 0.1558, 0.084, 0.052, 0.026, 0.024, 0.018, 0.012, 0.012, 0.008},
////              {0.45170000000000005, 0.1185, 0.10769999999999999, 0.0324, 0.054000000000000006, 0, 0, 0.0108, 0, 0}, // BSI 2014
//                {0.3676, 0.1956, 0.067, 0.0192, 0.012, 0.024, 0.0096, 0.0024, 0.0, 0.0}
//        };


        double probDeRetencaoPeriodo [3][10] = {
                {0.1845, 0.1429, 0.1202, 0.0804, 0.0557, 0.0396, 0.0369, 0.0515, 0.0414, 0.0487},
                {0.1951, 0.1477, 0.125, 0.0847, 0.0534, 0.031, 0.0428, 0.0423, 0.0514, 0.0487},
                {0.1723, 0.1376, 0.1151, 0.0746, 0.0573, 0.048, 0.0303, 0.0641, 0.0346, 0.0}
        };


//        {
//                {0.386, 0.1859, 0.0989, 0.0544, 0.0294, 0.0196, 0.014, 0.0208, 0.0196, 0.025},
//                {0.4013, 0.1837, 0.0978, 0.0599, 0.022, 0.014, 0.016, 0.018, 0.018, 0.046},
////                {0.5269, 0.172, 0.1183, 0.0431, 0.0108, 0, 0.0108, 0.0323, 0, 0.0216}, // BSI 2014
//                {0.3676, 0.1885, 0.1003, 0.0478, 0.0382, 0.0263, 0.0119, 0.024, 0.0215, 0.0}
//        };


//      POR SEMESTRE
        double probDeEvasaoSemestre [3][21] = {
//                {0.1424, 0.1315, 0.0859, 0.1, 0.0576, 0.0555, 0.0337, 0.0175, 0.0271, 0.0164, 0.0088, 0.0132, 0.0121, 0.0022, 0.0055, 0.0066, 0.0033, 0.0022, 0.0022, 0.0011, 0.0},
//                {0.1378, 0.1338, 0.1018, 0.0958, 0.048, 0.058, 0.03, 0.014, 0.04, 0.02, 0.014, 0.012, 0.014, 0.002, 0.004, 0.01, 0.004, 0.004, 0.002, 0.002, 0.0},
//                {0.148, 0.1288, 0.0669, 0.105, 0.0692, 0.0525, 0.0383, 0.0215, 0.012, 0.012, 0.0024, 0.0144, 0.0096, 0.0024, 0.0072, 0.0024, 0.0024, 0.0, 0.0024, 0.0, 0.0}

                {0.1425, 0.1534, 0.1183, 0.1554, 0.106, 0.1141, 0.0783, 0.0441, 0.0744, 0.054, 0.0342, 0.0659, 0.0815, 0.0204, 0.0714, 0.1132, 0.0833, 0.0769, 0.125, 0.125, 0.0},
                {0.1377, 0.1551, 0.1397, 0.1524, 0.0902, 0.1198, 0.0704, 0.0354, 0.107, 0.0685, 0.056, 0.0625, 0.0909, 0.0182, 0.0435, 0.125, 0.069, 0.0952, 0.0769, 0.125, 0.0},
                {0.1483, 0.1513, 0.0924, 0.1588, 0.1239, 0.1073, 0.0874, 0.0545, 0.0336, 0.0379, 0.0092, 0.0698, 0.069, 0.0233, 0.125, 0.0769, 0.1429, 0.0, 0.3333, 0.0, 0.0}


        };

        double probDeRetencaoSemestre [3][21] = {
//                {0.2696, 0.2512, 0.1174, 0.0522, 0.0348, 0.025, 0.0173, 0.0152, 0.0185, 0.0109, 0.0109, 0.0066, 0.0033, 0.0011, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0196},
//                {0.3234, 0.2316, 0.1218, 0.0439, 0.026, 0.024, 0.02, 0.016, 0.018, 0.01, 0.012, 0.004, 0.006, 0.002, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.018},
//                {0.2053, 0.2744, 0.1122, 0.0621, 0.0454, 0.0263, 0.0143, 0.0143, 0.0191, 0.012, 0.0096, 0.0096, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0215}

                {0.2699, 0.2928, 0.1617, 0.0811, 0.064, 0.0515, 0.0404, 0.0386, 0.0506, 0.036, 0.0427, 0.033, 0.0222, 0.0102, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 9.0},
                {0.3234, 0.2685, 0.1671, 0.0698, 0.0489, 0.0496, 0.0469, 0.0404, 0.0481, 0.0342, 0.048, 0.0208, 0.039, 0.0182, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 4.5},
                {0.2057, 0.3221, 0.1551, 0.0939, 0.0812, 0.0537, 0.0328, 0.0364, 0.0537, 0.0379, 0.0367, 0.0465, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0215}
        };




//        POR PERIODO E SEMESTRE
    double probDeEvasaoPeriodoSemestre[3][10][21] = {
            {{0.1261, 0.2081, 0.1937, 0.2667, 0.1928, 0.2683, 0.2273, 0.2, 0.2, 0.6667, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0508, 0.084, 0.2275, 0.1709, 0.2535, 0.2174, 0.24, 0.2857, 0.0, 0.0, 0.25, 0.3333, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0243, 0.0663, 0.0982, 0.175, 0.1636, 0.0857, 0.2105, 0.0714, 0.1111, 0.4286, 0.1667, 0.3333, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0741, 0.0341, 0.0355, 0.0581, 0.0727, 0.0455, 0.1562, 0.0476, 0.0769, 0.0, 0.1667, 0.0, 0.0, 0.5, 0.5, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0417, 0.01, 0.0194, 0.0119, 0.0, 0.087, 0.0968, 0.0476, 0.1818, 0.25, 0.25, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0263, 0.0282, 0.0625, 0.1042, 0.0312, 0.08, 0.1579, 0.0, 0.25, 0.5, 0.3333, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0238, 0.0, 0.0, 0.0192, 0.025, 0.0625, 0.12, 0.1176, 0.0, 0.3333, 0.1667, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0149, 0.0317, 0.0175, 0.0244, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.25, 0.3333, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.02, 0.0175, 0.0143, 0.0, 0.0426, 0.0, 0.0, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0345, 0.037, 0.05, 0.0769, 0.0}},
            {{0.1218, 0.2125, 0.2406, 0.2192, 0.1795, 0.2857, 0.1667, 0.2, 0.3333, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.046, 0.102, 0.1977, 0.1346, 0.3667, 0.1, 0.25, 0.3333, 0.0, 0.0, 0.5, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.024, 0.119, 0.0926, 0.1351, 0.2, 0.0, 0.2727, 0.1111, 0.1667, 0.2, 0.25, 0.3333, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.037, 0.0408, 0.0506, 0.0976, 0.0769, 0.0417, 0.2632, 0.0714, 0.125, 0.0, 0.3333, 0.0, 0.0, 0.5, 0.5, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0417, 0.0192, 0.0426, 0.037, 0.0, 0.2353, 0.1429, 0.125, 0.3333, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0476, 0.0645, 0.1429, 0.1176, 0.0, 0.1, 0.2, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0238, 0.0, 0.0, 0.0, 0.0833, 0.1429, 0.2, 0.1667, 0.0, 0.3333, 0.3333, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0222, 0.0476, 0.037, 0.0769, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0303, 0.0323, 0.0333, 0.0, 0.2222, 0.0, 0.0, 0.25, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0345, 0.037, 0.05, 0.0769, 0.0}},
            {{0.1313, 0.2029, 0.1417, 0.3226, 0.2045, 0.25, 0.3, 0.2, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0569, 0.0609, 0.2593, 0.2, 0.1707, 0.3077, 0.2353, 0.25, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0246, 0.0122, 0.1034, 0.2093, 0.12, 0.2, 0.125, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0256, 0.0161, 0.0222, 0.069, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0588, 0.0, 0.125, 0.3333, 0.25, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0294, 0.0968, 0.0476, 0.0667, 0.1111, 0.0, 0.25, 0.3333, 0.5, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0303, 0.0, 0.0, 0.0667, 0.0909, 0.0, 0.3333, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}}
    };

//        {
//                {
//                    { 0.1261, 0.1011, 0.0533, 0.0391, 0.0174, 0.012, 0.0054, 0.0022, 0.0011, 0.0022, 0, 0.0011, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//                    { 0.0163, 0.0261, 0.0239, 0.0413, 0.0217, 0.0196, 0.0109, 0.0065, 0.0043, 0, 0, 0.0011, 0.0011, 0, 0.0011, 0, 0, 0, 0, 0, 0 },
//                    { 0, 0.0043, 0.0065, 0.012, 0.012, 0.0152, 0.0098, 0.0033, 0.0043, 0.0011, 0.0011, 0.0033, 0.0011, 0.0011, 0, 0.0011, 0, 0, 0, 0, 0 },
//                    { 0, 0, 0.0022, 0.0065, 0.0054, 0.0054, 0.0043, 0.0022, 0.0054, 0.0011, 0.0011, 0, 0.0011, 0, 0, 0.0011, 0.0011, 0, 0, 0, 0 },
//                    { 0, 0, 0, 0.0011, 0.0011, 0.0022, 0.0011, 0, 0.0043, 0.0033, 0.0011, 0.0022, 0.0011, 0.0011, 0.0011, 0, 0, 0, 0, 0, 0 },
//                    { 0, 0, 0, 0, 0, 0, 0.0022, 0.0022, 0.0033, 0.0054, 0.0011, 0.0022, 0.0033, 0, 0.0011, 0.0022, 0.0011, 0, 0, 0, 0 },
//                    { 0, 0, 0, 0, 0, 0.0011, 0, 0, 0.0011, 0.0011, 0.0022, 0.0033, 0.0022, 0, 0.0022, 0.0011, 0, 0, 0, 0, 0 },
//                    { 0, 0, 0, 0, 0, 0, 0, 0.0011, 0.0022, 0.0011, 0.0011, 0, 0, 0, 0, 0, 0, 0.0011, 0.0011, 0, 0 },
//                    { 0, 0, 0, 0, 0, 0, 0, 0, 0.0011, 0.0011, 0.0011, 0, 0.0022, 0, 0, 0.0011, 0, 0, 0, 0, 0 },
//                    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0011, 0.0011, 0.0011, 0.0011, 0 }
//                },
//                {
//                    { 0.1218, 0.1018, 0.0639, 0.0319, 0.014, 0.012, 0.004, 0.002, 0.002, 0.002, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//                    { 0.016, 0.024, 0.0299, 0.0339, 0.014, 0.022, 0.004, 0.004, 0.004, 0, 0, 0.002, 0, 0, 0.002, 0, 0, 0, 0, 0, 0 },
//                    { 0, 0.008, 0.006, 0.02, 0.01, 0.01, 0.012, 0, 0.006, 0.002, 0.002, 0.002, 0.002, 0.002, 0, 0.002, 0, 0, 0, 0, 0 },
//                    { 0, 0, 0.002, 0.008, 0.008, 0.008, 0.004, 0.002, 0.01, 0.002, 0.002, 0, 0.002, 0, 0, 0.002, 0.002, 0, 0, 0, 0 },
//                    { 0, 0, 0, 0.002, 0.002, 0.004, 0.002, 0, 0.008, 0.004, 0.002, 0.002, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//                    { 0, 0, 0, 0, 0, 0, 0.004, 0.004, 0.004, 0.004, 0, 0.002, 0.004, 0, 0, 0.002, 0, 0, 0, 0, 0 },
//                    { 0, 0, 0, 0, 0, 0.002, 0, 0, 0, 0.002, 0.004, 0.004, 0.002, 0, 0.002, 0.002, 0, 0, 0, 0, 0 },
//                    { 0, 0, 0, 0, 0, 0, 0, 0.002, 0.004, 0.002, 0.002, 0, 0, 0, 0, 0, 0, 0.002, 0, 0, 0 },
//                    { 0, 0, 0, 0, 0, 0, 0, 0, 0.002, 0.002, 0.002, 0, 0.004, 0, 0, 0.002, 0, 0, 0, 0, 0 },
//                    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.002, 0.002, 0.002, 0.002, 0 }
//                },
//                {
//                    { 0.1313, 0.1002, 0.0406, 0.0477, 0.0215, 0.0119, 0.0072, 0.0024, 0, 0.0024, 0, 0.0024, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//                    { 0.0167, 0.0286, 0.0167, 0.0501, 0.031, 0.0167, 0.0191, 0.0095, 0.0048, 0, 0, 0, 0.0024, 0, 0, 0, 0, 0, 0, 0, 0 },
//                    { 0, 0, 0.0072, 0.0024, 0.0143, 0.0215, 0.0072, 0.0072, 0.0024, 0, 0, 0.0048, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//                    { 0, 0, 0.0024, 0.0048, 0.0024, 0.0024, 0.0048, 0.0024, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//                    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0024, 0, 0.0024, 0.0024, 0.0024, 0.0024, 0, 0, 0, 0, 0, 0 },
//                    { 0, 0, 0, 0, 0, 0, 0, 0, 0.0024, 0.0072, 0.0024, 0.0024, 0.0024, 0, 0.0024, 0.0024, 0.0024, 0, 0, 0, 0 },
//                    { 0, 0, 0, 0, 0, 0, 0, 0, 0.0024, 0, 0, 0.0024, 0.0024, 0, 0.0024, 0, 0, 0, 0, 0, 0 },
//                    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0024, 0, 0 },
//                    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//                    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
//                }
//        };


        double probDeRetencaoPeriodoSemestre [3][10][21] = {
                {{0.2685, 0.1969, 0.0079, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.2966, 0.084, 0.0359, 0.0171, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.2996, 0.0843, 0.0089, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.3704, 0.142, 0.0993, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.125, 0.14, 0.0874, 0.0, 0.0185, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0256, 0.1169, 0.0658, 0.0141, 0.0208, 0.0, 0.0312, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.119, 0.0625, 0.0755, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.1333, 0.0448, 0.0476, 0.0351, 0.0732, 0.069, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.1429, 0.12, 0.0526, 0.0143, 0.0339, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.2059, 0.0877, 0.0943, 0.0317, 0.0588, 0.0222, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}},
                {{0.3214, 0.1292, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.3142, 0.0612, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.344, 0.0476, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.3333, 0.1633, 0.0506, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0833, 0.1538, 0.0213, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0256, 0.125, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.119, 0.075, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.1333, 0.0667, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.1429, 0.0606, 0.0, 0.0333, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.2059, 0.0877, 0.0943, 0.0317, 0.0588, 0.0222, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}},
                {{0.2053, 0.2754, 0.0167, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.2749, 0.113, 0.0741, 0.0308, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.2541, 0.122, 0.0172, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.1154, 0.1613, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.125, 0.1429, 0.0, 0.0244, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.1034, 0.1471, 0.025, 0.0294, 0.0, 0.0476, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0417, 0.1739, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.1429, 0.0667, 0.1071, 0.125, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.2353, 0.1154, 0.0, 0.0444, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}}
        };



//        {
//                {
//                    { 0.2685, 0.0957, 0.0022, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0196 },
//                    { 0.0011, 0.1522, 0.0239, 0.0065, 0.0022, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//                    { 0, 0.0022, 0.0804, 0.0152, 0.0011, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//                    { 0, 0.0011, 0.0109, 0.0272, 0.0152, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//                    { 0, 0, 0, 0.0033, 0.0152, 0.0098, 0, 0.0011, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//                    { 0, 0, 0, 0, 0.0011, 0.0098, 0.0054, 0.0011, 0.0011, 0, 0.0011, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//                    { 0, 0, 0, 0, 0, 0.0054, 0.0043, 0.0043, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//                    { 0, 0, 0, 0, 0, 0, 0.0065, 0.0033, 0.0033, 0.0022, 0.0033, 0.0022, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//                    { 0, 0, 0, 0, 0, 0, 0.0011, 0.0054, 0.0065, 0.0033, 0.0011, 0.0022, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//                    { 0, 0, 0, 0, 0, 0, 0, 0, 0.0076, 0.0054, 0.0054, 0.0022, 0.0033, 0.0011, 0, 0, 0, 0, 0, 0, 0 }
//                },
//                {
//                    { 0.3214, 0.0619, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.018 },
//                    { 0.002, 0.1637, 0.018, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//                    { 0, 0.004, 0.0858, 0.008, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//                    { 0, 0.002, 0.018, 0.0319, 0.008, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//                    { 0, 0, 0, 0.004, 0.016, 0.002, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//                    { 0, 0, 0, 0, 0.002, 0.012, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//                    { 0, 0, 0, 0, 0, 0.01, 0.006, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//                    { 0, 0, 0, 0, 0, 0, 0.012, 0.006, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//                    { 0, 0, 0, 0, 0, 0, 0.002, 0.01, 0.004, 0, 0.002, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//                    { 0, 0, 0, 0, 0, 0, 0, 0, 0.014, 0.01, 0.01, 0.004, 0.006, 0.002, 0, 0, 0, 0, 0, 0, 0 }
//                },
//                {
//                    { 0.2053, 0.136, 0.0048, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0215 },
//                    { 0, 0.1384, 0.031, 0.0143, 0.0048, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//                    { 0, 0, 0.074, 0.0239, 0.0024, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//                    { 0, 0, 0.0024, 0.0215, 0.0239, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//                    { 0, 0, 0, 0.0024, 0.0143, 0.0191, 0, 0.0024, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//                    { 0, 0, 0, 0, 0, 0.0072, 0.0119, 0.0024, 0.0024, 0, 0.0024, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//                    { 0, 0, 0, 0, 0, 0, 0.0024, 0.0095, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//                    { 0, 0, 0, 0, 0, 0, 0, 0, 0.0072, 0.0048, 0.0072, 0.0048, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//                    { 0, 0, 0, 0, 0, 0, 0, 0, 0.0095, 0.0072, 0, 0.0048, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//                    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
//                }
//        };

        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;
        virtual void finish() override;
};


#endif
