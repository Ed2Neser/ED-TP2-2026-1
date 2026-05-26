#include "graph.hpp"
#include <iostream>

Grafo::Grafo(char mode, bool direcionado, Dictionary *dict)
{
    this->mode = mode;
    this->direcionado = direcionado;
    this->dict = dict;

    this->numLinhas = 10;
    this->numColunas = 10;

    // Alocar a matriz inicial
    matrizAdj = new int *[numLinhas];
    for (int i = 0; i < numLinhas; ++i)
    {
        matrizAdj[i] = new int[numColunas](); // O () inicializa os inteiros a 0
    }
}

void Grafo::limparListaAdj()
{
    int id = 0;
    while (true)
    {
        MainNode *sh = dict->getSocialHead(id);
        if (sh == nullptr)
            break; // Acabaram os usuários criados

        // Limpar Eixo X do Social (Seguidos)
        MainNode *currX = sh->eixoX;
        while (currX != nullptr)
        {
            MainNode *temp = currX;
            currX = currX->eixoX;
            delete temp;
        }
        sh->eixoX = nullptr;

        // Limpar Eixo Y do Social (Seguidores)
        MainNode *currY = sh->eixoY;
        while (currY != nullptr)
        {
            MainNode *temp = currY;
            currY = currY->eixoY;
            delete temp;
        }
        sh->eixoY = nullptr;

        // Limpar Eixo X de Temas associado a esse Usuário
        MainNode *tuh = dict->getThemeUserHead(id);
        if (tuh != nullptr)
        {
            MainNode *currTuh = tuh->eixoX;
            while (currTuh != nullptr)
            {
                MainNode *temp = currTuh;
                currTuh = currTuh->eixoX;
                delete temp;
            }
            tuh->eixoX = nullptr;
        }
        id++;
    }

    // Limpeza de temas (Grafo de Temas - Lado Tema)
    id = 0;
    while (true)
    {
        MainNode *th = dict->getThemeHead(id);
        if (th == nullptr)
            break; // Acabaram os temas

        // Em grafo de temas, os usuários interessados ficam no eixoX do tema
        MainNode *currX = th->eixoX;
        while (currX != nullptr)
        {
            MainNode *temp = currX;
            currX = currX->eixoX;
            delete temp;
        }
        th->eixoX = nullptr;
        id++;
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

    // Destruir as listas de adjacências, caso estejam em uso
    limparListaAdj();
}

// Método auxiliar interno para controle de memória da matriz, redimensionando conforme necessário para acomodar novos IDs.
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

void Grafo::setMode(char newMode)
{
    if (this->mode == newMode)
        return;

    this->mode = newMode;
    // Opcionalmente, pode-se iterar pelas estruturas aqui para copiar
    // os dados da lista para matriz ou vice-versa, permitindo troca em execução sem perda de dados
    // de acordo com as especificações exigidas na migração "A M" e "A L" do projeto.
}

void inserirOrdenado(MainNode *head, MainNode *novoNo, bool usaEixoY = false)
{
    if (head == nullptr || novoNo == nullptr)
        return; // Segurança extra

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

void Grafo::inserirAresta(int idHead, int idNew)
{
    if (mode == 'M')
    {
        redimensionarMatriz(numLinhas, numColunas, matrizAdj, idHead, idNew);

        // No grafo social: idHead segue idNew. No Temas: idHead interessa por idNew.
        matrizAdj[idHead][idNew] = 1;

        // Se fosse não-direcionado na mesma dimensão, adicionar matrizAdj[idNew][idHead] = 1.
        // Como o Grafo de temas é bipartido, mantemos Linha=Usuario, Coluna=Tema na matriz.
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
        for (int j = 0; j < numColunas; ++j)
        {
            if (matrizAdj[headId][j] == 1)
            {
                std::cout << " " << dict->getUser(j)->getNome();
            }
        }
    }
    else
    {
        MainNode *head = dict->getSocialHead(headId);
        std::cout << "LC " << dict->getUser(headId)->getNome();
        MainNode *curr = head->eixoX;
        while (curr != nullptr)
        {
            std::cout << " " << dict->getUser(curr->id)->getNome();
            curr = curr->eixoX;
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
            if (matrizAdj[j][headId] == 1)
            {
                std::cout << " " << dict->getUser(j)->getNome();
            }
        }
    }
    else
    {
        MainNode *head = dict->getSocialHead(headId);
        std::cout << "LS " << dict->getUser(headId)->getNome();
        MainNode *curr = head->eixoY;
        while (curr != nullptr)
        {
            std::cout << " " << dict->getUser(curr->id)->getNome();
            curr = curr->eixoY;
        }
        std::cout << std::endl;
    }
}
void Grafo::consultaDeAmigos(int headId)
{
    if (mode == 'M')
    {
        std::cout << "LA " << dict->getUser(headId)->getNome();
        for (int j = 0; j < numColunas; ++j)
        {
            if ((matrizAdj[j][headId] == 1) && (matrizAdj[headId][j] == 1))
            {
                std::cout << " " << dict->getUser(j)->getNome();
            }
        }
    }
}
void Grafo::checaRelacao(int idUser1, int idUser2)
{
    if (mode == 'M')
    {
        bool user1segue2 = matrizAdj[idUser1][idUser2];
        bool user2segue1 = matrizAdj[idUser2][idUser1];

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

        curr = head1->eixoY;
        while (curr != nullptr)
        {
            if (curr->id == idUser1)
            {
                user2segue1 = true;
                break;
            }
            curr = curr->eixoY;
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
        bool interesse = matrizAdj[idUser][idTema];
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
            if (matrizAdj[i][idTema] == 1)
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