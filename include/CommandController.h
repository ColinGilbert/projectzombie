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
   
    class OgreConsole; 
    /*
    namespace Util
    {
    class CharacterUtil;
    }*/

    //class ControlModuleProto;

    namespace COMMAND
    {       
        class CommandList;
    }

    class OgreConsole;

    class CommandController : public Ogre::Singleton<CommandController>
    {
    public:
        CommandController();
        ~CommandController();

        bool init();
        void addCommand(Ogre::String cmdName, COMMAND::ConsoleCommand &cmd);
        void attachConsole(std::auto_ptr<OgreConsole> theConsole);
        void onDestroy();

        OgreConsole* getConsole(); //Precondition: _console must valid!!!
        
        COMMAND::CommandList* getCommandList(){ return _commandList;}

        static void execute(const Ogre::StringVector &params);
        //void execute(std::vector<Ogre::String> &params);
        void executeCmd(const Ogre::StringVector &params);

        static ZGame::CommandController&
            getSingleton();
        static ZGame::CommandController*
            getSingletonPtr();
    protected:

    private:
        COMMAND::CommandList* _commandList;
        std::auto_ptr<OgreConsole> _console;
        //OgreConsole* _console; //we're going to use an naked pointer here.
    };
}


#endif