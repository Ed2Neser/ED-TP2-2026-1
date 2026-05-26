#include "graph.hpp"
#include <iostream>

Grafo::Grafo(char mode, bool direcionado, Dictionary *dict)
{
    this->mode = mode;
    this->direcionado = direcionado;
    this->dict = dict;

    this->numLinhas = 10;
    this->numColunas = 10;

    // Alocar a matriz inicial (usada mesmo no modo L, para facilitar a migração)
    matrizAdj = new int *[numLinhas];
    for (int i = 0; i < numLinhas; ++i)
    {
        matrizAdj[i] = new int[numColunas]();
    }
}

void Grafo::limparListaAdj()
{
    if (direcionado)
    {
        // Grafo Social: limpar eixoX e eixoY de cada socialHead
        int id = 0;
        while (true)
        {
            MainNode *sh = dict->getSocialHead(id);
            if (sh == nullptr)
                break;

            // Limpar Eixo X (Seguidos)
            MainNode *currX = sh->eixoX;
            while (currX != nullptr)
            {
                MainNode *temp = currX;
                currX = currX->eixoX;
                delete temp;
            }
            sh->eixoX = nullptr;

            // Limpar Eixo Y (Seguidores)
            MainNode *currY = sh->eixoY;
            while (currY != nullptr)
            {
                MainNode *temp = currY;
                currY = currY->eixoY;
                delete temp;
            }
            sh->eixoY = nullptr;

            id++;
        }
    }
    else
    {
        // Grafo de Temas: limpar eixoX de cada themeUserHead e themeHead
        int id = 0;
        while (true)
        {
            MainNode *tuh = dict->getThemeUserHead(id);
            if (tuh == nullptr)
                break;

            MainNode *curr = tuh->eixoX;
            while (curr != nullptr)
            {
                MainNode *temp = curr;
                curr = curr->eixoX;
                delete temp;
            }
            tuh->eixoX = nullptr;

            id++;
        }

        id = 0;
        while (true)
        {
            MainNode *th = dict->getThemeHead(id);
            if (th == nullptr)
                break;

            MainNode *curr = th->eixoX;
            while (curr != nullptr)
            {
                MainNode *temp = curr;
                curr = curr->eixoX;
                delete temp;
            }
            th->eixoX = nullptr;

            id++;
        }
    }
}

Grafo::~Grafo()
{
    // Destruir a matriz de adjacências
    for (int i = 0; i < numLinhas; ++i)
    {
        delete[] matrizAdj[i];
    }
    delete[] matrizAdj;

    // Destruir as listas de adjacências
    limparListaAdj();
}

// Função auxiliar para redimensionar a matriz de adjacências
void redimensionarMatriz(int &numLinhas, int &numColunas, int **&matrizAdj, int idHead, int idNew)
{
    int novoLinhas = (idHead >= numLinhas) ? (idHead + 1) * 2 : numLinhas;
    int novoColunas = (idNew >= numColunas) ? (idNew + 1) * 2 : numColunas;

    if (novoLinhas == numLinhas && novoColunas == numColunas)
        return;

    int **novaMatriz = new int *[novoLinhas];
    for (int i = 0; i < novoLinhas; ++i)
    {
        novaMatriz[i] = new int[novoColunas]();
        if (i < numLinhas)
        {
            for (int j = 0; j < numColunas && j < novoColunas; ++j)
            {
                novaMatriz[i][j] = matrizAdj[i][j];
            }
        }
    }

    for (int i = 0; i < numLinhas; ++i)
    {
        delete[] matrizAdj[i];
    }
    delete[] matrizAdj;

    matrizAdj = novaMatriz;
    numLinhas = novoLinhas;
    numColunas = novoColunas;
}

// Função auxiliar para inserção ordenada em lista encadeada
void inserirOrdenado(MainNode *head, MainNode *novoNo, bool usaEixoY = false)
{
    if (head == nullptr || novoNo == nullptr)
        return;

    MainNode *atual = head;
    if (!usaEixoY)
    {
        while (atual->eixoX != nullptr && atual->eixoX->id < novoNo->id)
        {
            atual = atual->eixoX;
        }
        novoNo->eixoX = atual->eixoX;
        atual->eixoX = novoNo;
    }
    else
    {
        while (atual->eixoY != nullptr && atual->eixoY->id < novoNo->id)
        {
            atual = atual->eixoY;
        }
        novoNo->eixoY = atual->eixoY;
        atual->eixoY = novoNo;
    }
}

