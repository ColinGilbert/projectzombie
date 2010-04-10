#include <iostream>
#include <list>
#include "CommandController.h"
#include "ogreconsole.h"
#include "CommandDelegates.h"



#include "ControlModuleProto.h"


using namespace ZGame;

std::map<Ogre::String,ZGame::COMMAND::ConsoleCommand> __cmdMap;

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
  cout << "In CommandController destructor." << endl;
}

/**
*This static method is the call back for executing a found command in OgreConsole. We assume that the command has been validated upstream in OgreConsole,
and thus when calling this params[0] should be the calle.
*/
void CommandController::execute(const Ogre::StringVector &params)
{
    try
    {
    __cmdMap[params[0]](params);
    }catch(Ogre::Exception e)
    {
        cout << "Caught Ogre exception in CommandController::execute" << endl;
    }
    catch(std::exception e)
    {
        cout << "Caught exception in CommandController::execute" << endl;
    }
}

/**
*This method will execute the command given in param. It is assumed the first element is the command, followed by N parameters.
*/
void 
CommandController::executeCmd(const Ogre::StringVector &params)
{
    try
    {
        std::map<Ogre::String,ZGame::COMMAND::ConsoleCommand>::iterator cmdMapIter;
        cmdMapIter = __cmdMap.find(params[0]);
        assert(cmdMapIter != __cmdMap.end() && "The command you are trying to execute does not exist in __cmdMap.");
        __cmdMap[params[0]](params);
    }catch(std::exception e)
    {
        cout << "Exception in CommandController executeCmd: " << e.what() << ". Maybe you forgot to insert the actual command. Please check." << endl;
    }
}

/**
*This method will initialize CommandController with an OgreConsole.
*
**/
bool CommandController::init()
{
  return true;
}

void CommandController::addCommand(Ogre::String cmdName, COMMAND::ConsoleCommand &cmd)
{
  ZGame::OgreConsole* console = ZGame::OgreConsole::getSingletonPtr();
  __cmdMap[cmdName]=cmd;
  console->addCommand(cmdName,ZGame::CommandController::execute);
}


