#include "Controller.h"

Controller::Controller()
{
    _score = 0;
    _balls = 0;
    _highScores = new HighScores();
}

Controller::~Controller()
{
    DestroyBallArray();
}

void Controller::DestroyBallArray()
{
    if (_balls)
    {
        for(int i = 0; i < _ballCount; i++)
        {
            delete _balls[i];
            _balls[i] = 0;
        }
        delete[] _balls;
        _balls = 0;
    }
}

void Controller::PrepareLevel(int level)
{
    _level = level;

    if (_level == 1)
    {
        _lives = 2;
        _score = 0;
    }

    DestroyBallArray();

    _speed = 5*level;

    _ballCount = _speed + 10 + (int)(((double)rand()/(double)RAND_MAX)*(double)30);
    if (_ballCount <= 30)
        _ballCount = 30;

    _balls = new Ball*[_ballCount];

    int skullCount = _level/2;
    if (skullCount >= (int)((double)_ballCount*(double)0.50))
            skullCount = (int)((double)_ballCount*(double)0.50);

    _movingBallCount = _ballCount - skullCount;

    int smallBallsCount = 0;
    if (_level > 2)
    {
        smallBallsCount = (double)(_ballCount-skullCount) * ((double)rand()/(double)RAND_MAX) * ((double)(_level-2)/(double)5);
        if (smallBallsCount >= (int)((double)_ballCount*(double)0.35))
            smallBallsCount = (int)((double)_ballCount*(double)0.35);
    }

    int i;
    for (i = 0; i < skullCount; i++)
    {
        _balls[i] = new Ball(_width, _height, BT_SKULL, _speed);
    }
    for (; i < skullCount + smallBallsCount; i++)
    {
        _balls[i] = new Ball(_width, _height, BT_FIRE, _speed);
    }
    for (; i < _ballCount; i++)
    {
        _balls[i] = new Ball(_width, _height, BT_NORMAL, _speed);
    }
}

int Controller::Lives()
{
    return _lives;
}

int Controller::Score()
{
    return _score;
}

int Controller::RemainingBalls()
{
    return _movingBallCount;
}

int Controller::Level()
{
    return _level;
}

bool Controller::Move()
{
    bool inverted = false;
    for (int i = 0; i < _ballCount; i++)
    {
        int x1, y1, x2, y2;
        _balls[i]->SimulateMove(x1, y1, x2, y2);
        if (x1 < 0 || x2 > _width)
        {
            _balls[i]->InvertX();
            inverted = true;
        }
        if (y1 < 0 || y2 > _height)
        {
            _balls[i]->InvertY();
            inverted = true;
        }

        _balls[i]->Move();
    }
    return inverted;
}

void Controller::ScreenSize(int width, int height)
{
    _width = width;
    _height = height;
}

int Controller::Hit(int x, int y, char* score)
{
    score[0] = '\0';

    POINTS p;
    p.x = x;
    p.y = y;

    int ballsHit = 0;
    int ballsScore = 0;
    int color = -1;
    bool differentColors = false;
    bool skullHit = false;
    for (int i = 0; i < _ballCount; i++)
    {
        if ((_balls[i]->Status() == BS_MOVING_LEFT || _balls[i]->Status() == BS_MOVING_RIGHT) && _balls[i]->Hittest(x,y))
        {
            if (_balls[i]->Color() == BC_SKULL)
                skullHit = true;
            else
            {
                if (color == -1)
                    color = _balls[i]->Color();
                else if (color != _balls[i]->Color())
                    differentColors = true;
                ballsHit++;
                ballsScore += _balls[i]->Score();
                _balls[i]->Freeze();
            }
        }
    }

    if (skullHit)
    {
        _lives--;
        strcpy(score, "0");

        _movingBallCount -= ballsHit;

        if ((_movingBallCount == 0)&&(_level%2))
            _lives++;

        return CH_SKULL;
    }
    else if (ballsHit == 0)
    {
        _lives--;
        strcpy(score, "0");

        return CH_HITNONE;
    }
    else
    {
        double ballsHitProportion = (double)ballsHit/(double)_movingBallCount;
        double bonusFactor = differentColors ? 1 : ballsHit;
        int normalScore = (int)(ballsHitProportion*ballsHitProportion*(double)ballsScore);

        char normalScoreChar[15];
        itoa(normalScore, normalScoreChar, 10);

        if (bonusFactor > 1)
        {
            itoa(bonusFactor, score, 10);
            strcat(score, "x");
            strcat(score, normalScoreChar);
        }
        else
        {
            strcpy(score, normalScoreChar);
        }

        _score += normalScore*bonusFactor;

        _movingBallCount -= ballsHit;

        if ((_movingBallCount == 0)&&(_level%2))
            _lives++;

        return CH_HIT;
    }
}

Ball* Controller::BallAt(int i)
{
    return _balls[i];
}

int Controller::BallCount()
{
    return _ballCount;
}

bool Controller::CheckLastScore()
{
    return _highScores->CheckHighScore(_score);
}

void Controller::CommitLastScore(const char* name)
{
    _highScores->AddHighScore(name, _score);
}

list<HighScore*>* Controller::HighScoresList()
{
    return _highScores->List();
}


