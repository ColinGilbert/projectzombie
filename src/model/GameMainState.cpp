/*
* GameMainState.cpp
*
*  Created on: Sep 18, 2008
*      Author: bey0nd
*/

#include <cmath>
#include <SdkTrays.h>
using namespace std;
#include <boost/shared_ptr.hpp>
#include "GameMainState.h"
#include "entities/ZEntity.h"
#include "ControlModuleProto.h"
#include "world/WorldController.h"
#include "CommandController.h"
#include "utilities/CharacterUtil.h"
#include "entities/EntitiesManager.h"
#include "entities/RenderEntitiesManager.h"
#include "entities/ZEntityBuilder.h"
#include "ZCL/ZCLController.h"
#include "GraphicsController.h"
#include "entities/EntitiesView.h"
#include "ZWorkspace.h"
#include "ZWorkspaceController.h"

using namespace Ogre;
using namespace ZGame;
using Entities::EntitiesManager;
using Entities::RenderEntitiesManager;
using ZCL::ZCLController;
//_cam(0), 
GameMainState::GameMainState() :
GameState(), _controlMod(new ControlModuleProto()),
    _worldController(new World::WorldController()), _charUtil(new Util::CharacterUtil()),
    _workspaceCtrl(new ZWorkspaceController()), 
    _entMgr(new Entities::EntitiesManager()), _rdrEntMgr(new Entities::RenderEntitiesManager())
{

}

GameMainState::~GameMainState()
{
    //_controlMod.reset();
    cout << "ControlModule reset." << endl;
    _worldController.reset(0);
    cout << "World controller reset." << endl;
    _charUtil.reset(0);
    cout << "character util reset." << endl;
    _entMgr.reset(0);
    cout << "entity manager reset." << endl;
    _rdrEntMgr.reset(0);
    cout << "render entity manager reset." << endl;
    _zclCtrl.reset(0);
    cout << "ZCL ctrl reset." << endl;
    _entsView.reset(0);
    cout << "ents view reset." << endl;
}

void
    GameMainState::getGameStateBootstrapInfo(GameStateBootstrapInfo &info)
{
    //Load this from a file.
    info.initalCameraPos = Ogre::Vector3(32.0f, 250.0f, 32.0f);
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

        //this
        try
        {
            LifeCycle::bindAndRegisterLifeCycleObserver<GameMainState>(lfcReg, 
                lfcObs, *this);
        }catch(Ogre::Exception e)
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, e.getDescription() + " in GameMainState", "");
        }
        try
        {
            //render entities
            LifeCycle::bindAndRegisterLifeCycleObserver<Entities::RenderEntitiesManager>(lfcReg,
                lfcObs, *_rdrEntMgr, LifeCycle::LFC_ON_INIT);
        }catch(Ogre::Exception e)
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, e.getDescription() + " in RenderEntitiesManager", "");
        }
        
        try
        {
            //OpenCLController
            LifeCycle::bindAndRegisterLifeCycleObserver<ZGame::ZCL::ZCLController>(lfcReg, 
                lfcObs, *_zclCtrl, LifeCycle::LFC_ON_DESTROY);
        }catch(Ogre::Exception e)
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, e.getDescription() + " in ZCLController", "");
        }
        try
        {
            //world controller
            LifeCycle::bindAndRegisterLifeCycleObserver<ZGame::World::WorldController>(lfcReg, 
                lfcObs, *_worldController, LifeCycle::LFC_ON_INIT | LifeCycle::LFC_ON_DESTROY);
        }catch(Ogre::Exception e)
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, e.getDescription() + " in WorldController", "");
        }
        try
        {
            //control module
            LifeCycle::bindAndRegisterLifeCycleObserver<ZGame::ControlModuleProto>(lfcReg, lfcObs, *_controlMod,
                LifeCycle::LFC_ON_INIT | LifeCycle::LFC_ON_UPDATE | LifeCycle::LFC_ON_DESTROY);
        }catch(Ogre::Exception e)
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, e.getDescription() + " in ControlModuleProto", "");
        }
        try
        {
            //Workspace controller
            LifeCycle::bindAndRegisterLifeCycleObserver<ZGame::ZWorkspaceController>(lfcReg, lfcObs, *_workspaceCtrl,
                LifeCycle::LFC_ON_INIT | LifeCycle::LFC_ON_DESTROY);
        }catch(Ogre::Exception e)
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, e.getDescription() + " in ZWorkspaceController", "");
        }
        try
        {
            LifeCycle::bindAndRegisterLifeCycleObserver<ZGame::Util::CharacterUtil>(lfcReg, lfcObs, *_charUtil,
                LifeCycle::LFC_ON_INIT);
        }catch(Ogre::Exception e)
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, e.getDescription() + " in CharacterUtil", "");
        }
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
    //ControlModule
    EVENT::KeyboardEvtObserver keyObs;
    EVENT::bindKeyObserver(keyObs, *_controlMod);
    keyReg.registerKeyObs(keyObs);

    EVENT::bindKeyObserver(keyObs, *this);
    keyReg.registerKeyObs(keyObs);
    //Workspace controller
    EVENT::bindKeyObserver(keyObs, *_workspaceCtrl);
    keyReg.registerKeyObs(keyObs);
}

