#ifndef _ZGAME_COMMAND_DELEGATES_H
#define _ZGAME_COMMAND_DELEGATES_H

#include <vector>

#include <Ogre.h>
#include "fastdelegate/FastDelegate.h"

namespace ZGame
{
    namespace COMMAND
    {
        //typedef fastdelegate::FastDelegate1<bool(const Ogre::StringVector &params)> ConsoleCommand;
        typedef fastdelegate::FastDelegate<bool(const Ogre::StringVector &params) > ConsoleCommand;
        //typedef fastdelegate::FastDelegate1<bool(const Ogre::StringVector &params) > ConsoleCommand;
        //typedef fastdelegate::FastDelegate1<const Ogre::StringVector &params, bool> ConsoleCommand;
        //typedef fd::delegate<int(const Ogre::StringVector &params)> ConsoleCommandIntRt;
    }
}

#endif