void Grafo::setMode(char newMode)
{
    if (this->mode == newMode)
        return;

    if (newMode == 'M')
    {
        // =============================================
        // MIGRAÇÃO: Lista → Matriz
        // =============================================
        // Percorrer as listas de adjacência e preencher a matriz.
        // A matriz já existe (alocada no construtor), mas pode precisar
        // ser redimensionada e deve ser zerada antes de preencher.

        if (direcionado)
        {
            // Grafo Social: percorrer socialHeads, ler eixoX (seguidos)
            // matrizAdj[seguidor][seguido] = 1
            int id = 0;
            while (true)
            {
                MainNode *sh = dict->getSocialHead(id);
                if (sh == nullptr)
                    break;

                MainNode *curr = sh->eixoX;
                while (curr != nullptr)
                {
                    // Garantir que a matriz comporta os IDs
                    redimensionarMatriz(numLinhas, numColunas, matrizAdj, id, curr->id);
                    matrizAdj[id][curr->id] = 1;
                    curr = curr->eixoX;
                }
                id++;
            }
        }
        else
        {
            // Grafo de Temas: percorrer themeUserHeads, ler eixoX (temas do user)
            // matrizAdj[usuario][tema] = 1
            int id = 0;
            while (true)
            {
                MainNode *tuh = dict->getThemeUserHead(id);
                if (tuh == nullptr)
                    break;

                MainNode *curr = tuh->eixoX;
                while (curr != nullptr)
                {
                    redimensionarMatriz(numLinhas, numColunas, matrizAdj, id, curr->id);
                    matrizAdj[id][curr->id] = 1;
                    curr = curr->eixoX;
                }
                id++;
            }
        }

        // Limpar as listas de adjacência (liberar memória dos nós encadeados)
        limparListaAdj();
    }
    else if (newMode == 'L')
    {
        // =============================================
        // MIGRAÇÃO: Matriz → Lista
        // =============================================
        // Percorrer a matriz e reconstruir as listas de adjacência.

        if (direcionado)
        {
            // Grafo Social: matrizAdj[i][j] == 1 significa i segue j
            for (int i = 0; i < numLinhas; ++i)
            {
                MainNode *sh = dict->getSocialHead(i);
                if (sh == nullptr)
                    continue;

                for (int j = 0; j < numColunas; ++j)
                {
                    if (matrizAdj[i][j] == 1)
                    {
                        // i segue j: inserir j no eixoX de i, inserir i no eixoY de j
                        MainNode *novoX = new MainNode(j, 'U');
                        inserirOrdenado(sh, novoX, false);

                        MainNode *shJ = dict->getSocialHead(j);
                        if (shJ != nullptr)
                        {
                            MainNode *novoY = new MainNode(i, 'U');
                            inserirOrdenado(shJ, novoY, true);
                        }
                    }
                }
            }
        }
        else
        {
            // Grafo de Temas: matrizAdj[user][tema] == 1
            for (int i = 0; i < numLinhas; ++i)
            {
                MainNode *tuh = dict->getThemeUserHead(i);
                if (tuh == nullptr)
                    continue;

                for (int j = 0; j < numColunas; ++j)
                {
                    if (matrizAdj[i][j] == 1)
                    {
                        // Inserir tema j no eixoX do user i
                        MainNode *novoTema = new MainNode(j, 'T');
                        inserirOrdenado(tuh, novoTema, false);

                        // Inserir user i no eixoX do tema j
                        MainNode *th = dict->getThemeHead(j);
                        if (th != nullptr)
                        {
                            MainNode *novoUser = new MainNode(i, 'U');
                            inserirOrdenado(th, novoUser, false);
                        }
                    }
                }
            }
        }

        // Zerar a matriz (os dados agora estão nas listas)
        for (int i = 0; i < numLinhas; ++i)
        {
            for (int j = 0; j < numColunas; ++j)
            {
                matrizAdj[i][j] = 0;
            }
        }
    }

    this->mode = newMode;
}

