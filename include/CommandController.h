#ifndef _ZGAME_CMDCONTROLLER_H
#define _ZGAME_CMDCONTROLLER_H

#include <memory>
#include <Ogre.h>
#include <string>
#include "Command.h"
#include "EngineView.h"
#include "Controller.h"
#include "CommandDelegates.h"
namespace ZGame
{
    namespace COMMAND
    {
        static const Ogre::String CHARLIST("charlist");
        static const Ogre::String CHARLISTMESHES("charlistmeshes");
        static const Ogre::String CHARCREATE("charcreate");
        static const Ogre::String NODELIST("nodelist");
        static const Ogre::String NODEREMOVE("removenode");
    }
    class OgreConsole;
    /*
    namespace Util
    {
    class CharacterUtil;
    }*/

    //class ControlModuleProto;

    class CommandController : public Ogre::Singleton<CommandController>
    {
    public:
        CommandController();
        ~CommandController();

        bool init();
        void addCommand(Ogre::String cmdName, COMMAND::ConsoleCommand &cmd);

        static void execute(const Ogre::StringVector &params);
        //void execute(std::vector<Ogre::String> &params);
        void executeCmd(const Ogre::StringVector &params);

        static ZGame::CommandController&
            getSingleton();
        static ZGame::CommandController*
            getSingletonPtr();
    protected:

    private:
    };
}


#endif