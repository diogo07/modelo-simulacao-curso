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

        simsignal_t evadidosPorSemestre[21];
        simsignal_t reprovadosPorSemestre[21];
        simsignal_t graduadosPorSemestre[21];
        simsignal_t totalPorSemestre[21];
        simsignal_t duracaoTransicaoPeriodo[21];

        simsignal_t totalEvadidos;
        simsignal_t quantidadeEvadidosGeral;
        simsignal_t quantidadeReprovadosGeral;

        virtual bool compare(Aluno * aluno1, Aluno * aluno2);
        virtual void adicionarNaTurma(Aluno *aluno);
        virtual void avaliarAlunoPorEvasaoEreprovacao(Aluno *aluno);
        virtual bool evadir(int semestre);
        virtual bool reprovar(int semestre);
        virtual bool graduar(int semestre);
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
        int portaSaidaInicialReprovacao;
        int curso = 1; // 0 - BSI, 1 - BCC
        int probsTipo = 2; // 0 - POR PERIODO, 1 - POR SEMESTRE, 2 - SEMESTRE E PERIODO

//      ESPECIFICOS
        int contadorDeAlunosNaTurma;

//      GERAL
        int reprovadosGeral;
        int evadidosGeral;


//      TESTES
        bool evadido = false;

/**
 *
 * METRICAS DE EVASAO E REPROVACAO USADAS NO MODELO
 *
 */


//      UFRPE


//      POR PERIODO
        double probsEvasaoPeriodo [2][10] = {
                {0.2384, 0.1028, 0.0759, 0.0415, 0.0437, 0.0428, 0.0087, 0.0114, 0.0, 0.0},
                {0.2573, 0.1135, 0.0662, 0.0272, 0.0267, 0.0396, 0.0138, 0.0142, 0.0, 0.0}
        };


        double probsReprovacaoPeriodo [2][10] = {
                {0.6464, 0.6116, 0.5241, 0.4505, 0.4325, 0.5058, 0.476, 0.6866, 0.0},
                {0.6946, 0.5482, 0.4355, 0.4942, 0.4844, 0.4653, 0.3379, 0.3617, 0.2963},
        };


//      POR DURACAO DE VINCULO
        double probsEvasaoDuracaoVinculo [2][23] = {
                {0.1262, 0.1361, 0.1133, 0.1275, 0.0581, 0.1045, 0.0732, 0.033, 0.1018, 0.051, 0.0492, 0.0288, 0.0843, 0.0133, 0.037, 0.0698, 0.0526, 0.069, 0.0476, 0.0769, 0.0, 0.0},
                {0.1531, 0.1312, 0.0886, 0.1397, 0.0927, 0.0983, 0.0649, 0.0365, 0.0238, 0.0351, 0.0102, 0.05, 0.0417, 0.0179, 0.0714, 0.0435, 0.0833, 0.0, 0.25, 0.0, 0.0, 0.0},
        };

        double probsReprovacaoDuracaoVinculo [2][23] = {
                {0.4827, 0.5396, 0.4958, 0.4698, 0.4302, 0.5136, 0.478, 0.5, 0.5689, 0.5287, 0.5656, 0.625, 0.6145, 0.5467, 0.7222, 0.7674, 0.7368, 0.6897, 0.4286, 0.3846, 0.25, 0.0},
                {0.45, 0.45, 0.4797, 0.5415, 0.5122, 0.474, 0.4221, 0.3942, 0.3651, 0.2719, 0.3878, 0.5, 0.4028, 0.4107, 0.3333, 0.3043, 0.5, 0.5, 0.75, 0.0, 0.0, 0.0},
        };



