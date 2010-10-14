/*
 * GameState.h
 *
 *  Created on: Aug 28, 2008
 *      Author: bey0nd
 *
 * This class defines the GameState class. The GameState class is the super class for the concept of states in ZGame's engine.
 * Mainly the idea here is that the state class will store the current state of the game. For example: In the GameMainState, we would store all the controllers and managers
 * used by the GameMainState. It will also handle registering any objects residing in the state for events. So conceptually this is similar to a controller. The other idea
 * is we can push stuff to the objects residing in a game state--dependency injection,sort of.
 */

#pragma once

#include <vector>
#include <memory>

#include <OIS/OIS.h>

using namespace std;

namespace OgreBites
{
  class SdkTrayManager;
}

namespace ZGame
{
  class LifeCycleRegister;
  class KeyEventRegister;
  class MouseEventRegister;
  class GraphicsController;

  class GameState
  {
  public:
    virtual
    ~GameState();
    void
    init(LifeCycleRegister &lfcReg, KeyEventRegister &keyReg,
        MouseEventRegister &mouseReg, OgreBites::SdkTrayManager* tray);
  protected:

    GameState(); //protected constructor
    virtual void
    regLfcObsForInjection(LifeCycleRegister &lfcReg)
    {
    }
    virtual void
    regKeyObsForInjection(KeyEventRegister &keyReg)
    {
    }
    virtual void
    regMouseObsForInjection(MouseEventRegister &mouseReg)
    {
    }

    GraphicsController*
    getGraphicsController()
    {
      return _gfxCtrl.get();
    }

    OgreBites::SdkTrayManager*
    getSdkTray()
    {
      return _tray;
    }

  protected:
  private:
    auto_ptr<GraphicsController> _gfxCtrl;
    OgreBites::SdkTrayManager* _tray;

  };

}


