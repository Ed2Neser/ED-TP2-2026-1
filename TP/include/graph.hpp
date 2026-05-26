#ifndef GRAFO_HPP
#define GRAFO_HPP
#include "mainNode.hpp"
#include "dictionary.hpp"

// Um grafo adaptável para forma de Lista ou Matriz de Adjacências, com estrutura única para se adequar a ambos.
// Pode ser usado para representar tanto Temas quanto Social, usando nós otimizados para cada tipo.
class Grafo
{
private:
    bool direcionado; // Indica se o grafo é direcionado (Grafo Social) ou não direcionado (Grafo de Temas).

    Dictionary *dict; // Ponteiro para o dicionário global. Permite acesso rápido aos nós cabeça e elementos do sistema.

    char mode; // 'L' para Lista de Adjacências, 'M' para Matriz de Adjacências.

    int **matrizAdj; // Para representação em Matriz de Adjacências.

    int numLinhas;
    int numColunas;

public:
    Grafo(char mode, bool direcionado, Dictionary *dict);
    ~Grafo();

    void setMode(char newMode); // Define o modo do grafo, e aloca a estrutura de dados correspondente (Lista ou Matriz de Adjacências).

    void limparListaAdj(); // Limpa as listas de adjacências, usada na transição de Lista para Matriz.

    void inserirAresta(int idHead, int idNew); // Com base no modo, adiciona uma aresta entre o nó cabeça e um novo nó, seja em Lista ou Matriz de Adjacências.

    void removerFollow(int idSeguidor, int idSeguido); // Usado apenas para remover arestas entre user num Grafo Social, removendo a relação de "seguir" entre dois usuários.
    void consultaDeTemas(int headId);                  // Consulta os temas de interesse de um usuário específico, usando o eixoX do nó cabeça correspondente.
    void consultaDeSeguidores(int headId);             // Consulta os seguidores de um usuário específico, usando o eixoY do nó cabeça correspondente.
    void consultaDeSeguidos(int headId);               // Consulta os usuários seguidos por um usuário específico, usando o eixoX do nó cabeça correspondente.
    void consultaDeAmigos(int headId);                 // Consulta os amigos de um usuário específico, usando ambos os eixos do nó cabeça correspondente para encontrar relações de amizade (seguidores mútuos).
    void checaRelacao(int idUser1, int idUser2);       // Verifica a relação entre dois usuários, como se são amigos, seguidores ou seguidos, usando os eixos do nó cabeça de cada usuário para determinar a relação.
    void checaInteresse(int idUser, int idTema);       // Verifica se um usuário tem interesse em um tema específico, usando o eixoX do nó cabeça do usuário para verificar a presença do tema.
    void consultaPopularidade(int idTema);             // Consulta a popularidade de um tema específico, contando quantos usuários têm interesse nesse tema usando o eixoY do nó cabeça do tema para contar os seguidores.
};
#endif /// GRAFO_HPP