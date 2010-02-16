#ifndef _WORLDCONTROLLER_H
#define _WORLDCONTROLLER_H

#include <Ogre.h>
#include "Controller.h"

/**
*This class defines the world controller.
*
**/

namespace Ogre
{
  class SceneManager;
}

namespace ZGame
{
  namespace World
  {
    
    class WorldController : public Controller
    {
    public:
      WorldController();
      ~WorldController();

      //Virtual method which will handle execuetion of controller commands.
      virtual int execute(ZGame::Command);

      void init();
      void loadWorld(); //temp.

      //implements the LifeCycleEvents
      bool onInit();
      bool onUpdate(const Ogre::FrameEvent &evt);
      bool onDestroy();  

    protected:
    private:

      Ogre::Entity* _bobEnt;
      Ogre::AnimationState* _animState;

    };
  }
}


#endif