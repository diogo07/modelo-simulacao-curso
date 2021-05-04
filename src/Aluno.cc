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


Aluno::Aluno(int numero, std::string nome) {
    this->setNumero(numero);
    this->setNome(nome);
}

void Aluno::setNumero(int numero){
    this->numero = numero;
}

void Aluno::setNome(std::string nome){
    this->nome = nome;
}

void Aluno::setNota(double nota){
    this->nota = nota;
}

std::string Aluno::getNome(){
    return this->nome;
}

double Aluno::getNota(){
    return this->nota;
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

void Aluno::setNovato(bool novato){
    this->novato = novato;
}

void Aluno::setEntrada(double entrada){
    this->entrada = entrada;
}

void Aluno::setSaida(double saida){
    this->saida = saida;
}

int Aluno::getNumero(){
    return this->numero;
}

void Aluno::setEvadido(int evadido){
    this->evadido = evadido;
}

int Aluno::getEvadido(){
    return this->evadido;
}

void Aluno::setFaltas(int faltas){
    this->faltas = faltas;
}

int Aluno::getFaltas(){
    return this->faltas;
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

bool Aluno::getNovato(){
    return this->novato;
}

double Aluno::getEntrada(){
    return this->entrada;
}

double Aluno::getSaida(){
    return this->saida;
}

double Aluno::getQuantidadeSemestresCursados(){
    return (this->saida - this->entrada) / 6;
}
