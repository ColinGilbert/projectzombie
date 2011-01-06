/*
* GameMainMenuState.cpp
*
*  Created on: Sep 5, 2008
*      Author: bey0nd
*/

#include "GameMainMenuState.h"
#include "gui/GuiController.h"
#include "gui/MainMenuScreen.h"
#include "ZInitPacket.h"
#include "world/CinematicController.h"
#include "world/CinematicManager.h"

using namespace ZGame;

GameMainMenuState::GameMainMenuState() : _mainMenu(0)
{

}

GameMainMenuState::~GameMainMenuState()
{

}

void
    GameMainMenuState::getGameStateBootstrapInfo(GameStateBootstrapInfo &info)
{
    info.initalCameraPos = Ogre::Vector3(32.0f, 250.0f, 32.0f);
    info.requireWorldController = true;
}


void GameMainMenuState::regLfcObsForInjection(LifeCycleRegister &lfcReg)
{
    GameState::regLfcObsForInjection(lfcReg);
    //This
    LifeCycle::LifeCycleObserver lfcObs;
    try
    {
        LifeCycle::bindAndRegisterLifeCycleObserver<GameMainMenuState>(lfcReg, lfcObs,*this);
    }catch(Ogre::Exception e)
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, e.getDescription() + " in GameMainMenuState", 
            "GameMainMenuState::regLfcObsForInjection");
    }

}

void GameMainMenuState::regKeyObsForInjection(KeyEventRegister &keyReg)
{
    GameState::regKeyObsForInjection(keyReg);
    /*
    EVENT::KeyboardEvtObserver keyObs;
    EVENT::bindKeyObserver(keyObs,*this);
    keyReg.registerKeyObs(keyObs);
    EVENT::clearKeyObs(keyObs);
    */
}

bool GameMainMenuState::onUpdate(const Ogre::FrameEvent& evt)
{
    return true;
}

bool GameMainMenuState::onInit(ZGame::ZInitPacket *packet)
{
    _mainMenu = static_cast<Gui::MainMenuScreen*>(packet->guiCtrl->getScreen("MainMenuScreen"));
    _mainMenu->onLoad();
    return true;
}

bool GameMainMenuState::onDestroy()
{
    return true;
}

bool GameMainMenuState::onKeyDown(const OIS::KeyEvent &evt)
{
    return true;
}

bool GameMainMenuState::onKeyUp(const OIS::KeyEvent &evt)
{
    return true;
}

void
    GameMainMenuState::onGuiConfiguration(Gui::GuiController* guiCtrl)
{

}

void
    GameMainMenuState::onCinematicControllerConfiguration(World::CinematicController* cineCtrl)
{
    using World::CinematicController;
    //Create the cameras;
    std::vector<CinematicController::CAM_PAIR> cams;
    cams.push_back(std::make_pair<Ogre::String, CinematicController::CAM_INIT_STATE>("PERSPECTIVE",
        std::make_pair<Ogre::Vector3, Ogre::Quaternion>( Ogre::Vector3(32.0f, 1000.0f, 32.0f), Ogre::Quaternion() ) ) );
    cineCtrl->loadCameras(cams, 0);
}