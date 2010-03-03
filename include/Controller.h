#ifndef _ZGAME_CONTROLLER_H
#define _ZGAME_CONTROLLER_H

#include "Command.h"

namespace ZGame
{
  class Controller
  {
  public:
    ~Controller(){}
    virtual int execute(const ZGame::Command&) = 0; //pure virtual
    virtual int blah(const ZGame::Command&) = 0;//Do not need these.
  protected:
    Controller(){}
  };
}

#endif