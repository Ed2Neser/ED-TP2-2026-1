#ifndef GRAFO_HPP
#define GRAFO_HPP
#include "mainNode.hpp"

// Um grafo adaptável para forma de Lista ou Matriz de Adjacências, com estrutura única para se adequar a ambos.
// Pode ser usado para representar tanto Temas quanto Social, usando nós otimizados para cada tipo.
class Grafo
{
private:
    char mode; // 'L' para Lista de Adjacências, 'M' para Matriz de Adjacências.

    int *matrizAdj; // Para representação em Matriz de Adjacências.
    // INCLUIR NUMERO DE LINHAS E COLUNAS ATUAIS QUE A MATRIZ TERIA (com base no tipo e etc)
    int numLinhas;  // Número de linhas da matriz, para controle de alocação e acesso.
    int numColunas; // Número de colunas da matriz, para controle de alocação e acesso.

public:
    Grafo();
    ~Grafo();

    // Lógica de criar os mainNodes, que retorna o endereço do criado para colocar no dicionário.
};

#endif // GRAFO_HPP