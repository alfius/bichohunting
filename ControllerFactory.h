#ifndef CONTROLLERFACTORY
#define CONTROLLERFACTORY

#include "Controller.h"

class ControllerFactory
{
    static Controller* _controller;
public:
    static Controller* Current();
    static void Delete();
};

#endif
