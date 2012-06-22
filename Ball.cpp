#include "Ball.h"
#include <cstdlib>

Ball::Ball(int screenWidth, int screenHeight, int type, int maxSpeed)
{
    _animationStep = 0;

    switch (type)
    {
        case BT_NORMAL:
            _color = rand()%BC_COLOR_COUNT;
            _size = BSIZEBIG;
            break;
        case BT_FIRE:
            _color = BC_ORANGE;
            _size = BSIZESMALL;
            break;
        case BT_SKULL:
            _color = BC_SKULL;
            _size = BSIZEBIG;
            break;
    }

    _x = rand()%(screenWidth - _size);
    _y = rand()%(screenHeight - _size);

    _dx = (rand()%(2*maxSpeed + 1)) - maxSpeed;
    _dy = (rand()%(2*maxSpeed + 1)) - maxSpeed;
    if (_dx < 0)
        _status = BS_MOVING_LEFT;
    else _status = BS_MOVING_RIGHT;
}

void Ball::Move()
{
    _x += _dx;
    _y += _dy;
}

void Ball::InvertX()
{
    _dx = -_dx;
    if (_status == BS_MOVING_RIGHT)
        _status = BS_MOVING_LEFT;
    else if (_status == BS_MOVING_LEFT)
        _status = BS_MOVING_RIGHT;
}

void Ball::InvertY()
{
    _dy = -_dy;
}

int Ball::X1()
{
    return _x;
}

int Ball::Y1()
{
    return _y;
}

int Ball::X2()
{
    return _x + _size;
}

int Ball::Y2()
{
    return _y + _size;
}

int Ball::Size()
{
    return _size;
}

int Ball::Status()
{
    return _status;
}

int Ball::AnimationStep()
{
    return 1+(++_animationStep)%3;
}

int Ball::Color()
{
    return _color;
}

int Ball::Score()
{
    int dx = abs(_dx);
    int dy = abs(_dy);
    int speed = dx < dy ? dy : dx;
    int factor = _size == BSIZEBIG ? 5 : 20;

    return factor*speed;
}

void Ball::Freeze()
{
    if (_color != BC_SKULL && (_status == BS_MOVING_LEFT || _status == BS_MOVING_RIGHT))
    {
        _status = BS_FROZEN;
        _dx = _dy = 0;
    }
}

void Ball::Remove()
{
    _status = BS_INVISIBLE;
}

void Ball::SimulateMove(int &x1, int &y1, int &x2, int &y2)
{
    x1 = _x + _dx;
    y1 = _y + _dy;
    x2 = x1 + _size;
    y2 = y1 + _size;
}

bool Ball::Hittest(int x, int y)
{
    if ((X1() <= x) && (X2() >= x))
        return true;
    if ((Y1() <= y) && (Y2() >= y))
        return true;

    return false;
}
