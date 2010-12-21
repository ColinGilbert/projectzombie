#include <iostream>
#include <list>
#include "CommandController.h"
#include "ogreconsole.h"
#include "CommandDelegates.h"
#include "command/CommandList.h"
#include "command/ServicesManager.h"
using ZGame::COMMAND::ServicesManager;
#include "ControlModuleProto.h"
#include "command/CmdException.h"

using namespace ZGame;
using COMMAND::CmdException;

//std::map<Ogre::String,ZGame::COMMAND::ConsoleCommand> __cmdMap;

CommandController::CMD_MAP CommandController::__cmdMap;

template<>
CommandController* Ogre::Singleton<CommandController>::ms_Singleton = 0;

CommandController& CommandController::getSingleton()
{
    return *ms_Singleton;
}

CommandController* CommandController::getSingletonPtr()
{
    return ms_Singleton;
}


CommandController::CommandController()
{
}

CommandController::~CommandController()
{
   
}

/**
** \note you should refactor the way console and command controller are coupled. The problem 
* is that CommandController is a singleton. It shouldn't keep track of state like this. The way
*we're doing it is also because we copy-and-pasta OgreConsole from somewhere. ALL is not lost however,
*since we are planning to refactor OgreConsole using LibRocket anyway. Many new systems has been implemented
*since OgreConsole, so use that (the current system patterns).
**/
void
    CommandController::onDestroy()
{
     if(_console.get() != 0)
        _console->shutdown();
    __cmdMap.clear();
}


void 
CommandController::attachConsole(std::auto_ptr<OgreConsole> theConsole)
{
    _console = theConsole;
}

OgreConsole* 
CommandController::getConsole()
{ 
    return _console.get(); 
}

/**
*This method will execute the command object.
*/
fastdelegate::DelegateMemento 
//CommandController::executeCmd(const Ogre::StringVector &params)
CommandController::executeCmd(const COMMAND::Command &cmd)
{
    fastdelegate::DelegateMemento NULL_MEMENTO;
    try
    {
        //std::map<Ogre::String,ZGame::COMMAND::ConsoleCommand>::iterator cmdMapIter;
        MAP_ITER cmdMapIter;
        cmdMapIter = __cmdMap.find(cmd.getKey());
        if(cmdMapIter == __cmdMap.end())
            throw CmdException("The command you are trying to execute does not exist in __cmdMap. Key: "+cmd.getKey());
        //assert(cmdMapIter != __cmdMap.end() && "The command you are trying to execute does not exist in __cmdMap.");
        __cmdMap[cmd.getKey()]->execute(cmd);
    
    }
    catch(CmdException e)
    {
        throw e;
    }

    return NULL_MEMENTO;
}

/**
*This method will initialize CommandController with an OgreConsole.
*
**/
bool CommandController::init()
{
    return true;
}

void CommandController::addCommand(shared_ptr<COMMAND::Command> cmd)
{
    assert(cmd->getCommandMemento() != 0 && "Class invariant assert failed: Trying to add a Command object without a memento set!"); //assert that the command has a DelegateMemento to execute.
    MAP_ITER iter = __cmdMap.find(cmd->getKey());
    assert(iter == __cmdMap.end() && "Class invariant assert failed: Unique key violation in CommandController.");
    __cmdMap[cmd->getKey()] = cmd;
}


void CommandController::addCommand(const COMMAND_KEY &key, const DelegateMemento &memento)
{
    //create the string command.
    using COMMAND::StringCommand;
    using COMMAND::CommandList;
    StringCommand *strCmd = new ZGame::COMMAND::StringCommand(key);
    strCmd->setCommandMemento(memento);
    addCommand(shared_ptr<Command>(strCmd));
}


/*
void CommandController::addCommand(Ogre::String cmdName, COMMAND::ConsoleCommand &cmd)
{
    //ZGame::OgreConsole* console = ZGame::OgreConsole::getSingletonPtr();
    __cmdMap[cmdName]=cmd;
    if(_console.get() == 0) //is console attached?
        return; //don't need to hook command up to console if it's not there.
    _console->addCommand(cmdName,ZGame::CommandController::execute);
}*/


