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
#include "EngineView.h"
namespace ZGame
{
  GameEditState::GameEditState() :
    GameState() , _controlMod(50.0f), _gfxCtrl(new ZGame::GraphicsController()), _volumePaging(0)//_editView(new GameEditView())

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
    //This state
    Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL, "In GameEditState regLfcObsForInjection");
    LifeCycle::LifeCycleObserver lfcObs;
    LifeCycle::bindLifeCycleObserver(lfcObs, *this);
    lfcReg.registerLfcObs(lfcObs);

    LifeCycle::bindLifeCycleObserver(lfcObs, _controlMod);
    lfcReg.registerLfcObs(lfcObs);

    LifeCycle::bindLifeCycleObserver(lfcObs, *_gfxCtrl.get(), LifeCycle::LFC_ON_UPDATE);

    Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL, "Out GameEditState regLfcObsForInjection");
  }

  void
  GameEditState::regKeyObsForInjection(KeyEventRegister &keyReg)
  {
    Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL, "In GameEditState regKeyObsForInjection");

    //This
    EVENT::KeyboardEvtObserver keyObs;
    EVENT::bindKeyObserver(keyObs, *this);
    keyReg.registerKeyObs(keyObs);

    EVENT::bindKeyObserver(keyObs, _controlMod);
    keyReg.registerKeyObs(keyObs);

    EVENT::bindKeyObserver(keyObs, *_gfxCtrl.get());
    keyReg.registerKeyObs(keyObs);

    Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL, "In GameEditState regKeyObsForInjection");
  }

  void
  GameEditState::regMouseObsForInjection(MouseEventRegister &mouseReg)
  {
    EVENT::MouseEvtObserver mouseObs;

    EVENT::bindMouseObserver(mouseObs, *this);
    mouseReg.registerMouseObs(mouseObs);

    EVENT::bindMouseObserver(mouseObs, _controlMod);
    mouseReg.registerMouseObs(mouseObs);
  }

  bool
  GameEditState::onInit()
  {
    Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL, "In GameEditState::onInit");
    Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL, "Initializing graphics controller.");
    _gfxCtrl->onInit();
    Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL, "Initializing Volume Map");
    _map.setOrigin(Ogre::Vector3(0, 128, 0));
    _map.load();

    _pageManager.setPageProvider(&_dummyPageProvider);
    _pageManager.addCamera(EngineView::getSingleton().getCurrentCamera());
    _volumePaging = OGRE_NEW World::VolumeMapPaging(&_pageManager);
    Ogre::PagedWorld* world = _pageManager.createWorld();
    _volumePaging->createWorldSection(world, &_map, _map.getRegionsHalfWidth(), _map.getRegionsHalfWidth(), -32768, -32768, 32768, 32768, EngineView::getSingleton().getSceneManager());

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
    Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL, "In GameEditState::onDestroy");
    return true;
  }

  bool
  GameEditState::onMouseDown(const OIS::MouseEvent &evt, const OIS::MouseButtonID id)
  {
    OgreBites::SdkTrayManager* _sdkTrayMgr = getSdkTray();
    if (_sdkTrayMgr->injectMouseDown(evt, id))
      return true;
    return true;
  }
  bool
  GameEditState::onMouseUp(const OIS::MouseEvent &evt, const OIS::MouseButtonID id)
  {
    OgreBites::SdkTrayManager* _sdkTrayMgr = getSdkTray();
    if (_sdkTrayMgr->injectMouseUp(evt, id))
      return true;
    return true;
  }
  bool
  GameEditState::onMouseMove(const OIS::MouseEvent &evt)
  {
    OgreBites::SdkTrayManager* _sdkTrayMgr = getSdkTray();
    if (_sdkTrayMgr->injectMouseMove(evt))
      return true;
    return true;
  }
  bool
  GameEditState::onKeyDown(const OIS::KeyEvent &evt)
  {
    OgreBites::SdkTrayManager* _sdkTrayMgr = getSdkTray();
    if (evt.key == OIS::KC_TAB)
      {
        if (_sdkTrayMgr->areTraysVisible() && _sdkTrayMgr->isCursorVisible())
          {
            _sdkTrayMgr->hideAll();
          }
        else
          {
            _sdkTrayMgr->showAll();
          }
      }
    return true;
  }

  bool
  GameEditState::onKeyUp(const OIS::KeyEvent &evt)
  {
    return true;
  }

}

