/*
 * EngineController.cpp
 *
 *  Created on: Aug 24, 2008
 *      Author: bey0nd
 */

#include <string>
#include <iostream>
#include <stdexcept>
using namespace std;

#include "EngineController.h"
#include "EngineView.h"
#include "GameStateFactory.h"
#include "StatesLoader.h"
#include "InputController.h"
#include "GameState.h"
#include "LifeCyclePump.h"
#include "KeyboardPump.h"
#include "MousePump.h"
#include "LifeCycleRegister.h"
#include "KeyEventRegister.h"
#include "MouseEventRegister.h"

namespace ZGame
{

  EngineController::EngineController() :
    _stillRunning(true), _lfcPump(new LifeCyclePump()), _keyPump(
        new KeyboardPump()), _mousePump(new MousePump())
  {
    // TODO Auto-generated constructor stub
    _listenerID = "EngineControllerListenerID";
  }

  EngineController::~EngineController()
  {
    // TODO Auto-generated destructor stub
    _gameSInfoMap.clear();

  }

  void
  EngineController::transitionState(const string key)
  {
    try
      {
        loadCurrentState(key);
        realizeCurrentState();
      }
    catch (std::invalid_argument e)
      {
        std::ostringstream sstream;
        sstream << "Exception: " << e.what() << endl;
        sstream << "Transition state does not exist: " << key << endl;
        sstream << "Transitioning from state: " << _curStateInfo->key << endl;
        Ogre::LogManager::getSingleton().logMessage(Ogre::LML_CRITICAL,
            sstream.str());
      }
  }

  Ogre::Camera*
  EngineController::createDefaultCamera()
  {
    using namespace Ogre;
    //Camera* cam =_scnMgr->createCamera(_window->getName());
    Camera* cam = _scnMgr->createCamera("ENGINE_VIEW_CAMERA");
    cam->setPosition(0, 0, 100.0);
    cam->lookAt(0, 0, -1);
    cam->setNearClipDistance(1.0);
    return cam;
  }

  bool
  EngineController::onInit()
  {
    using namespace Ogre;
    //_root = new Ogre::Root("plugins.cfg");
    _root.reset(new Ogre::Root("plugins.cfg"));
    if (_root->showConfigDialog())
      {
        _window = _root->initialise(true);
      }
    else
      return false;

    _scnMgr = _root->createSceneManager(Ogre::ST_GENERIC, "ExampleSMInstance");

    loadAssets();
    Ogre::Camera* cam = createDefaultCamera();
    Ogre::Viewport* vp = _window->addViewport(cam);
    vp->setBackgroundColour(Ogre::ColourValue(1.0, 0.0, 0.0));

    cam->setAspectRatio(Real(vp->getActualWidth())
        / Real(vp->getActualHeight()));

    _engineView.reset(new ZGame::EngineView(_window, cam, _scnMgr));

    //load states
    loadStates();

    //input
    _inController.reset(new InputController());
    _inController->onInit(_window);
    injectInputSubject();

    _root->addFrameListener(this);

    //set logging lvl
    Ogre::LogManager::getSingleton().setLogDetail(Ogre::LL_NORMAL);

    return true;
  }

  void
  EngineController::injectInputSubject()
  {
    ZGame::EVENT::KeyboardEvtObserver keyObs;
    keyObs.kde.bind(&ZGame::EngineController::onKeyDown, this);
    keyObs.kue.bind(&ZGame::EngineController::onKeyUp, this);
    _inController->addKeyListeners(_listenerID, keyObs);
    ZGame::EVENT::MouseEvtObserver mouseObs;
    mouseObs.mde.bind(&ZGame::EngineController::onMouseDown, this);
    mouseObs.mue.bind(&ZGame::EngineController::onMouseUp, this);
    mouseObs.mme.bind(&ZGame::EngineController::onMouseMove, this);
    _inController->addMouseListeners(_listenerID, mouseObs);
  }

