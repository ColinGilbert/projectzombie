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

int CommandController::execute(ZGame::Command cmd)
{
  return 0;
}

int CommandController::execute(std::string cmd)
{
  return 0;
}



/**
*This static method is the call back for executing a found command in OgreConsole. We assume that the command has been validated upstream in OgreConsole,
and thus when calling this params[0] should be the calle.
*/
void CommandController::execute(std::vector<Ogre::String> &params)
{
    __cmdMap[params[0]](params);
}

/**
*This method will initialize CommandController with an OgreConsole.
*
**/
bool CommandController::init()
{
  /*
  Ogre::String CHARLIST("charlist");
  Ogre::String CHARLISTMESHES("charlistmeshes");
  Ogre::String CHARCREATE("charcreate");
  Ogre::String CHARATTACH("attachcamera");

  //_charUtil.
  //void (ZGame::Util::CharacterUtil::list)(std::vector<Ogre::String>) memPtr
  COMMAND::ConsoleCommand cmd;
  cmd.bind(&ZGame::Util::CharacterUtil::list,_charUtil.get());
  addCommand(CHARLIST,cmd);
  cmd.bind(&ZGame::Util::CharacterUtil::listMeshes,_charUtil.get());
  addCommand(CHARLISTMESHES,cmd);
  cmd.bind(&ZGame::Util::CharacterUtil::create,_charUtil.get());
  addCommand(CHARCREATE,cmd);
  cmd.bind(&ZGame::ControlModuleProto::attachNode,_controlModule);
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL,"init. command controller");
  */
  return true;
}

void CommandController::addCommand(Ogre::String cmdName, COMMAND::ConsoleCommand &cmd)
{
  ZGame::OgreConsole* console = ZGame::OgreConsole::getSingletonPtr();
  __cmdMap[cmdName]=cmd;
  console->addCommand(cmdName,ZGame::CommandController::execute);
}


