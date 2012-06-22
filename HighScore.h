#ifndef HIGHSCORE
#define HIGHSCORE

class HighScore
{
    char _name[25];
    int _score;
public:
    HighScore(const char* name, int score);
    const char* Name();
    int Score();
};

#endif
