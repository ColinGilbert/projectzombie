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
#include "GPUEntities.h"
#include "GPUEntsGenProps.h"
#include "GPUEntsView.h"
#include "GPUEntsGen.h"
#include "entities/ZEntity.h"
#include "EngineView.h"
#include "DelegatesUtil.h"
#include "EventDelegates.h"
#include "LifeCycleRegister.h"
#include "KeyEventRegister.h"
#include "MouseEventRegister.h"
#include "ControlModuleProto.h"
#include "WhiteNoiseView.h"
#include "GPUEntsControl.h"
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

using namespace Ogre;
using namespace ZGame;
using Entities::EntitiesManager;
using Entities::RenderEntitiesManager;
using ZCL::ZCLController;

GameMainState::GameMainState() :
  GameState(), _gpuEntsView(new GPUEntsView()), _cam(0), _dz(1.5f), _forward(false), _backward(false), _trans(1.0), _controlMod(new ControlModuleProto()),
      _whtNoiseView(new WhiteNoiseView()), _gpuEntsControl(new GPUEntsControl()), _worldController(new World::WorldController()), _charUtil(
          new Util::CharacterUtil()), _entMgr(0), //Do not initialize them here as services are not up yet when we are creating the GameMainState. This needs to change. i.e: we need to create game main state after ogre initializes.
      _rdrEntMgr(0), _zclCtrl(new ZCLController()), _entsView(new Entities::EntitiesView()), _workspace(0)
{

}

GameMainState::~GameMainState()
{
  _controlMod.reset();
  cout << "ControlModule reset." << endl;
  _gpuEntsControl.reset();
  cout << "gpu ents ctrl reset." << endl;
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
  //this
  LifeCycle::LifeCycleObserver lfcObs;
  LifeCycle::bindLifeCycleObserver(lfcObs, *this);
  lfcReg.registerLfcObs(lfcObs);

  //OpenCLController
  LifeCycle::bindLifeCycleObserver(lfcObs, *_zclCtrl, LifeCycle::LFC_ON_UPDATE | LifeCycle::LFC_ON_DESTROY);
  //world controller
  LifeCycle::bindLifeCycleObserver(lfcObs, *_worldController, LifeCycle::LFC_ON_UPDATE | LifeCycle::LFC_ON_DESTROY);
  lfcReg.registerLfcObs(lfcObs);

  //control module
  LifeCycle::bindLifeCycleObserver(lfcObs, *_controlMod);
  lfcReg.registerLfcObs(lfcObs);

}

/**
 *This class will register keyboard observers for later injection into keyboard subjects.
 **/

void
GameMainState::regKeyObsForInjection(KeyEventRegister &keyReg)
{
  //ControlModule
  EVENT::KeyboardEvtObserver keyObs;
  EVENT::bindKeyObserver(keyObs, *_controlMod);
  keyReg.registerKeyObs(keyObs);
  EVENT::clearKeyObs(keyObs);

  EVENT::bindKeyObserver(keyObs, *this);
  keyReg.registerKeyObs(keyObs);
  EVENT::clearKeyObs(keyObs);

}

/**
 *This class will register mouse observers for later injection into mouse subjects.
 **/
void
GameMainState::regMouseObsForInjection(MouseEventRegister &mouseReg)
{
  //Control Module
  EVENT::MouseEvtObserver mouseObs;

  EVENT::bindMouseObserver(mouseObs, *this);
  mouseReg.registerMouseObs(mouseObs);
  //THe entities view (this is the temp entities creation menu.)
  //EVENT::bindMouseObserver(mouseObs, *_entsView);
  //mouseReg.registerMouseObs(mouseObs);
  //EVENT::clearMouseObs(mouseObs);

  //EVENT::bindMouseObserver(mouseObs, *_controlMod);
  //mouseReg.registerMouseObs(mouseObs);
  //EVENT::clearMouseObs(mouseObs);

}

/**
 *This method will initialize the GameMainState object. This method is called during the initialization phase of the life-cycle. There is an odering
 *dependency for initialization of the GameMainState object, and as such we must pay heed to the ordering of object initialization. We tried to keep
 *the different modules orthgonal whenever possible.
 *
 **/
bool
GameMainState::onInit()
{
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL, "In GameMainState onInit");

  _zclCtrl->init("../scripts/testkernels.cl");
  Ogre::LogManager::getSingleton().logMessage("Done initializing OpenCL.");
  //Setup the entities manager.
  _entMgr.reset(new Entities::EntitiesManager());
  _rdrEntMgr.reset(new Entities::RenderEntitiesManager());

  initGraphicalState();
  _worldController->onInit();
  Ogre::LogManager::getSingleton().logMessage("Done creating world");

  createCharacters();
  Ogre::LogManager::getSingleton().logMessage("Done creating characters.");
  //We need to initialize OpenCL after creating the world and entities, as OpenCL requires both entities and world map data to function.
  Ogre::LogManager::getSingleton().logMessage("Intitializing MainGameState SDK Tray.");
  _workspace.reset(new ZWorkspace(_entMgr.get(), _rdrEntMgr.get(), getSdkTray(), _zclCtrl.get(), _worldController.get()));
  _entsView->init(_workspace.get());

  return true;
}


