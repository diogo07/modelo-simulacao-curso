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
        simsignal_t reprovadosPorPeriodo[10];

        simsignal_t evadidosPorSemestre[21];
        simsignal_t reprovadosPorSemestre[21];
        simsignal_t graduadosPorSemestre[21];

        simsignal_t evadidosSemestrePeriodo[21][10];
        simsignal_t reprovadosSemestrePeriodo[21][10];
        simsignal_t totalSemestrePeriodo[21][10];
        simsignal_t duracaoTransicaoPeriodo[21][10];


        simsignal_t totalEvadidos;
        simsignal_t quantidadeEvadidosGeral;
        simsignal_t quantidadeReprovadosGeral;

        virtual bool compare(Aluno * aluno1, Aluno * aluno2);
        virtual void adicionarNaTurma(Aluno *aluno);
        virtual void avaliarAlunoPorEvasaoEreprovacao(Aluno *aluno);
        virtual bool evadir(int semestre);
        virtual bool reprovar(int semestre);
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
        int curso = 0; // 0 - BSI, 1 - BCC
        int probsTipo = 1; // 0 - POR PERIODO, 1 - POR SEMESTRE, 2 - SEMESTRE E PERIODO

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
                {0.2385, 0.1055, 0.0799, 0.0643, 0.0449, 0.0503, 0.0383, 0.0126, 0.0049, 0.0769},
                {0.2555, 0.1264, 0.0743, 0.04, 0.0231, 0.0272, 0.0442, 0.0091, 0.0065, 0.0}
        };


        double probsReprovacaoPeriodo [2][10] = {
                {0.7946, 0.5327, 0.5303, 0.4429, 0.4607, 0.3719, 0.4851, 0.3522, 0.7537, 0.4615},
                {0.8632, 0.5333, 0.4966, 0.365, 0.4676, 0.4293, 0.4254, 0.2273, 0.4351, 0.3621}
        };


//      POR DURACAO DE VINCULO
        double probsEvasaoDuracaoVinculo [2][21] = {
                {0.125, 0.1373, 0.112, 0.1262, 0.0573, 0.1071, 0.0718, 0.0324, 0.1, 0.0562, 0.048, 0.0377, 0.0814, 0.013, 0.0364, 0.1087, 0.05, 0.069, 0.0476, 0.0769, 0.0},
                {0.1529, 0.135, 0.0866, 0.1416, 0.0948, 0.1061, 0.0755, 0.0429, 0.0236, 0.0348, 0.01, 0.0602, 0.0533, 0.0172, 0.0698, 0.0417, 0.0769, 0.0, 0.2, 0.0, 0.0}
        };

        double probsReprovacaoDuracaoVinculo [2][23] = {
                {0.5637, 0.5882, 0.6639, 0.5648, 0.5305, 0.5223, 0.5359, 0.5405, 0.6235, 0.5938, 0.688, 0.7075, 0.7442, 0.7013, 0.7455, 0.8696, 0.875, 0.9655, 0.9524, 0.6923, 0.625, 1.0},
                {0.4862, 0.589, 0.6173, 0.6094, 0.6019, 0.5978, 0.5786, 0.45, 0.4331, 0.3739, 0.41, 0.5301, 0.5733, 0.6724, 0.5581, 0.625, 0.6923, 1.0, 0.8, 1.0, 0.0, 0.0},
        };



//      POR PERIODO E DURACAO DE VINCULO
        double probsEvasaoPeriodoDuracaoVinculo[2][11][23] = {
                {{0.2043, 0.2848, 0.2455, 0.2459, 0.1875, 0.2667, 0.2222, 0.2, 0.3333, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0225, 0.0593, 0.1395, 0.2258, 0.087, 0.3793, 0.1304, 0.1818, 0.25, 0.0, 0.0, 0.3333, 0.0, 0.5, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0093, 0.0099, 0.1404, 0.0263, 0.1852, 0.28, 0.0526, 0.3636, 0.1111, 0.2, 0.0, 0.6667, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0147, 0.0889, 0.1, 0.0476, 0.0, 0.3636, 0.1818, 0.2, 0.0, 0.0, 0.0, 0.0, 0.5, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.069, 0.087, 0.15, 0.0714, 0.0, 0.1667, 0.6667, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0278, 0.0, 0.0, 0.0, 0.1333, 0.1538, 0.2222, 0.125, 0.0, 0.3333, 0.3333, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0909, 0.1, 0.1176, 0.0, 0.0909, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.125, 0.0, 0.0, 0.25, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.037, 0.0, 0.0, 0.125, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.5, 0.5, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}},
                {{0.283, 0.2687, 0.1667, 0.2979, 0.1875, 0.2941, 0.3636, 0.1667, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0298, 0.0988, 0.1207, 0.3636, 0.2162, 0.1905, 0.3333, 0.2857, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0339, 0.0244, 0.1481, 0.375, 0.1176, 0.2857, 0.2, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0132, 0.0238, 0.0667, 0.0526, 0.1538, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0172, 0.0, 0.0, 0.0, 0.0, 0.0, 0.1111, 0.0, 0.25, 0.25, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.1538, 0.1, 0.0, 0.0, 0.3333, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0909, 0.0, 0.0, 0.1667, 0.25, 0.0, 0.3333, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.3333, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}},
        };


        double probsReprovacaoPeriodoDuracaoVinculo [2][11][23] = {
                {0.4583, 0.3113, 0.6639, 0.5648, 0.5305, 0.5223, 0.5359, 0.5405, 0.6235, 0.5938, 0.688, 0.7075, 0.7558, 0.7013, 0.7636, 0.8696, 0.9, 0.9655, 0.9524, 0.6923, 0.625, 1.0},
                {0.4954, 0.362, 0.6173, 0.6094, 0.6066, 0.5978, 0.5786, 0.45, 0.4331, 0.3739, 0.41, 0.5301, 0.5867, 0.7069, 0.5814, 0.625, 0.6923, 1.0, 0.8, 1.0, 0.0, 0.0},
        };


        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;
        virtual void finish() override;
};


#endif
