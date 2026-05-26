#ifndef MAINNODE_HPP
#define MAINNODE_HPP

// Um nó principal para os Grafos na forma de Lista de Adjacências.
// É componente head para listas duplamente encadeadas, com estrutura adaptável para Temas (T) ou Social (S).
class MainNode
{
public:
    int id;    // ID do nó, para identificação do tema/user.
    char tipo; // 'U' para Usuário 'T' para Tema.
    // Eixo principal: Users Seguidos em Grafo Social, Temas de um User em Grafo de Temas
    // Users que seguem um tema em Grafo de Tema, ou simplesmente "next".
    MainNode *eixoX;

    // Eixo secundário: Seguidores de um User em Grafo Social ou simplesmente "previous".
    MainNode *eixoY;

    MainNode(int id, char tipo);
};

#endif // MAINNODE_HPP