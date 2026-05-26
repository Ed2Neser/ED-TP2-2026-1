#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP
#include "mainNode.hpp"
#include "user.hpp"
#include "theme.hpp"

// Estrutura responsável por localizar os elementos do sistema a partir de
// seus identificadores, facilitando a busca e manipulação dos dados dos Grafos.
class Dictionary
{
private:
    Tema **Temas;       // Array de temas, para acesso rápido aos temas por ID.
    int maxTemas;       // Capacidade máxima do array de temas.
    Usuario **Usuarios; // Array de usuários, para acesso rápido aos usuários por ID.
    int maxUsuarios;    // Capacidade máxima do array de usuários.

    MainNode **socialHeads;    // Array de nós cabeça, para acesso rápido aos nós de um Grafo Social.
    MainNode **themeUserHeads; // Array de nós cabeça, para acesso rápido aos nós 'U's de um Grafo de Temas.
    MainNode **themeHeads;     // Array de nós cabeça, para acesso rápido aos nós 'T's de um Grafo de Temas.

public:
    Dictionary();
    ~Dictionary();

    void addUser(Usuario *user); // Adiciona um usuário ao dicionário, e atualiza a lista de nós cabeça.
    void addTheme(Tema *theme);  // Adiciona um tema ao dicionário e atualiza a lista de nós cabeça.

    Usuario *getUser(int id); // Encontra um usuário pelo ID.
    Tema *getTheme(int id);   // Encontra um tema pelo ID.

    MainNode *getSocialHead(int id);    // Encontra um nó cabeça de Grafo Social pelo ID.
    MainNode *getThemeUserHead(int id); // Encontra um nó cabeça 'U' de Grafo de Temas pelo ID.
    MainNode *getThemeHead(int id);     // Encontra um nó cabeça 'T' de Grafo de Temas pelo ID.
};

#endif // DICTIONARY_HPP