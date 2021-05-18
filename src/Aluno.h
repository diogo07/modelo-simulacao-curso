#include <string.h>
#include <omnetpp.h>
#include <math.h>
#include <string.h>


class Aluno : public omnetpp::cMessage{
private:
    int numero;
    std::string nome;
    double nota;
    int raca;//1 branco 2 preto 3 indio
    int evadido; //0 nao evadido 1 evadido
    int faltas;
    int qtdMatriculas;
    bool novato;
    double entrada;
    double saida;
    int reprovacoes [10] = {};
public:
    Aluno();
    Aluno(int numero, std::string nome);
    virtual ~Aluno();

    virtual void setNome(std::string nome);
    virtual void setNumero(int numero);
    virtual void setNota(double nota);

    virtual void setRaca(int raca);
    virtual void setEvadido(int evadido);
    virtual void setFaltas(int faltas);

    virtual void setReprovacoes(int index, int reprovacoes);
    virtual void setNovato(bool novato);

    virtual void setEntrada(double entrada);
    virtual void setSaida(double saida);

    virtual std::string getNome();
    virtual int getNumero();
    virtual double getNota();

    virtual int getRaca();
    virtual int getEvadido();
    virtual int getFaltas();

    virtual int getReprovacoes(int index);
    virtual bool getNovato();
    virtual double getEntrada();
    virtual double getSaida();

    virtual int getQtdMatriculas();
    virtual void setQtdMatriculas(int qtde);

    virtual int getQuantidadeSemestresCursados(double saida);


    virtual bool temPossibilidadeDeGraduar(double semestreAtual, int periodoAtual);


};

