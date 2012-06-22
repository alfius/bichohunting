#include "HighScore.h"
#include <string.h>

HighScore::HighScore(const char* name, int score)
{
    strcpy(_name, name);
    _score = score;
}

const char* HighScore::Name()
{
    return _name;
}

int HighScore::Score()
{
    return _score;
}

