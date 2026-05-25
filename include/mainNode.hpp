#ifndef MAINNODE_HPP
#define MAINNODE_HPP
#include "genericNode.hpp"

// Um nó principal para os Grafos na forma de Lista de Adjacências.
// É componente head para listas duplamente encadeadas, com estrutura adaptável para Temas (T) ou Social (S).
class MainNode
{
private:
    int id;    // ID do nó, para identificação do tema/user.
    char tipo; // 'U' para Usuário 'T' para Tema.

    genericNode *listaMain; // Lista principal; Users Seguidos(S) / Temas de um User(T) / Users de um Tema (T).
    genericNode *listaSec;  // Lista secundária; Seguidores de um User (S).

public:
    MainNode();
    ~MainNode();
};

#endif // MAINNODE_HPP