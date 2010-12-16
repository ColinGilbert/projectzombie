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
#include "LifeCycleRegister.h"
#include "KeyEventRegister.h"
#include "MouseEventRegister.h"
#include "LifeCycleDelegates.h"
#include "DelegatesUtil.h"

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
    namespace Gui
    {
        class GuiController;
    }
    /**
    * This class contains information needed to bootstrap the Engine. The system
    *requires certain bootstrapping information in order to boot various sub systems.
    *We do this so we can couple this with a state, say to load this information along
    *with the State description.
    **/
    struct GameStateBootstrapInfo
    {
        Ogre::Vector3 initalCameraPos;
    };

    class GameState
    {
    public:
        virtual
            ~GameState();
        void
            init(LifeCycleRegister &lfcReg, KeyEventRegister &keyReg,
            MouseEventRegister &mouseReg, OgreBites::SdkTrayManager* tray);
            virtual void
            getGameStateBootstrapInfo(GameStateBootstrapInfo &info)
        {
            info.initalCameraPos = Ogre::Vector3::ZERO;
        }
    protected:

        GameState(); //protected constructor



        virtual void
            GameState::regLfcObsForInjection(LifeCycleRegister &lfcReg);
        virtual void
            GameState::regKeyObsForInjection(KeyEventRegister &keyReg);
        virtual void
            GameState::regMouseObsForInjection(MouseEventRegister &mouseReg);



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
        auto_ptr<Gui::GuiController> _guiCtrl;
        OgreBites::SdkTrayManager* _tray;

    };

}


