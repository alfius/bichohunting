#ifndef MOVINGOBJECT
#define MOVINGOBJECT

#define BS_MOVING_RIGHT 0
#define BS_MOVING_LEFT  1
#define BS_FROZEN       2
#define BS_INVISIBLE    3

#define BC_COLOR_COUNT  4   // Normal colors count.

#define BC_GREEN        0
#define BC_BLUE         1
#define BC_RED          2
#define BC_YELLOW       3

#define BC_ORANGE       4
#define BC_SKULL        5

#define BSIZEBIG        64
#define BSIZESMALL      32

#define BT_NORMAL       0
#define BT_FIRE         1
#define BT_SKULL        2

class Ball
{
    int _x;
    int _y;
    int _dx;
    int _dy;
    int _status;
    int _color;
    int _size;
    int _animationStep;
public:
    Ball(int screenWidth, int screenHeight, int type, int maxSpeed);
    void Move();
    void InvertX();
    void InvertY();
    int X1();
    int Y1();
    int X2();
    int Y2();
    int Size();
    int Status();
    int AnimationStep();
    int Color();
    int Score();
    void Freeze();
    void Remove();
    void SimulateMove(int &x1, int &y1, int &x2, int &y2);
    bool Hittest(int x, int y);
};

#endif
