#ifndef HIGHSCORES
#define HIGHSCORES

#include "HighScore.h"
#include <list>
using namespace std;

class HighScores
{
    list<HighScore*>* _list;

    void SaveHighScores();
    void LoadHighScores();
    void LoadEmptyHighScores();
public:
    HighScores();
    ~HighScores();
    bool CheckHighScore(int score);
    void AddHighScore(const char* name, int score);
    list<HighScore*>* List();
};

#endif