  void
  EngineController::loadAssets()
  {
    Ogre::ConfigFile cf;
    string resourcePath;
    resourcePath = "";
    cf.load(resourcePath + "resources.cfg");
    //go thourhg all sections & settings in the file
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements())
      {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
          {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                resourcePath + archName, typeName, secName);
          }
      }
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
  }

  bool
  EngineController::frameStarted(const Ogre::FrameEvent &evt)
  {
    if (!_stillRunning)
      return false;
    try
      {
        _inController->run();
        _lfcPump->updateOnUpdateObs(evt);
      }
    catch (Ogre::Exception e)
      {
        throw e;
      }

    return true;

  }
  void
  EngineController::run()
  {
    realizeCurrentState();
    _root->startRendering();
    //_root->renderOneFrame();
  }

  void
  EngineController::onDestroy()
  {
    Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL,
        "EngineController.onDestroy()");
    //unloadCurrentState();

    try
      {
        _inController->onDestroy();
        Ogre::Root* root = _root.release();
        delete root;
        //_root->shutdown();

      }
    catch (Ogre::Exception e)
      {
        cout << "Exeception during shutdown: " << e.what() << endl;
      }
  }

  void
  EngineController::loadStates()
  {
    StatesLoader stLoader;
    GameStateInfo startState;
    stLoader.loadStates(_gameSInfoMap, startState);
    loadStartStateToCurrentState(startState.key);

  }

  bool
  EngineController::onKeyUp(const OIS::KeyEvent &event)
  {
    if (event.key == OIS::KC_ESCAPE)
      _stillRunning = false;
    _keyPump->updateKeyUpObs(event);
    return true;
  }

  bool
  EngineController::onKeyDown(const OIS::KeyEvent &event)
  {
    _keyPump->updateKeyDownObs(event);
    return true;
  }

  bool
  EngineController::onMouseMove(const OIS::MouseEvent &event)
  {
    _mousePump->updateMouseMoveEvt(event);
    return true;
  }

  bool
  EngineController::onMouseDown(const OIS::MouseEvent &event,
      const OIS::MouseButtonID id)
  {
    _mousePump->updateMouseDownEvt(event, id);
    return true;
  }

  bool
  EngineController::onMouseUp(const OIS::MouseEvent &event,
      const OIS::MouseButtonID id)
  {
    _mousePump->updateMouseUpEvt(event, id);
    return true;
  }

  void
  EngineController::loadStartStateToCurrentState(const string curKey)
  {
    Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL,
        "In loadStartStateToCurrentstate");
    ZGame::GameStateInfoMapItr it = _gameSInfoMap.find(curKey);
    if (it != _gameSInfoMap.end())
      {
        _curStateInfo.reset(&it->second);
        if (_curStateInfo->stateType == ZGame::GameStateInfo::STATELESS)
          {
            _lfcPump->removeAllObs(); //make sure we clear all LFC observers.
            _keyPump->removeAllObs();
            _curGameState.release();
          }
        else
          {
            //do stateful crap here.
          }
      }
    else
      throw(std::invalid_argument("Current State does not exist!"));

  }

  void
  EngineController::loadCurrentState(const string curKey)
  {
    Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL,
        "In load current state");
    ZGame::GameStateInfoMapItr it = _gameSInfoMap.find(curKey);
    if (it != _gameSInfoMap.end())
      {
        if (_curStateInfo->stateType == ZGame::GameStateInfo::STATELESS)
          {
            if (_curGameState.get() == 0)
              throw(invalid_argument(
                  "Current game state is null when trying to load a new STATELESS current state"));
            unloadCurrentState();
            _curStateInfo.reset(&it->second);
          }
        else
          {
            //do stateful crap here.
          }
      }
    else
      throw(std::invalid_argument("Current State does not exist!"));

  }
  void
  EngineController::unloadCurrentState()
  {
    _lfcPump->updateOnDestroyObs();
    _lfcPump->removeAllObs();
    _keyPump->removeAllObs();
    _mousePump->removeAllObs();
    _curGameState.release();
  }
  /**
   * This class realizes the current state. What it does is load the data pointed to by current state meta data.
   */
  void
  EngineController::realizeCurrentState()
  {
    using namespace ZGame;
    //attach the observers
    Ogre::LogManager* logM = Ogre::LogManager::getSingletonPtr();
    logM->logMessage(Ogre::LML_NORMAL, "In realizeCurrentState");
    logM->logMessage(Ogre::LML_NORMAL, "StateInfo: ");
    logM->logMessage(Ogre::LML_NORMAL, "Key: " + _curStateInfo->key);
    logM->logMessage(Ogre::LML_NORMAL, "Class: "
        + _curStateInfo->gameStateClass);
    if (_curStateInfo->stateType == GameStateInfo::STATEFUL)
      {
        //add to stateful
      }
    else
      {
        ZGame::EVENT::KeyboardEvtObserver keyObs;
        if (_curGameState.get() != 0)
          throw(invalid_argument(
              "Invalid current game state when realizing new state. Current game state is not null!"));
        _curGameState.reset(ZGame::GameStateFactory::createGameState(
            _curStateInfo->gameStateClass));

        //LifeCycleSubject
        LifeCycle::LifeCycleSubject lcs; //life cycle subject
        lcs.bind(&LifeCyclePump::addLifeCycleObserver, _lfcPump.get());
        //Keyboard subject
        EVENT::KeyEvtSubject ks; //keyboard subject
        ks.bind(&KeyboardPump::addKeyboardObserver, _keyPump.get());
        //Inject Mouse subject
        EVENT::MouseEvtSubject ms;
        ms.bind(&MousePump::addMouseObserver, _mousePump.get());

        //Registers for events
        LifeCycleRegister lfcReg;
        KeyEventRegister keyReg;
        MouseEventRegister mouseReg;

        _curGameState->init(lfcReg, keyReg, mouseReg);

        //inject subject to observers
        lfcReg.injectLfcSubj(lcs);
        keyReg.injectKeySubj(ks);
        mouseReg.injectMouseSubj(ms);
        logM->logMessage(Ogre::LML_TRIVIAL, "About to update onInit obs");
        _lfcPump->updateOnItObs(); //pump on init event to observers.

      }

  }

}
