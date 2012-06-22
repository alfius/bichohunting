#include "ControllerFactory.h"

Controller* ControllerFactory::_controller = 0;

Controller* ControllerFactory::Current()
{
    if (_controller == 0)
    {
        _controller = new Controller();
    }
    return _controller;
}

void ControllerFactory::Delete()
{
    delete _controller;
}
