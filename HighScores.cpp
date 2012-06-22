#include "HighScores.h"
#include "RegistryKey.h"

HighScores::HighScores()
{
     _list = new list<HighScore*>();

     LoadHighScores();
}

HighScores::~HighScores()
{
     delete _list;
}

void HighScores::SaveHighScores()
{
    RegistryKey* _keyCurrentUser = RegistryKey::CurrentUser();
    RegistryKey* _keySoftware = _keyCurrentUser->OpenKey("Software");
    RegistryKey* _keyAlfonsoSoftware = _keySoftware->OpenKey("AlfonsoSoftware");
    if (_keyAlfonsoSoftware == 0)
    {
        _keyAlfonsoSoftware = _keySoftware->CreateKey("AlfonsoSoftware");
    }
    RegistryKey* _keyBichoHunting = _keyAlfonsoSoftware->OpenKey("BichoHunting");
    if (_keyBichoHunting == 0)
    {
        _keyBichoHunting = _keyAlfonsoSoftware->CreateKey("BichoHunting");
    }

    list<HighScore*>::iterator it = _list->begin();
    int i = 1;
    while (i<6)
    {
        if (it!=_list->end())
        {
            char hs[3];
            itoa(i, hs, 10);
            strcat(hs,"i");
            _keyBichoHunting->SetLong(hs, (*it)->Score());
            hs[1] = 's';
            _keyBichoHunting->SetString(hs, (*it)->Name());
            hs[1] = 'l';
            _keyBichoHunting->SetLong(hs, strlen((*it)->Name())+1);
            it++;
        }
        else
        {
            char hs[3];
            itoa(i, hs, 10);
            strcat(hs,"i");
            _keyBichoHunting->SetLong(hs, 0);
            hs[1] = 's';
            _keyBichoHunting->SetString(hs, "");
            hs[1] = 'l';
            _keyBichoHunting->SetLong(hs, 1);
        }
        i++;
    }

    delete _keyBichoHunting;
    delete _keyAlfonsoSoftware;
    delete _keySoftware;
    delete _keyCurrentUser;
}

void HighScores::LoadHighScores()
{
    RegistryKey* _keyCurrentUser = RegistryKey::CurrentUser();
    RegistryKey* _keySoftware = _keyCurrentUser->OpenKey("Software");
    RegistryKey* _keyAlfonsoSoftware = _keySoftware->OpenKey("AlfonsoSoftware");
    if (_keyAlfonsoSoftware == 0)
    {
        LoadEmptyHighScores();
        return;
    }
    RegistryKey* _keyBichoHunting = _keyAlfonsoSoftware->OpenKey("BichoHunting");
    if (_keyBichoHunting == 0)
    {
        LoadEmptyHighScores();
        return;
    }

    int i = 1;
    while (i<6)
    {
        char hs[3];
        itoa(i, hs, 10);
        char name[25];
        name[0] = '\0';
        strcat(hs, "l");
        int lenght = _keyBichoHunting->GetLong(hs);
        hs[1] = 's';
        _keyBichoHunting->GetString(hs, name, lenght);
        hs[1] = 'i';
        int score = _keyBichoHunting->GetLong(hs);
        _list->push_back(new HighScore(name, score));
        i++;
    }

    delete _keyBichoHunting;
    delete _keyAlfonsoSoftware;
    delete _keySoftware;
    delete _keyCurrentUser;
}

void HighScores::LoadEmptyHighScores()
{
    for (int i = 0; i < 5; i++)
        _list->push_front(new HighScore("",0));
}

bool HighScores::CheckHighScore(int score)
{
    return (score > _list->back()->Score());
}

void HighScores::AddHighScore(const char* name, int score)
{
    list<HighScore*>::iterator it = _list->end();
    while (--it != _list->begin() && (score > (*it)->Score()));
    if (it == _list->begin())
    {
        if (score <= (*it)->Score())
            it++;
    }
    else it++;

    _list->insert(it, new HighScore(name, score));
    it = _list->end();
    it--;
    _list->erase(it);

    SaveHighScores();
}

list<HighScore*>* HighScores::List()
{
    return _list;
}