bool
GameMainState::onUpdate(const Ogre::FrameEvent &evt)
{
  if (_entMgr->getNumOfEntities() > 0)
    {
      _zclCtrl->onUpdate(evt);
      const float* posBuf = 0;
      const float* orientBuf = 0;
      const float* velocityBuf = 0;
      _zclCtrl->getBuffers(posBuf, orientBuf, velocityBuf);
      _rdrEntMgr->updateRenderEntities(posBuf, orientBuf, velocityBuf, evt.timeSinceLastFrame);
    }
  return true;
}

bool
GameMainState::onDestroy()
{
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL, "In GameMainState::noDestroy()");
  _zclCtrl->printKernelTime();
  return true;
}
/**
 * Handle custom on mouse up events. Here there is a implied ordering with respect to the GUI element. Note, we may need to refactor how we do
 * eclusive mode control, so we do not generate control events when in such a mode.
 */
bool
GameMainState::onMouseMove(const OIS::MouseEvent &evt)
{
  OgreBites::SdkTrayManager* _sdkTrayMgr = getSdkTray();
  if (_sdkTrayMgr->injectMouseMove(evt))
    return true;
  if (_sdkTrayMgr->isCursorVisible())
    {
      _entsView->onMouseMove(evt);
      return true;
    }
  _controlMod->onMouseMove(evt);

  return true;
}

bool
GameMainState::onMouseDown(const OIS::MouseEvent &evt, const OIS::MouseButtonID id)
{
  OgreBites::SdkTrayManager* _sdkTrayMgr = getSdkTray();
  if (_sdkTrayMgr->injectMouseDown(evt, id))
    return true;
  _entsView->onMouseDown(evt, id);
  _controlMod->onMouseDown(evt, id);
}

bool
GameMainState::onMouseUp(const OIS::MouseEvent &evt, const OIS::MouseButtonID id)
{
  OgreBites::SdkTrayManager* _sdkTrayMgr = getSdkTray();
  if (_sdkTrayMgr->injectMouseUp(evt, id))
    return true;
  _entsView->onMouseUp(evt, id);
  _entsView->onMouseUp(evt, id);
}

bool
GameMainState::onKeyDown(const OIS::KeyEvent &evt)
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
GameMainState::onKeyUp(const OIS::KeyEvent &evt)
{
  return true;
}

void
GameMainState::createGPUEntities()
{
  LogManager* lm = LogManager::getSingletonPtr();
  lm->logMessage(LML_NORMAL, "GameMainState::createGPUEntities");
  //note: we are using shared_ptr here is because later we will have an entity resource manager.
  boost::shared_ptr<Entities::ZEntity> zent(new Entities::ZEntity("ZombieEntity", //"robot.mesh"));
      "RZR-002.mesh"));
  //"athene.mesh"));
  int texW = 64;
  int texH = 64;
  Real minX, maxX, minZ, maxZ; //the space into which we want to distribute the GPU entitise
  minX = -2000.0f;
  maxX = 2000.0f;
  minZ = -500.0;
  maxZ = 500.0f;
  Real entHeight = 2.0f; //1.6 meters
  auto_ptr<GPUEntsGenProps> props(new GPUEntsGenProps(texW, texH, entHeight));
  props->setExtents(minX, minZ, maxX, maxZ);
  GPUEntsGen entsGen(zent, props);
  entsGen.build();
  _gpuEnts = entsGen.getOutput();
  lm->logMessage(LML_TRIVIAL, "About to attach GPU ents");
  _gpuEntsView->attachGPUEnts(_gpuEnts.get());
  lm->logMessage(Ogre::LML_TRIVIAL, "GameMainState::createGPUEntities done");
  _gpuEntsControl->attachGPUEnts(_gpuEnts.get());
  lm->logMessage(LML_TRIVIAL, "Out Gamestate createGPUEntities");
}

void
GameMainState::createCharacters()
{
}

void
GameMainState::initGraphicalState()
{
  Ogre::LogManager *lm = Ogre::LogManager::getSingletonPtr();
  lm->logMessage(Ogre::LML_TRIVIAL, "In GameMainState::initGraphicalState");
  GraphicsController* gfxCtrl = getGraphicsController();
  gfxCtrl->onInit();
  lm->logMessage(Ogre::LML_TRIVIAL, "Out of GameMainState::initGraphicalState");
}

