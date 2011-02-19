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

#include "ZPrerequisites.h"

//NEED to put out this lifecycle register no forward declare fire later.
#include "LifeCycleRegister.h"
#include "KeyEventRegister.h"
#include "MouseEventRegister.h"
#include "LifeCycleDelegates.h"
#include "DelegatesUtil.h"




namespace ZGame
{
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

        virtual void
            onGuiConfiguration(Gui::GuiController* guiCtrl){};

        virtual void
            onWorldControllerConfiguration(World::WorldController* worldCtrl){};

        virtual void
            onCinematicControllerConfiguration(World::CinematicController* cineCtrl){};



    protected:

        GameState(); //protected constructor

        virtual void
            regLfcObsForInjection(LifeCycleRegister &lfcReg);
        virtual void
            regKeyObsForInjection(KeyEventRegister &keyReg);
        virtual void
            regMouseObsForInjection(MouseEventRegister &mouseReg);

    protected:
    private:

    };

}


