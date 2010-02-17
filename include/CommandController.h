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
  namespace Util
  {
  class CharacterUtil;
  }

  class ControlModuleProto;

  class CommandController : public Controller, public Ogre::Singleton<CommandController>
  {
  public:
    CommandController();
    ~CommandController();

    virtual int execute(ZGame::Command);
    int execute(std::string cmd);
    bool init();
    void addCommand(Ogre::String cmdName, COMMAND::ConsoleCommand &cmd);

    static void execute(Ogre::StringVector &params);
    //void execute(std::vector<Ogre::String> &params);

    static ZGame::CommandController&
      getSingleton();
    static ZGame::CommandController*
      getSingletonPtr();
  protected:

  private:
  };
}


#endif