/**
*This class will register mouse observers for later injection into mouse subjects.
**/
void
    GameMainState::regMouseObsForInjection(MouseEventRegister &mouseReg)
{
    GameState::regMouseObsForInjection(mouseReg);
    //Control Module
    EVENT::MouseEvtObserver mouseObs;

    EVENT::bindMouseObserver(mouseObs, *this);
    mouseReg.registerMouseObs(mouseObs);

    EVENT::bindMouseObserver(mouseObs, *_workspaceCtrl);
    mouseReg.registerMouseObs(mouseObs);
}

/**
*This method will initialize the GameMainState object. This method is called during the initialization phase of the life-cycle. There is an odering
*dependency for initialization of the GameMainState object, and as such we must pay heed to the ordering of object initialization. We tried to keep
*the different modules orthgonal whenever possible.
*
**/
bool
    GameMainState::onInit(ZGame::ZInitPacket initPacket)
{
    Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL, "In GameMainState onInit");

    Ogre::LogManager::getSingleton().logMessage("Done creating characters.");
    Ogre::LogManager::getSingleton().logMessage("Intitializing MainGameState SDK Tray.");

    _workspace.reset(new ZWorkspace(initPacket.sceneManager, _entMgr.get(), _rdrEntMgr.get(), getSdkTray(), _zclCtrl.get(), _worldController.get()));
    _workspaceCtrl->setZWorkspace(_workspace.get());

    return true;
}

bool
    GameMainState::onUpdate(const Ogre::FrameEvent &evt)
{
    /*
    if (_entMgr->getNumOfEntities() > 0)
    {
    _entMgr->updateDensityBuffer();
    _zclCtrl->onUpdate(evt);
    const float* posBuf = 0;
    const float* orientBuf = 0;
    const float* velocityBuf = 0;
    _zclCtrl->getBuffers(posBuf, orientBuf, velocityBuf);
    _rdrEntMgr->updateRenderEntities(posBuf, orientBuf, velocityBuf, evt.timeSinceLastFrame);
    }
    */
    _worldController->onUpdate(evt);
    return true;
}

bool
    GameMainState::onDestroy()
{
    Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL, "In GameMainState::noDestroy()");
    //_zclCtrl->printKernelTime();
    return true;
}
/**
* Handle custom on mouse up events. Here there is a implied ordering with respect to the GUI element. Note, we may need to refactor how we do
* eclusive mode control, so we do not generate control events when in such a mode.
*/
bool
    GameMainState::onMouseMove(const OIS::MouseEvent &evt)
{
    _controlMod->onMouseMove(evt);

    return true;
}

bool
    GameMainState::onMouseDown(const OIS::MouseEvent &evt, const OIS::MouseButtonID id)
{
    _controlMod->onMouseDown(evt, id);
    return true;
}

bool
    GameMainState::onMouseUp(const OIS::MouseEvent &evt, const OIS::MouseButtonID id)
{
    return true;
}

bool
    GameMainState::onKeyDown(const OIS::KeyEvent &evt)
{
    return true;
}

bool
    GameMainState::onKeyUp(const OIS::KeyEvent &evt)
{
    return true;
}