void Grafo::inserirAresta(int idHead, int idNew)
{
    if (mode == 'M')
    {
        redimensionarMatriz(numLinhas, numColunas, matrizAdj, idHead, idNew);

        // No grafo social: idHead segue idNew. No Temas: idHead interessa por idNew.
        matrizAdj[idHead][idNew] = 1;
    }
    else if (mode == 'L')
    {
        MainNode *headSource = nullptr;

        if (!direcionado)
        {
            // Grafo de Temas (Não direcionado e bipartido)
            // 1. Inserção ordenada do Tema no eixo X do Usuário
            headSource = dict->getThemeUserHead(idHead);
            MainNode *novoTema = new MainNode(idNew, 'T');
            inserirOrdenado(headSource, novoTema, false);

            // 2. Inserção ordenada do Usuário no eixo X do Tema
            headSource = dict->getThemeHead(idNew);
            MainNode *novoUser = new MainNode(idHead, 'U');
            inserirOrdenado(headSource, novoUser, false);
        }
        else
        {
            // Grafo Social (Direcionado)
            // 1. Inserção ordenada de quem está sendo seguido (Eixo X)
            headSource = dict->getSocialHead(idHead);
            MainNode *novoX = new MainNode(idNew, 'U');
            inserirOrdenado(headSource, novoX, false);

            // 2. Inserção ordenada do seguidor na lista de quem foi seguido (Eixo Y)
            headSource = dict->getSocialHead(idNew);
            MainNode *novoY = new MainNode(idHead, 'U');
            inserirOrdenado(headSource, novoY, true);
        }
    }
}

void Grafo::removerFollow(int idSeguidor, int idSeguido)
{
    if (mode == 'M')
    {
        if (idSeguidor < numLinhas && idSeguido < numColunas)
        {
            matrizAdj[idSeguidor][idSeguido] = 0;
        }
    }
    else if (mode == 'L')
    {
        // Remover idSeguido do eixoX de idSeguidor
        MainNode *headSource = dict->getSocialHead(idSeguidor);
        while (headSource != nullptr && headSource->eixoX != nullptr)
        {
            if (headSource->eixoX->id == idSeguido)
            {
                MainNode *temp = headSource->eixoX;
                headSource->eixoX = temp->eixoX;
                delete temp;
                break;
            }
            headSource = headSource->eixoX;
        }

        // Remover idSeguidor do eixoY de idSeguido
        MainNode *headTarget = dict->getSocialHead(idSeguido);
        while (headTarget != nullptr && headTarget->eixoY != nullptr)
        {
            if (headTarget->eixoY->id == idSeguidor)
            {
                MainNode *temp = headTarget->eixoY;
                headTarget->eixoY = temp->eixoY;
                delete temp;
                break;
            }
            headTarget = headTarget->eixoY;
        }
    }
}

void Grafo::consultaDeTemas(int headId)
{
    if (mode == 'M')
    {
        std::cout << "LT " << dict->getUser(headId)->getNome();
        for (int j = 0; j < numColunas; ++j)
        {
            if (matrizAdj[headId][j] == 1)
            {
                std::cout << " " << dict->getTheme(j)->getNome();
            }
        }
        std::cout << std::endl;
    }
    else
    {
        MainNode *head = dict->getThemeUserHead(headId);
        std::cout << "LT " << dict->getUser(headId)->getNome();
        MainNode *curr = head->eixoX;
        while (curr != nullptr)
        {
            std::cout << " " << dict->getTheme(curr->id)->getNome();
            curr = curr->eixoX;
        }
        std::cout << std::endl;
    }
}

void Grafo::consultaDeSeguidores(int headId)
{
    if (mode == 'M')
    {
        std::cout << "LC " << dict->getUser(headId)->getNome();
        for (int j = 0; j < numLinhas; ++j)
        {
            if (matrizAdj[j][headId] == 1)
            {
                std::cout << " " << dict->getUser(j)->getNome();
            }
        }
        std::cout << std::endl;
    }
    else
    {
        MainNode *head = dict->getSocialHead(headId);
        std::cout << "LC " << dict->getUser(headId)->getNome();
        MainNode *curr = head->eixoY;
        while (curr != nullptr)
        {
            std::cout << " " << dict->getUser(curr->id)->getNome();
            curr = curr->eixoY;
        }
        std::cout << std::endl;
    }
}

void Grafo::consultaDeSeguidos(int headId)
{
    if (mode == 'M')
    {
        std::cout << "LS " << dict->getUser(headId)->getNome();
        for (int j = 0; j < numColunas; ++j)
        {
            if (matrizAdj[headId][j] == 1)
            {
                std::cout << " " << dict->getUser(j)->getNome();
            }
        }
        std::cout << std::endl;
    }
    else
    {
        MainNode *head = dict->getSocialHead(headId);
        std::cout << "LS " << dict->getUser(headId)->getNome();
        MainNode *curr = head->eixoX;
        while (curr != nullptr)
        {
            std::cout << " " << dict->getUser(curr->id)->getNome();
            curr = curr->eixoX;
        }
        std::cout << std::endl;
    }
}

