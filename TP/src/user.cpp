#include "user.hpp"

Usuario::Usuario(int id, int idade, const std::string &nome)
{
    this->id = id;
    this->idade = idade;
    this->nome = nome;
}

int Usuario::getId() const
{
    return this->id;
}

int Usuario::getIdade() const
{
    return this->idade;
}

std::string Usuario::getNome() const
{
    return this->nome;
}