#include "Aluno.h"
#include <math.h>
#include <string.h>
#include <omnetpp.h>

Aluno::Aluno() {
    // TODO Auto-generated constructor stub
}

Aluno::~Aluno() {
    // TODO Auto-generated destructor stub
}

int Aluno::getRaca(){
    return this->raca;
}

void Aluno::setRaca(int raca){
    this->raca = raca;
}

void Aluno::setReprovacoes(int index, int reprovacoes){
    this->reprovacoes[index] = reprovacoes;
}

void Aluno::setEntradaPeriodo(int index, int entrada){
    this->entradaPeriodo[index] = entrada;
}

void Aluno::setSaidaPeriodo(int index, int saida){
    this->saidaPeriodo[index] = saida;
}

void Aluno::setNovato(bool novato){
    this->novato = novato;
}

void Aluno::setEntrada(double entrada){
    this->entrada = entrada;
}

void Aluno::setSaida(double saida){
    this->saida = saida;
}

void Aluno::setQtdMatriculas(int qtde){
    this->qtdMatriculas = qtde;
}

int Aluno::getQtdMatriculas(){
    return this->qtdMatriculas;
}

int Aluno::getReprovacoes(int index){
    return this->reprovacoes[index];
}

int Aluno::getEntradaPeriodo(int index){
    return this->entradaPeriodo[index];
}

int Aluno::getSaidaPeriodo(int index){
    return this->saidaPeriodo[index];
}

int Aluno::getTotalReprovacoes(){
    int total = 0;
    for (int i = 0; i > 10; i++){
        total += this->reprovacoes[i];
    }

    return total;
}

bool Aluno::getNovato(){
    return this->novato;
}

double Aluno::getEntrada(){
    return this->entrada;
}

double Aluno::getSaida(){
    return this->saida;
}

int Aluno::getQuantidadeSemestresCursados(double saida){
    return (int)((saida - this->entrada) / 6);
}

bool Aluno::temPossibilidadeDeGraduar(double semestreAtual, int periodoAtual){
    int semestres = (int) ((semestreAtual - this->entrada) / 6);
    return (semestres + (10 - periodoAtual)) <= 15;
}

