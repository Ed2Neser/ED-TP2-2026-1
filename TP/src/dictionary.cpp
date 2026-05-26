#include "dictionary.hpp"

Dictionary::Dictionary()
{
    maxTemas = 10;
    maxUsuarios = 10;

    Temas = new Tema *[maxTemas]();
    Usuarios = new Usuario *[maxUsuarios]();

    socialHeads = new MainNode *[maxUsuarios]();
    themeUserHeads = new MainNode *[maxUsuarios]();
    themeHeads = new MainNode *[maxTemas]();
}

Dictionary::~Dictionary()
{
    // Libertar os objetos Usuários e seus Heads
    for (int i = 0; i < maxUsuarios; ++i)
    {
        if (Usuarios[i] != nullptr)
        {
            delete Usuarios[i];
            delete socialHeads[i];
            delete themeUserHeads[i];
        }
    }
    // Libertar os objetos Temas e seus Heads
    for (int i = 0; i < maxTemas; ++i)
    {
        if (Temas[i] != nullptr)
        {
            delete Temas[i];
            delete themeHeads[i];
        }
    }

    // Libertar os próprios arrays de ponteiros
    delete[] Temas;
    delete[] Usuarios;
    delete[] socialHeads;
    delete[] themeUserHeads;
    delete[] themeHeads;
}

void Dictionary::addUser(Usuario *user)
{
    int id = user->getId();

    // Lógica de Redimensionamento Dinâmico caso exceda o tamanho
    if (id >= maxUsuarios)
    {
        int novoMax = maxUsuarios * 2;
        if (id >= novoMax)
            novoMax = id + 1;

        Usuario **novoUsuarios = new Usuario *[novoMax]();
        MainNode **novoSocialHeads = new MainNode *[novoMax]();
        MainNode **novoThemeUserHeads = new MainNode *[novoMax]();

        for (int i = 0; i < maxUsuarios; ++i)
        {
            novoUsuarios[i] = Usuarios[i];
            novoSocialHeads[i] = socialHeads[i];
            novoThemeUserHeads[i] = themeUserHeads[i];
        }

        delete[] Usuarios;
        delete[] socialHeads;
        delete[] themeUserHeads;

        Usuarios = novoUsuarios;
        socialHeads = novoSocialHeads;
        themeUserHeads = novoThemeUserHeads;
        maxUsuarios = novoMax;
    }

    Usuarios[id] = user;

    // Criar cabeças de lista correspondentes
    socialHeads[id] = new MainNode(id, 'U');
    themeUserHeads[id] = new MainNode(id, 'U');
}

void Dictionary::addTheme(Tema *theme)
{
    int id = theme->getId();

    if (id >= maxTemas)
    {
        int novoMax = maxTemas * 2;
        if (id >= novoMax)
            novoMax = id + 1;

        Tema **novoTemas = new Tema *[novoMax]();
        MainNode **novoThemeHeads = new MainNode *[novoMax]();

        for (int i = 0; i < maxTemas; ++i)
        {
            novoTemas[i] = Temas[i];
            novoThemeHeads[i] = themeHeads[i];
        }

        delete[] Temas;
        delete[] themeHeads;

        Temas = novoTemas;
        themeHeads = novoThemeHeads;
        maxTemas = novoMax;
    }

    Temas[id] = theme;

    themeHeads[id] = new MainNode(id, 'T');
}

Usuario *Dictionary::getUser(int id) { return (id >= 0 && id < maxUsuarios) ? Usuarios[id] : nullptr; }
Tema *Dictionary::getTheme(int id) { return (id >= 0 && id < maxTemas) ? Temas[id] : nullptr; }

MainNode *Dictionary::getSocialHead(int id) { return (id >= 0 && id < maxUsuarios) ? socialHeads[id] : nullptr; }
MainNode *Dictionary::getThemeUserHead(int id) { return (id >= 0 && id < maxUsuarios) ? themeUserHeads[id] : nullptr; }
MainNode *Dictionary::getThemeHead(int id) { return (id >= 0 && id < maxTemas) ? themeHeads[id] : nullptr; }

//