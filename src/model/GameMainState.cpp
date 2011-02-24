/*
* GameMainState.cpp
*
*  Created on: Sep 18, 2008
*      Author: bey0nd
*/

using namespace std;
#include "GameMainState.h"
#include "gui/GuiController.h"
#include "gui/MainMenuScreen.h"
using namespace ZGame;

//_cam(0), 
GameMainState::GameMainState() :
GameState()
{

}

GameMainState::~GameMainState()
{
}

void
    GameMainState::getGameStateBootstrapInfo(GameStateBootstrapInfo &info)
{
    //Load this from a file.
    info.initalCameraPos = Ogre::Vector3(32.0f, 250.0f, 32.0f);
    info.requireZCLController = true;
    info.requireWorldController = true;
    info.requireControlModule = true;
    info.requireWorkspace = true;
    info.requireCharacterUtil = true;
}


/**
*This class will register LifeCycle observers (to be later injected into LifeCycle subjects: The Subject Observer pattern.)
*
*\note Notice we are selectively masking the life cycle functions. When this project started we had some sort of dependency injection thing in mind,
but alas that was a stupid idea because we don't really need that. We can manually do it ourselves. This means that there is not any concept of ordering in
life-cycle pumps, since dependency injection is not implemented. This works fine for now for mostly we just need a global pump to pump update events to
people. Initializing is not really needed. OnDestruction events are still useful. Safe-to-say we must think about dependencies.
**/
void
    GameMainState::regLfcObsForInjection(LifeCycleRegister &lfcReg)
{
    try
    {
        GameState::regLfcObsForInjection(lfcReg);
        LifeCycle::LifeCycleObserver lfcObs;

      
    }catch(Ogre::Exception e)
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, e.getDescription(),
            "GameMainState::regLfcObsForInjection");
    }

}

/**
*This class will register keyboard observers for later injection into keyboard subjects.
**/

void
    GameMainState::regKeyObsForInjection(KeyEventRegister &keyReg)
{
    GameState::regKeyObsForInjection(keyReg);
}

/**
*This class will register mouse observers for later injection into mouse subjects.
**/
void
    GameMainState::regMouseObsForInjection(MouseEventRegister &mouseReg)
{
    GameState::regMouseObsForInjection(mouseReg);
}

void
    GameMainState::onGuiConfiguration(Gui::GuiController* guiCtrl)
{
   
}