#ifndef THEME_HPP
#define THEME_HPP
#include <string>

// Classe para representar um tema na rede social, com atributos como ID, tipo e nome.
class Tema
{
private:
    int id;           // ID do tema
    char tipo;        // Tipo do tema
    std::string nome; // Nome do tema
public:
    Tema(int id, char tipo, const std::string &nome);

    int getId() const;           // Retorna o ID do tema.
    char getTipo() const;        // Retorna o tipo do tema.
    std::string getNome() const; // Retorna o nome do tema.
};

#endif /// THEME_HPP