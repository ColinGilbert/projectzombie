#ifndef _ZGAME_CMDCONTROLLER_H
#define _ZGAME_CMDCONTROLLER_H

//#include <boost\shared_ptr.hpp>


#include <Ogre.h>

#include <string>

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include <memory>
#else
#include <tr1/memory>
#endif

//test

#include "fastdelegate/FastDelegate.h"
#include "command/Command.h"
#include "EngineView.h"
#include "Controller.h"
#include "CommandDelegates.h"

namespace ZGame
{
   
    
    class OgreConsole; 
    
    namespace COMMAND
    {       
        class CommandList;
        typedef Ogre::String COMMAND_KEY;
        class ServicesManager;
    }

    using COMMAND::CommandList;

    class OgreConsole;
    using COMMAND::COMMAND_KEY;
    using fastdelegate::DelegateMemento;
    using ZGame::COMMAND::Command;
    using std::tr1::shared_ptr;
    class CommandController : public Ogre::Singleton<CommandController>
    {
    public:

        CommandController();
        ~CommandController();

        bool init();
        void addCommand(shared_ptr<Command> cmd);
        /** \brief This method is used to add a String command. It is used for String Command only. This is done to minimize coupling.*/
        void addCommand(const COMMAND_KEY &key, const DelegateMemento &memento);
        /** \brief This is a string command specific add command method. This is done to decrease coupling.*/
        void attachConsole(std::auto_ptr<OgreConsole> theConsole);
        void onDestroy();

        OgreConsole* getConsole(); //Precondition: _console must valid!!!
        
        /** \brief This method is called by anyone to execute a Command object.**/
        fastdelegate::DelegateMemento 
            executeCmd(const Command &cmd);
        static ZGame::CommandController&
            getSingleton();
        static ZGame::CommandController*
            getSingletonPtr();
    protected:

    private:
        typedef std::map<Ogre::String, shared_ptr<Command> > CMD_MAP;
        typedef CMD_MAP::iterator MAP_ITER;
        static CMD_MAP __cmdMap;
        //COMMAND::CommandList* _commandList;
        std::auto_ptr<OgreConsole> _console;
        //OgreConsole* _console; //we're going to use an naked pointer here.
        //Command addCommandFactory(const COMMAND_TYPE &type);
        COMMAND::ServicesManager* _svcManager; //Naked pointer here is fine because it's private and we're going to remember to delete it.
        
    };
}


#endif
