#ifndef _ZGAME_COMMAND_DELEGATES_H
#define _ZGAME_COMMAND_DELEGATES_H

#include <vector>

#include <Ogre.h>
#include "fastdelegate/delegate.h"

namespace ZGame
{
  namespace COMMAND
  {
    typedef fd::delegate<bool(const Ogre::StringVector &params)> ConsoleCommand;
  }
}

#endif