void Grafo::consultaDeAmigos(int headId)
{
    if (mode == 'M')
    {
        std::cout << "LA " << dict->getUser(headId)->getNome();
        for (int j = 0; j < numLinhas; ++j)
        {
            if (j < numColunas && headId < numLinhas)
            {
                if ((matrizAdj[j][headId] == 1) && (matrizAdj[headId][j] == 1))
                {
                    std::cout << " " << dict->getUser(j)->getNome();
                }
            }
        }
        std::cout << std::endl;
    }
    else
    {
        MainNode *head = dict->getSocialHead(headId);
        std::cout << "LA " << dict->getUser(headId)->getNome();
        MainNode *currX = head->eixoX;
        while (currX != nullptr)
        {
            MainNode *currY = head->eixoY;
            while (currY != nullptr)
            {
                if (currX->id == currY->id)
                {
                    std::cout << " " << dict->getUser(currX->id)->getNome();
                    break;
                }
                currY = currY->eixoY;
            }
            currX = currX->eixoX;
        }
        std::cout << std::endl;
    }
}

void Grafo::checaRelacao(int idUser1, int idUser2)
{
    if (mode == 'M')
    {
        bool user1segue2 = (idUser1 < numLinhas && idUser2 < numColunas) && matrizAdj[idUser1][idUser2];
        bool user2segue1 = (idUser2 < numLinhas && idUser1 < numColunas) && matrizAdj[idUser2][idUser1];

        std::cout << "Q " << dict->getUser(idUser1)->getNome() << " " << dict->getUser(idUser2)->getNome();

        if (user1segue2 && user2segue1)
            std::cout << " 3" << std::endl;
        else if (user1segue2)
            std::cout << " 1" << std::endl;
        else if (user2segue1)
            std::cout << " 2" << std::endl;
        else
            std::cout << " 0" << std::endl;
    }
    else
    {
        MainNode *head1 = dict->getSocialHead(idUser1);
        MainNode *head2 = dict->getSocialHead(idUser2);

        bool user1segue2 = false;
        bool user2segue1 = false;

        MainNode *curr = head1->eixoX;
        while (curr != nullptr)
        {
            if (curr->id == idUser2)
            {
                user1segue2 = true;
                break;
            }
            curr = curr->eixoX;
        }

        curr = head2->eixoX;
        while (curr != nullptr)
        {
            if (curr->id == idUser1)
            {
                user2segue1 = true;
                break;
            }
            curr = curr->eixoX;
        }

        std::cout << "Q " << dict->getUser(idUser1)->getNome() << " " << dict->getUser(idUser2)->getNome();

        if (user1segue2 && user2segue1)
            std::cout << " 3" << std::endl;
        else if (user1segue2)
            std::cout << " 1" << std::endl;
        else if (user2segue1)
            std::cout << " 2" << std::endl;
        else
            std::cout << " 0" << std::endl;
    }
}

void Grafo::checaInteresse(int idUser, int idTema)
{
    if (mode == 'M')
    {
        bool interesse = (idUser < numLinhas && idTema < numColunas) && matrizAdj[idUser][idTema];
        std::cout << "G " << dict->getUser(idUser)->getNome() << " " << dict->getTheme(idTema)->getNome() << (interesse ? " 1" : " 0") << std::endl;
    }
    else
    {
        MainNode *head = dict->getThemeUserHead(idUser);
        bool interesse = false;
        MainNode *curr = head->eixoX;
        while (curr != nullptr)
        {
            if (curr->id == idTema)
            {
                interesse = true;
                break;
            }
            curr = curr->eixoX;
        }
        std::cout << "G " << dict->getUser(idUser)->getNome() << " " << dict->getTheme(idTema)->getNome() << (interesse ? " 1" : " 0") << std::endl;
    }
}

void Grafo::consultaPopularidade(int idTema)
{
    if (mode == 'M')
    {
        int popularidade = 0;
        for (int i = 0; i < numLinhas; ++i)
        {
            if (idTema < numColunas && matrizAdj[i][idTema] == 1)
            {
                popularidade++;
            }
        }
        std::cout << "F " << dict->getTheme(idTema)->getNome() << " " << popularidade << std::endl;
    }
    else
    {
        MainNode *head = dict->getThemeHead(idTema);
        int popularidade = 0;
        MainNode *curr = head->eixoX;
        while (curr != nullptr)
        {
            popularidade++;
            curr = curr->eixoX;
        }
        std::cout << "F " << dict->getTheme(idTema)->getNome() << " " << popularidade << std::endl;
    }
}

//