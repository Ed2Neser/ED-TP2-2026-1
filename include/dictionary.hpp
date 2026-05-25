#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP
#include "mainNode.hpp"

// Estrutura responsável por localizar os elementos do sistema a partir de
// seus identificadores, facilitando a busca e manipulação dos dados dos Grafos.
class Dictionary
{
private:
    MainNode *socialHeads;     // Array de nós cabeça, para acesso rápido aos nós de um Grafo Social.
    MainNode *themeUserHeads;  // Array de nós cabeça, para acesso rápido aos nós 'U's de um Grafo de Temas.
    MainNode *themeThemeHeads; // Array de nós cabeça, para acesso rápido aos nós 'T's de um Grafo de Temas.

public:
    Dictionary();
    ~Dictionary();
};

#endif // DICTIONARY_HPP