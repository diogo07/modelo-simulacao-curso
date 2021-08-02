#include <string.h>
#include <omnetpp.h>
#include <math.h>
#include <string.h>


class Aluno : public omnetpp::cMessage{
private:
    int numero;
    int raca;
    int qtdMatriculas;
    bool novato;
    double entrada;
    double saida;
    int reprovacoes [10] = {};
public:
    Aluno();
    virtual ~Aluno();

    virtual void setRaca(int raca);

    virtual void setReprovacoes(int index, int reprovacoes);
    virtual void setNovato(bool novato);

    virtual void setEntrada(double entrada);
    virtual void setSaida(double saida);

    virtual int getRaca();

    virtual int getReprovacoes(int index);
    virtual int getTotalReprovacoes();
    virtual bool getNovato();
    virtual double getEntrada();
    virtual double getSaida();

    virtual int getQtdMatriculas();
    virtual void setQtdMatriculas(int qtde);

    virtual int getQuantidadeSemestresCursados(double saida);

    virtual bool temPossibilidadeDeGraduar(double semestreAtual, int periodoAtual);


};

