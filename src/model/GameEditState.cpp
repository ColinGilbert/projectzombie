/*
* GameEditState.cpp
*
*  Created on: Aug 28, 2008
*      Author: bey0nd
*/
#include <Ogre.h>
#include <SdkTrays.h>
#include "GameEditState.h"
#include "GameEditView.h"
#include "LifeCycleRegister.h"
#include "KeyEventRegister.h"
#include "MouseEventRegister.h"
#include "EventDelegates.h"
#include "DelegatesUtil.h" //This is for LifeCycle::bindLifeCycleObserver and etc...
#include "GraphicsController.h"
namespace ZGame
{
    GameEditState::GameEditState() :
GameState() , _controlMod(50.0f)

{
    // TODO Auto-generated constructor stub
}

GameEditState::~GameEditState()
{
    // TODO Auto-generated destructor stub
}

void
    GameEditState::regLfcObsForInjection(LifeCycleRegister &lfcReg)
{
    GameState::regLfcObsForInjection(lfcReg);
    //This state
    LifeCycle::LifeCycleObserver lfcObs;
    Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL, "In GameEditState regLfcObsForInjection");

    try
    {
        try
        {

            LifeCycle::bindAndRegisterLifeCycleObserver<GameEditState>(lfcReg, lfcObs, *this);
        }catch(Ogre::Exception e)
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, e.getDescription() + " in GameEditState", "");
        }
        try
        {
            LifeCycle::bindAndRegisterLifeCycleObserver<ZGame::ControlModuleProto>(lfcReg, lfcObs, _controlMod);
        }catch(Ogre::Exception e)
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, e.getDescription() + " in ControlModuleProto", "");
        }
    }catch(Ogre::Exception e)
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, e.getDescription(), "GameEditState::regLfcObsForInjection");
    }
    Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL, "Out GameEditState regLfcObsForInjection");
}

void
    GameEditState::regKeyObsForInjection(KeyEventRegister &keyReg)
{
    Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL, "In GameEditState regKeyObsForInjection");
   
    Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL, "In GameEditState regKeyObsForInjection");
}

void
    GameEditState::regMouseObsForInjection(MouseEventRegister &mouseReg)
{
    GameState::regMouseObsForInjection(mouseReg);
    
 
}

bool
    GameEditState::onInit()
{
    Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL, "In GameEditState::onInit");

    return true;
}

bool
    GameEditState::onUpdate(const Ogre::FrameEvent& evt)
{
    //Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL,"In GameEditState::onUpdate");
    return true;
}

bool
    GameEditState::onDestroy()
{
    using World::VolumeMapPaging;
    Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL, "In GameEditState::onDestroy");
    return true;
}

bool
    GameEditState::onMouseDown(const OIS::MouseEvent &evt, const OIS::MouseButtonID id)
{
    return true;
}
bool
    GameEditState::onMouseUp(const OIS::MouseEvent &evt, const OIS::MouseButtonID id)
{
    return true;
}
bool
    GameEditState::onMouseMove(const OIS::MouseEvent &evt)
{
    return true;
}
bool
    GameEditState::onKeyDown(const OIS::KeyEvent &evt)
{
    return true;
}

bool
    GameEditState::onKeyUp(const OIS::KeyEvent &evt)
{
    return true;
}

}

