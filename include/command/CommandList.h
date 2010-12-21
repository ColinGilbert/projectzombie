#ifndef _ZGAME_COMMAND_LIST_H
#define _ZGAME_COMMAND_LIST_H

//#include <Ogre.h>
#include <map>
using std::map;

#include "CommandController.h"

#include "command/Command.h"


namespace ZGame
{
    //class CommandController;

    namespace COMMAND
    {
        //class Command;

        class CommandList
        {
        public:
            friend class ZGame::CommandController;

            
            static const COMMAND::COMMAND_KEY CHARLIST;
            static const COMMAND::COMMAND_KEY CHARLISTMESHES;
            static const COMMAND::COMMAND_KEY CHARCREATE;
            static const COMMAND::COMMAND_KEY NODELIST;
            static const COMMAND::COMMAND_KEY NODEREMOVE;
            //static const COMMAND::COMMAND_KEY CONSOLE_CMD;
            static const COMMAND::COMMAND_KEY CAMERA_ATTACH;
            static const COMMAND::COMMAND_KEY CLIENT_CONNECT;
            static const COMMAND::COMMAND_KEY CLIENT_DISCONNECT;
            static const COMMAND::COMMAND_KEY GRAPHICS_ADJUSTSHADOW;
            static const COMMAND::COMMAND_KEY ENGINECTRL_CMD;
            //static const COMMAND::COMMAND_KEY CREATE_RENDER_ENT;
            //static const Ogre::String NODETEST;
            
            /*
            static const COMMAND::StringCommand CHARLIST;
            static const COMMAND::StringCommand CHARLISTMESHES;
            static const COMMAND::StringCommand CHARCREATE;
            static const COMMAND::StringCommand NODELIST;
            static const COMMAND::StringCommand NODEREMOVE;
            static const COMMAND::StringCommand CONSOLE_CMD;
            */
            //static const COMMAND::NODETEST;
            //static const COMMAND::Command             
            //auto_ptr<Command> getCommandObject(const Ogre::String &cmdStr);
            template<typename T>
            static COMMAND::StringCommand bindStrCommand(const COMMAND::StringCommand &commandPrototype, T &delegate)
            {
                COMMAND::StringCommand command = commandPrototype;
                command.setCommandMemento(delegate.GetMemento());
                return command;
            }

        protected:
            CommandList(){}
            virtual ~CommandList(){}

        };
    }
}



#endif