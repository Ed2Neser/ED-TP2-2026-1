#include <iostream>
#include <string>
#include <sstream>
#include "dictionary.hpp"
#include "graph.hpp"
#include "user.hpp"
#include "theme.hpp"

int main()
{
    // 1. Inicializar o Dicionário, o Grafo Social e o Grafo de Temas
    Dictionary *dict = new Dictionary();
    Grafo *grafoSocial = nullptr;
    Grafo *grafoTemas = nullptr;

    // 2. Contadores de IDs sequenciais
    int proxIdUsuario = 0;
    int proxIdTema = 0;

    // 3. Para cada linha da entrada:
    std::string linha;
    while (std::getline(std::cin, linha))
    {
        if (linha.empty())
            continue;

        std::stringstream ss(linha);
        std::string comando;
        ss >> comando;

        if (comando == "A")
        {
            // Armazenamento: A <modo>
            char modo;
            ss >> modo;

            if (grafoSocial == nullptr)
            {
                // Primeira chamada: criar os grafos com o modo informado
                grafoSocial = new Grafo(modo, true, dict);
                grafoTemas = new Grafo(modo, false, dict);
            }
            else
            {
                // Migração: alterar o modo de armazenamento dos grafos
                grafoSocial->setMode(modo);
                grafoTemas->setMode(modo);
            }

            std::cout << "A " << modo << std::endl;
        }
        else if (comando == "T")
        {
            // Tema: T <nome_tema> <tipo_tema>
            std::string nomeTema;
            char tipoTema;
            ss >> nomeTema >> tipoTema;

            Tema *novoTema = new Tema(proxIdTema, tipoTema, nomeTema);
            dict->addTheme(novoTema);

            std::cout << "T " << proxIdTema << std::endl;
            proxIdTema++;
        }
        else if (comando == "U")
        {
            // Usuário: U <nome_usuario> <idade_usuario> <id_tema_1> ... <id_tema_k>
            std::string nomeUser;
            int idade;
            ss >> nomeUser >> idade;

            Usuario *novoUser = new Usuario(proxIdUsuario, idade, nomeUser);
            dict->addUser(novoUser);

            // Inserir as arestas entre o usuário e seus temas de interesse no grafo de temas
            int idTema;
            while (ss >> idTema)
            {
                grafoTemas->inserirAresta(proxIdUsuario, idTema);
            }

            std::cout << "U " << proxIdUsuario << std::endl;
            proxIdUsuario++;
        }
        else if (comando == "S")
        {
            // Seguir: S <id_1> <id_2>
            int id1, id2;
            ss >> id1 >> id2;

            grafoSocial->inserirAresta(id1, id2);

            std::cout << "S " << dict->getUser(id1)->getNome() << " "
                      << dict->getUser(id2)->getNome() << std::endl;
        }
        else if (comando == "R")
        {
            // Remoção de seguimento: R <id_1> <id_2>
            int id1, id2;
            ss >> id1 >> id2;

            grafoSocial->removerFollow(id1, id2);

            std::cout << "R " << dict->getUser(id1)->getNome() << " "
                      << dict->getUser(id2)->getNome() << std::endl;
        }
        else if (comando == "LT")
        {
            // Consulta de temas: LT <id_usuario>
            int idUser;
            ss >> idUser;
            grafoTemas->consultaDeTemas(idUser);
        }
        else if (comando == "LC")
        {
            // Consulta de seguidores: LC <id_usuario>
            int idUser;
            ss >> idUser;
            grafoSocial->consultaDeSeguidores(idUser);
        }
        else if (comando == "LS")
        {
            // Consulta de seguidos: LS <id_usuario>
            int idUser;
            ss >> idUser;
            grafoSocial->consultaDeSeguidos(idUser);
        }
        else if (comando == "LA")
        {
            // Consulta de amigos: LA <id_usuario>
            int idUser;
            ss >> idUser;
            grafoSocial->consultaDeAmigos(idUser);
        }
        else if (comando == "Q")
        {
            // Consulta de relação: Q <id_1> <id_2>
            int id1, id2;
            ss >> id1 >> id2;
            grafoSocial->checaRelacao(id1, id2);
        }
        else if (comando == "G")
        {
            // Consulta de interesse: G <id_usuario> <id_tema>
            int idUser, idTema;
            ss >> idUser >> idTema;
            grafoTemas->checaInteresse(idUser, idTema);
        }
        else if (comando == "F")
        {
            // Consulta de popularidade: F <id_tema>
            int idTema;
            ss >> idTema;
            grafoTemas->consultaPopularidade(idTema);
        }
    }

    // Liberação de memória
    delete grafoSocial;
    delete grafoTemas;
    delete dict;

    return 0;
}