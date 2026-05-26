#include "theme.hpp"

Tema::Tema(int id, char tipo, const std::string &nome)
{
    this->id = id;
    this->tipo = tipo;
    this->nome = nome;
}

int Tema::getId() const
{
    return this->id;
}

char Tema::getTipo() const
{
    return this->tipo;
}

std::string Tema::getNome() const
{
    return this->nome;
}
//