//      POR PERIODO E DURACAO DE VINCULO
        double probsEvasaoPeriodoDuracaoVinculo[2][10][22] = {
                {{0.1163, 0.2359, 0.2348, 0.2419, 0.1818, 0.1875, 0.25, 0.2, 0.3333, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0431, 0.0882, 0.2048, 0.0678, 0.3158, 0.1818, 0.1053, 0.3333, 0.0, 0.1667, 0.1667, 0.2, 0.25, 0.3333, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0098, 0.0556, 0.098, 0.1875, 0.1538, 0.0588, 0.3636, 0.1, 0.2, 0.0, 0.3333, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0159, 0.0, 0.0222, 0.04, 0.0, 0.2941, 0.1538, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0204, 0.0455, 0.069, 0.0, 0.1111, 0.0714, 0.2, 0.1667, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0833, 0.0952, 0.1667, 0.0, 0.0833, 0.0, 0.25, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0769, 0.0, 0.0, 0.125, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0385, 0.0417, 0.0556, 0.1111, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}},
                {{0.1375, 0.2397, 0.1647, 0.2979, 0.1875, 0.2941, 0.4, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0402, 0.1039, 0.2963, 0.2195, 0.1739, 0.1818, 0.3333, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0183, 0.0, 0.0938, 0.3478, 0.1053, 0.2222, 0.1667, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.026, 0.0208, 0.0, 0.069, 0.0556, 0.0, 0.0, 0.0, 0.25, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.1176, 0.1111, 0.0, 0.25, 0.2, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0435, 0.0455, 0.0, 0.1667, 0.125, 0.0, 0.25, 0.3333, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0345, 0.0, 0.0, 0.0, 0.125, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.2, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}},
        };


        double probsReprovacaoPeriodoDuracaoVinculo [2][10][22] = {
                {{0.4827, 0.5897, 0.5391, 0.5323, 0.4848, 0.5, 0.625, 0.6, 0.6667, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.4928, 0.5588, 0.6867, 0.6102, 0.8158, 0.5758, 0.6316, 0.8333, 0.6, 1.0, 0.8333, 0.8, 0.75, 0.6667, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.3627, 0.4111, 0.5098, 0.7188, 0.5769, 0.5882, 0.9091, 0.5, 0.8, 0.75, 0.6667, 0.5, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.2063, 0.3846, 0.4667, 0.6, 0.7, 0.7647, 0.4615, 0.2857, 1.0, 1.0, 0.3333, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.16, 0.4082, 0.4545, 0.4828, 0.7895, 0.6667, 0.5714, 0.6, 0.1667, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.25, 0.4762, 0.5349, 0.5278, 0.619, 0.6667, 0.5625, 0.5833, 0.4, 0.75, 0.6667, 0.5, 1.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.1481, 0.3824, 0.4857, 0.5, 0.5, 0.5625, 0.7692, 0.6667, 0.6154, 0.625, 0.4, 0.5, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.1333, 0.2903, 0.7333, 0.6579, 0.8611, 0.6579, 0.7857, 0.9231, 0.8519, 0.9231, 0.75, 0.5, 0.5556, 0.4, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}},
                {{0.45, 0.5822, 0.5529, 0.6809, 0.5312, 0.5882, 0.5, 0.2, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.3391, 0.6623, 0.7407, 0.5366, 0.4348, 0.5455, 0.5, 0.3333, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.2936, 0.5098, 0.625, 0.6522, 0.4211, 0.5556, 0.6667, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.3377, 0.5208, 0.6486, 0.5172, 0.5, 0.2727, 0.3333, 1.0, 0.75, 0.3333, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.4038, 0.4524, 0.4138, 0.5385, 0.4762, 0.4118, 0.4444, 0.75, 1.0, 0.4, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.129, 0.4839, 0.3333, 0.5217, 0.2727, 0.5625, 0.6667, 0.625, 0.6, 0.75, 0.6667, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.16, 0.4737, 0.3103, 0.1579, 0.375, 0.4, 0.25, 0.3333, 1.0, 0.6667, 1.0, 0.5, 1.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.1, 0.2308, 0.4167, 0.3077, 0.5263, 0.3125, 0.3636, 0.6, 0.0, 0.5, 0.0, 1.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.1579, 0.087, 0.28, 0.3667, 0.375, 0.4194, 0.1724, 0.2308, 0.3333, 1.0, 0.5, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}},
        };

        double probsGraduacaoPeriodoDuracaoVinculo [2][10][22] = {
                {{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.04, 0.0, 0.0625, 0.0833, 0.0, 0.0909, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.1429, 0.3571, 0.175, 0.2791, 0.0286, 0.1944, 0.1724, 0.0714, 0.0769, 0.0417, 0.0556, 0.25, 0.2857, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.6667, 0.5, 0.75, 1.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0}},
                {{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0303, 0.0, 0.0476, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0625, 0.0333, 0.05, 0.0, 0.0, 0.2222, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.2609, 0.4583, 0.3714, 0.1944, 0.2927, 0.1471, 0.4062, 0.4, 0.5714, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}}
        };


        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;
        virtual void finish() override;
};


#endif
