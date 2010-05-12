#ifndef _ZGAME_CMDCONTROLLER_H
#define _ZGAME_CMDCONTROLLER_H

#include <boost\shared_ptr.hpp>

#include <Ogre.h>
#include <string>

#include "fastdelegate/fastdelegate.h"
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
    }

    using COMMAND::CommandList;

    class OgreConsole;
    using COMMAND::COMMAND_KEY;
    using fastdelegate::DelegateMemento;
    using ZGame::COMMAND::Command;
    using boost::shared_ptr;
    class CommandController : public Ogre::Singleton<CommandController>
    {
    public:
        CommandController();
        ~CommandController();

        bool init();
        //void addCommandX(Ogre::String cmdName, COMMAND::ConsoleCommand &cmd);
        void addCommand(shared_ptr<Command> cmd);
        /** \brief This method is used to add a String command. It is used for String Command only. This is done to minimize coupling.*/
        void addCommand(const COMMAND_KEY &key, const DelegateMemento &memento);
        /** \brief This is a string command specific add command method. This is done to decrease coupling.*/
        void attachConsole(std::auto_ptr<OgreConsole> theConsole);
        void onDestroy();

        OgreConsole* getConsole(); //Precondition: _console must valid!!!
        
        //const CommandList* getCommandList(){ return _commandList;}

        /** \brief This method is called from OgreConsole to directly execute a command from the console.*/
        //static void execute(const Ogre::StringVector &params); 
        //void execute(std::vector<Ogre::String> &params);
        //void executeCmd(const Ogre::StringVector &params);
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
        
    };
}


#endif