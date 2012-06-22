#ifndef CONTROLLER
#define CONTROLLER

#include <windows.h>

#include "HighScores.h"
#include "Ball.h"

#define CH_HIT      0
#define CH_HITNONE  1
#define CH_SKULL    2

using namespace std;

class Controller
{
    Ball** _balls;
    int _ballCount;
    int _movingBallCount;

    int _width;
    int _height;

    int _lives;
    int _level;
    int _score;
    int _speed;

    HighScores* _highScores;

    void DestroyBallArray();
public:
    Controller();
    ~Controller();
    bool Move();
    void ScreenSize(int width, int height);
    void PrepareLevel(int level);
    int Lives();
    int Score();
    int RemainingBalls();
    int Level();
    int Hit(int x, int y, char* score);
    Ball* BallAt(int);
    int BallCount();
    bool CheckLastScore();
    void CommitLastScore(const char* name);
    list<HighScore*>* HighScoresList();
};

#endif
