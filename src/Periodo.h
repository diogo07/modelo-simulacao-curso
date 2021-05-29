#ifndef __CURSO_PERIODO_H
#define __CURSO_PERIODO_H

#include <omnetpp.h>
#include "Aluno.h"

using namespace omnetpp;

class Periodo : public cSimpleModule {
    private:
        simsignal_t tamanhoFilaEspera;
        simsignal_t tamanhoTurma;
        simsignal_t quantidadeEvadidos;
        simsignal_t quantidadeAprovados;
        simsignal_t quantidadeReprovados;
        simsignal_t totalEvadidos;
        simsignal_t evadidosPorSemestre[21];
        simsignal_t retidosPorSemestre[21];
        simsignal_t aprovadosPorSemestre[21];
        simsignal_t evadidos_Semestre_periodo[21][10];
        simsignal_t aprovados_Semestre_periodo[21][10];
        simsignal_t retidos_Semestre_periodo[21][10];
        simsignal_t turma_Semestre_periodo[21][10];



        simsignal_t quantidadeEvadidosGeral;
        simsignal_t quantidadeAprovadosGeral;
        simsignal_t quantidadeReprovadosGeral;

        virtual bool compare(Aluno * aluno1, Aluno * aluno2);
        virtual void adicionarNaTurma(Aluno *aluno);
        virtual void avaliarAluno(Aluno *aluno);
        virtual bool evadir();
        virtual bool reter();
        virtual double notaAleatoria();
        virtual void emitirDadosDoPeriodo();
        virtual void processarAluno(Aluno *aluno);
        virtual void iniciarEstatisticasEmArray();

    public:
        virtual ~Periodo() override;

    protected:
        cQueue turma;
        cQueue filaEspera;
        cQueue evadidos;
        int capacidadeTurma;
        int indice;
        int portaSaida;
        int portaSaidaInicialReprovacao;
        int contadorDeAlunosNaTurma;
        int aprovados;
        int reprovados;

        int aprovadosGeral;
        int reprovadosGeral;
        int evadidosGeral;

//      BOUMI
//        double taxasDeEvasao [10] =  {0.2126, 0.2126, 0.2407, 0.2407, 0.2503, 0.2503, 0.2534, 0.2534, 0.2544, 0.2544 };
//        double taxasDeRetencao [10] = {0.47, 0.47, 0.195, 0.195, 0.0654, 0.0654, 0.0207, 0.0207, 0.064, 0.064 };

//      JU
        double taxasDeRetencao [10] =  {0.25, 0.25, 0.3, 0.3, 0.2, 0.2, 0.15, 0.15, 0.2, 0.2};
        double taxasDeEvasao [10] = {0.12, 0.12, 0.1, 0.1, 0.07, 0.07, 0.075, 0.075, 0.03, 0.03};
        SimTime tempo;
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;
        virtual void finish() override;
};


#endif
