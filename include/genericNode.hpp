#ifndef GENERICNODE_HPP
#define GENERICNODE_HPP

// Um nó secundário para os Grafos na forma de Lista de Adjacências.
// É componente de uma lista em formato de duplamente encadeada, para facilitar remoções e inserções.
class genericNode
{
private:
    int id;    // ID do nó, para identificação do tema/user.
    char tipo; // 'U' para Usuário 'T' para Tema.

    genericNode *next; // Ponteiro para o próximo nó na lista.
    genericNode *prev; // Ponteiro para o nó anterior na lista.

public:
    genericNode();
    ~genericNode();
};

#endif // GENERICNODE_HPP