#ifndef USER_HPP
#define USER_HPP
#include <string>

// Classe para representar um usuário na rede social, com atributos como ID, idade e nome.
class Usuario
{
private:
    int id;           // ID do usuário
    int idade;        // Idade do usuário
    std::string nome; // Nome do usuário

public:
    Usuario(int id, int idade, const std::string &nome);

    int getId() const;           // Retorna o ID do usuário.
    int getIdade() const;        // Retorna a idade do usuário.
    std::string getNome() const; // Retorna o nome do usuário.
};
#endif // USER_HPP