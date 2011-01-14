/*
* EngineController.cpp
*
*  Created on: Aug 24, 2008
*      Author: bey0nd
*/
#include <iostream>
#include <stdexcept>
using namespace std;
#include "EngineController.h"
#include "GameStateFactory.h"
#include "StatesLoader.h"
#include "InputController.h"
#include "GameState.h"
#include "LifeCyclePump.h"
#include "KeyboardPump.h"
#include "MousePump.h"
#include "LifeCycleRegister.h"
#include "LifeCycleDelegates.h"
#include "KeyEventRegister.h"
#include "MouseEventRegister.h"
#include "MessageIdentifiers.h"
#include "ogreconsole.h"
#include "CommandController.h"
#include "DelegatesUtil.h"
#include "net/NetClientController.h"

#include "GraphicsController.h"
#include "gui/GuiController.h"
#include "GameMainState.h"
#include "entities/ZEntity.h"
#include "ControlModuleProto.h"
#include "world/WorldController.h"
#include "utilities/CharacterUtil.h"
#include "entities/EntitiesManager.h"
#include "entities/RenderEntitiesManager.h"
#include "entities/ZEntityBuilder.h"
#include "ZCL/ZCLController.h"
#include "GraphicsController.h"
#include "entities/EntitiesView.h"
#include "ZWorkspace.h"
#include "ZWorkspaceController.h"

#include "world/CinematicController.h"
#include "world/CinematicManager.h"

#include "command/CommandList.h"
#include "CommandController.h"


#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
static const Ogre::String PlatformPath("configs_windows/");
#else 
static const Ogre::String PlatformPath("configs_linux/");
#endif
namespace ZGame
{

    EngineController::EngineController() :
MainController(), _stillRunning(true), _lfcPump(0), _keyPump(0), _mousePump(0), _curStateInfo(0),
    _curGameState(0), _statsClockVariable(0), _sdkTrayMgr(0), _switchingState(false), _vp(0), _scnMgr(0),
    _commandController(new CommandController()), _startEngine(true), _cineController(0)
{
    // TODO Auto-generated constructor stub
    _listenerID = "EngineControllerListenerID";
}

EngineController::~EngineController()
{

    onDestroy();
    _commandController.reset(0);
    // TODO Auto-generated destructor stub
    _gameSInfoMap.clear();
    cout << "EngineController destructor" << "\n";

}




void
    EngineController::transitionState(const Ogre::String key)
{
    try
    {
        loadCurrentState(key);
        realizeCurrentState();
    }
    catch (std::invalid_argument e)
    {
        Ogre::Log::Stream sstream = Ogre::LogManager::getSingleton().stream();
        sstream << "Exception: " << e.what() << "\n";
        sstream << "Transition state does not exist: " << key << "\n";
        sstream << "Transitioning from state: " << _curStateInfo->key << "\n";
    }
}

Ogre::Camera*
    EngineController::createDefaultCamera(Ogre::Vector3 initialPos)
{
    using namespace Ogre;
    Camera* cam = 0;
    try
    {
        cam = _scnMgr->getCamera("ENGINE_VIEW_CAMERA"); //_scnMgr->createCamera("ENGINE_VIEW_CAMERA");
    }catch(Ogre::Exception e)
    {
        //Do nothing. I don't really get Ogre's exception system. How do I catch a particular exception. In this case,
        //we want to do nothing if we do not find the camera. Else throw the error up the chain.
    }
    if(cam == 0)
        cam = _scnMgr->createCamera("ENGINE_VIEW_CAMERA");
    cam->setNearClipDistance(0.5f);
    cam->setFarClipDistance(800.0f); //320.0f
    cam->setPosition(initialPos);
    //cam->rotate(Vector3(0.0f, 1.0f, 0.0f), Ogre::Radian(-Ogre::Math::PI / 2.0f));
    return cam;
}

void
    EngineController::updateStats()
{
    using std::cout;
    using std::endl;
    static String currFps = "Current FPS: ";
    static String avgFps = "Average FPS: ";
    static String bestFps = "Best FPS: ";
    static String worstFps = "Worst FPS: ";
    static String tris = "Triangle Count: ";
    static String batches = "Batch Count: ";


    // update stats when necessary
    try
    {

        const RenderTarget::FrameStats& stats = _window->getStatistics();

        if ((_statsClockVariable % 400) == 0)
        {
            cout << avgFps << ": " << StringConverter::toString(stats.avgFPS) << endl;
            cout << currFps << ": " << StringConverter::toString(stats.lastFPS) << endl;
            cout << bestFps << ": " << StringConverter::toString(stats.bestFPS) << endl;
            cout << tris << ": " << StringConverter::toString(stats.triangleCount) << endl;
            cout << batches << ": " << StringConverter::toString(stats.batchCount) << endl;
        }
        _statsClockVariable++;
    }
    catch (...)
    { /* ignore */
    }

}

void
    EngineController::loadSdkTrays()
{
    using namespace Ogre;
    using namespace OgreBites;

    _sdkTrayMgr.reset(new SdkTrayManager("ZombieTray", _window, _inController->getMouse(), 0));
}

void
    EngineController::_initCommands()
{
    using ZGame::CommandController;
    CommandController &cmdCtrl = CommandController::getSingleton();
    ZGame::COMMAND::ConsoleCommand cmd;
    cmd.bind(this, &EngineController::executeCmd);
    cmdCtrl.addCommand(CommandList::ENGINECTRL_CMD, cmd.GetMemento());
}

bool
    EngineController::loadStartStates()
{
    //load states
    loadStates();
    return true;
}

bool
    EngineController::onInit()
{
    using namespace Ogre;
    cout << "EngineController::onInit()" << endl;
    //_root = new Ogre::Root("plugins.cfg");

    _root.reset(new Ogre::Root(PlatformPath + "plugins.cfg", "pchaos.cfg", "Engine.log"));
    _root->getWorkQueue()->setResponseProcessingTimeLimit(0);
    //Note: We should pass in a stream to this which appends to App.log.
    Ogre::LogManager::getSingleton().createLog("App.log");
    if(_startEngine) 
    {

        if (_root->showConfigDialog())
        {
            _window = _root->initialise(true);
        }
        else
            return false;
        _startEngine = false;
    }
    else
    {
        if(_root->restoreConfig())
        {
            _window = _root->initialise(true);
        }
        else
            return false;
    }


    chooseSceneManager();

    _initCommands();

    //input
    _inController.reset(new InputController());
    _inController->onInit(_window);
    injectInputSubject();

    //bootstrap Sdk resources.
    loadAssets(PlatformPath + "bootstrap.cfg");
    loadSdkTrays();
    _sdkTrayMgr->showLoadingBar();

    loadAssets(PlatformPath + "resources.cfg");

    //Ogre::ResourceGroupManager::getSingleton().loadResourceGroup("Popular");
    Ogre::ResourceGroupManager::getSingleton().loadResourceGroup("PROJECT_ZOMBIE");

    Ogre::LogManager* lm = LogManager::getSingletonPtr();
    lm->setLogDetail(Ogre::LL_NORMAL);

    lm->logMessage(Ogre::LML_TRIVIAL, "States finished loading");

    lm->logMessage(Ogre::LML_TRIVIAL, "Injected input.");

    _root->addFrameListener(this);

    lm->logMessage(Ogre::LML_TRIVIAL, "Starting multiplayer engine!");

    //let's init the console now.
    lm->logMessage(Ogre::LML_NORMAL, "initializing console.");

    initConsole();

    _netClient.reset(new ZGame::Networking::NetClientController());

    _sdkTrayMgr->hideLoadingBar();
    _sdkTrayMgr->hideAll();
    _scnMgr->addRenderQueueListener(this);


    return true;
}

void
    EngineController::chooseSceneManager()
{
    bool notFound = true;
    _scnMgr = _root->createSceneManager(Ogre::ST_EXTERIOR_REAL_FAR, "ProjectChaos");
    RenderQueue* rdrQueue = _scnMgr->getRenderQueue();
    rdrQueue->setDefaultQueueGroup(Ogre::RENDER_QUEUE_MAIN);

}

void
    EngineController::injectInputSubject()
{
    ZGame::EVENT::KeyboardEvtObserver keyObs;
    keyObs.kde.bind(this, &ZGame::EngineController::onKeyDown);
    keyObs.kue.bind(this, &ZGame::EngineController::onKeyUp);
    _inController->addKeyListeners(_listenerID, keyObs);
    ZGame::EVENT::MouseEvtObserver mouseObs;
    mouseObs.mde.bind(this, &ZGame::EngineController::onMouseDown);
    mouseObs.mue.bind(this, &ZGame::EngineController::onMouseUp);
    mouseObs.mme.bind(this, &ZGame::EngineController::onMouseMove);
    _inController->addMouseListeners(_listenerID, mouseObs);
}

void
    EngineController::loadAssets(Ogre::String filename)
{
    Ogre::ConfigFile cf;
    cf.load(filename);
    Ogre::String resourcePath("");
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
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(resourcePath + archName, typeName, secName);
        }
    }
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

}

bool
    EngineController::frameStarted(const Ogre::FrameEvent &evt)
{
    _lfcPump->updateOnFrameStarted(evt);
    return true;
}

bool
    EngineController::frameEnded(const Ogre::FrameEvent &evt)
{
    _lfcPump->updateOnFrameEnded(evt);
    return true;
}

bool
    EngineController::frameRenderingQueued(const Ogre::FrameEvent &evt)
{
    if (!_stillRunning)
        return false;
    if(_switchingState)
    {
        return false;
    }
    try
    {
        _inController->run();
        _lfcPump->updateOnUpdateObs(evt);
        //updateStats();
    }
    catch (Ogre::Exception e)
    {
        throw e;
    }
    return true;
}

void
    EngineController::renderQueueStarted(Ogre::uint8 queueGroupId,
    const Ogre::String& invocation, bool& skipThisInvocation)
{
    _lfcPump->updateOnRenderQueueStartObs(queueGroupId, invocation, skipThisInvocation);
}

void
    EngineController::renderQueueEnded(Ogre::uint8 queueGroupId,
    const Ogre::String& invocation, bool& skipThisInvocation)
{
    _lfcPump->updateOnRenderQueueEndObs(queueGroupId, invocation, skipThisInvocation);
}

void
    EngineController::run()
{
    realizeCurrentState();
    while(_stillRunning)
    {
        _root->startRendering();
        if(_switchingState)
        {
            _doSwitchingState();
        }
    }
    //_root->renderOneFrame();
}

void
    EngineController::onDestroy()
{
    MainController::onDestroy();
    Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL, "EngineController.onDestroy()");
    try
    {
        _inController->onDestroy();
        _inController.reset(0);
        unloadCurrentState();
        _lfcPump.reset(0);
        _keyPump.reset(0);
        _mousePump.reset(0);
        _netClient.reset(0);
        _sdkTrayMgr.reset(0);
        _commandController->onDestroy(); //command controller is singleton. It shouldn't have state. This is due to OgreConsole coupling inside CommandController,
        _cineController.reset(0);
        _gfxCtrl.reset(0);
        _guiCtrl.reset(0);
        _root.reset(0);
        _statsClockVariable = 0;
        _vp = 0;
        _scnMgr = 0;
    }
    catch (Ogre::Exception e)
    {
        cout << "Exeception during shutdown: " << e.what() << "\n";
    }
}

void
    EngineController::loadStates()
{
    StatesLoader stLoader;
    GameStateInfo startState;
    stLoader.loadStates(_gameSInfoMap, startState);
    cout << "startState.key: " << startState.key << "\n";
    loadStartStateToCurrentState(startState.key);
}

bool
    EngineController::onKeyUp(const OIS::KeyEvent &event)
{
    _keyPump->updateKeyUpObs(event);
    if (event.key == OIS::KC_ESCAPE)
    {
        _stillRunning = false;
        return false;
    }
    return true;
}

bool
    EngineController::onKeyDown(const OIS::KeyEvent &event)
{
    //We assume here the ogre console exists (has been created and attached to CommandController). 
    //This SHOULD be the case for EngineController, because we
    //created the console in the initialization code in this controller.
    OgreConsole* ogreConsole = CommandController::getSingleton().getConsole();
    bool consoleVis = ogreConsole->isVisible();
    //console
    if (event.key == OIS::KC_GRAVE)
    {
        if (consoleVis)
        {
            //turn off console
            ogreConsole->setVisible(false);
        }
        else
            ogreConsole->setVisible(true);
    }
    else if(event.key == OIS::KC_SYSRQ)
    {
        _window->writeContentsToTimestampedFile("zombie_shot",".png");
    }
    else if(event.key == OIS::KC_TAB)
    {
        //if(_controlMod.get() != 0)
        if(_controlMod->isEnabled())
        {
            _controlMod->disable(true);
        }
        else
            _controlMod->disable(false);
    }
    ogreConsole->onKeyPressed(event);
    
    if (!consoleVis)
    {
        _keyPump->updateKeyDownObs(event);
    }
    return true;
}

bool
    EngineController::onMouseMove(const OIS::MouseEvent &event)
{
    /*
    * Note: It is ugly to intercept trays events this way. The original plan is to have the concept of intercepting events. We need to implement
    * that using Delegates. This way, we can filter and intercept events. Bascially we need to better manage this. Perhaps by having a pump at the game state
    * level and implement a handler which wraps the pumps and is reactive to events.
    */
    _mousePump->updateMouseMoveEvt(event);
    return true;
}

bool
    EngineController::onMouseDown(const OIS::MouseEvent &event, const OIS::MouseButtonID id)
{
    _mousePump->updateMouseDownEvt(event, id);
    return true;
}

bool
    EngineController::onMouseUp(const OIS::MouseEvent &event, const OIS::MouseButtonID id)
{
    _mousePump->updateMouseUpEvt(event, id);
    return true;
}

void
    EngineController::loadStartStateToCurrentState(const Ogre::String curKey)
{
    //Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL, "In loadStartStateToCurrentstate");

    for (ZGame::GameStateInfoMapItr it = _gameSInfoMap.begin(); it != _gameSInfoMap.end(); ++it)
    {
        cout << "info keys,keys: " << it->first << " " << it->second.key << "\n";
    }

    ZGame::GameStateInfoMapItr it = _gameSInfoMap.find(curKey);
    if (it != _gameSInfoMap.end())
    {
        _curStateInfo = &it->second;
        //_curStateInfo.reset(&it->second);
        if (_curStateInfo->stateType == ZGame::GameStateInfo::STATELESS)
        {
            _curGameState.reset(0); //delete current game state
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
    EngineController::loadCurrentState(const Ogre::String curKey)
{
    Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL, "In load current state");
    ZGame::GameStateInfoMapItr it = _gameSInfoMap.find(curKey);
    if (it != _gameSInfoMap.end())
    {
        if (_curStateInfo->stateType == ZGame::GameStateInfo::STATELESS)
        {
            //_curStateInfo.reset(&it->second);
            _curStateInfo = &it->second;
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
    cout << "-------------------Unloading current state-----------------" << endl;
    _lfcPump->updateOnDestroyObs();
    _lfcPump->removeAllObs();
    cout << "All observers from life cycle pump removed." << endl;
    _keyPump->removeAllObs();
    cout << "All observers from key pump removed." << endl;
    _mousePump->removeAllObs();
    cout << "All observers from mouse pump removed." << endl;
    _curGameState.reset(0);
    cout << "Reset all modules" << endl;
    _removeSubSystemsOnUnloadState();
    cout << "--------------------------------------------------------------" << endl;
}
/**
* This class realizes the current state. What it does is load the data pointed to by current state meta data.
*/
void
    EngineController::realizeCurrentState()
{
    using namespace ZGame;

    _lfcPump.reset(new LifeCyclePump());
    _mousePump.reset(new MousePump());
    _keyPump.reset(new KeyboardPump());

    //attach the observers
    Ogre::LogManager* logM = Ogre::LogManager::getSingletonPtr();
    logM->logMessage(Ogre::LML_NORMAL, "In realizeCurrentState");
    logM->logMessage(Ogre::LML_NORMAL, "StateInfo: ");
    logM->logMessage(Ogre::LML_NORMAL, "Key: " + _curStateInfo->key);
    logM->logMessage(Ogre::LML_NORMAL, "Class: " + _curStateInfo->gameStateClass);



    if (_curStateInfo->stateType == GameStateInfo::STATEFUL)
    {
        //add to stateful
    }
    else
    {
        ZGame::EVENT::KeyboardEvtObserver keyObs;
        if (_curGameState.get() != 0)
            throw(invalid_argument("Invalid current game state when realizing new state. Current game state is not null!"));
        _curGameState.reset(ZGame::GameStateFactory::createGameState(_curStateInfo->gameStateClass));
        GameStateBootstrapInfo info;
        //Note: The camera system is not fully implemented, that's why we have this initial camera business.
        _curGameState->getGameStateBootstrapInfo(info);

        //Ogre::Camera* cam = createDefaultCamera(info.initalCameraPos);

        //_vp = _window->addViewport(cam);
        //_vp->setOverlaysEnabled(true);
        //_vp->setBackgroundColour(Ogre::ColourValue(0.3f, 0.0f, 0.0f));

        //cam->setAspectRatio(Real(_vp->getActualWidth()) / Real(_vp->getActualHeight()));

        //LifeCycleSubject
        LifeCycle::LifeCycleSubject lcs; //life cycle subject
        lcs.bind(_lfcPump.get(), &LifeCyclePump::addLifeCycleObserver);
        //Keyboard subject
        EVENT::KeyEvtSubject ks; //keyboard subject
        ks.bind(_keyPump.get(), &KeyboardPump::addKeyboardObserver);
        //Inject Mouse subject
        EVENT::MouseEvtSubject ms;
        ms.bind(_mousePump.get(), &MousePump::addMouseObserver);

        //Registers for events
        LifeCycleRegister lfcReg;
        KeyEventRegister keyReg;
        MouseEventRegister mouseReg;

        _curGameState->init(lfcReg, keyReg, mouseReg, 0);

        _initSubSystemsOnLoadState(info, lfcReg, keyReg, mouseReg, *_curGameState.get());

        _initPacket = new ZInitPacket(&info, _scnMgr, _cineController->getCinematicManager()->getRootCam(), 
            _window,
            _inController->getKeyboard(), _workspaceCtrl.get(), 
            _gfxCtrl.get(), _guiCtrl.get());



        LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL, "Current game state done init.");
        //We manually register the net client for now. We do this
        //since we have not implemented Stateful states. The original
        //idea for Stateful states is so we use Stateful states as
        //services, services which are used by rest of the system (hence theya are stateful. I.E:
        //services are stateful objects which exist for the duration of the engine.
        manuallyRegisterNetClient(lfcReg);

        //inject subject to observers
        lfcReg.injectLfcSubj(lcs);
        keyReg.injectKeySubj(ks);
        mouseReg.injectMouseSubj(ms);
        logM->logMessage(Ogre::LML_TRIVIAL, "About to update onInit obs");

        if(!_initPacket->valid())
            OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Init packet is not valid",
            "EngineController::realizeCurrentState");

        //Gui Configuration
        _gfxCtrl->onInit(_initPacket); 
        _guiCtrl->onInit(_initPacket); //manual init GUI controller;

        _curGameState->onGuiConfiguration(_guiCtrl.get());
        _lfcPump->updateOnItObs(_initPacket); //pump on init event to observers.

        delete _initPacket;
    }
    logM->logMessage(Ogre::LML_NORMAL, "Realizing current state done");
}

void
    EngineController::initConsole()
{
    auto_ptr<OgreConsole> console(new OgreConsole());
    console->init(_root.get());
    CommandController::getSingleton().attachConsole(console); //CommandConstroller should exist by now. CommandController is created in EngineController (it's a service.)
}

void
    EngineController::manuallyRegisterNetClient(LifeCycleRegister &lfcReg)
{
    LifeCycle::LifeCycleObserver lfcObs;
    try
    {
        LifeCycle::bindAndRegisterLifeCycleObserver<ZGame::Networking::NetClientController>(lfcReg, lfcObs, *_netClient.get());
    }catch(Ogre::Exception e)
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, e.getDescription() + " in NetClientController.", "EngineController::manuallyRegisterNetClient");
    }
}

void
    EngineController::_initSubSystemsOnLoadState(const ZGame::GameStateBootstrapInfo &info, LifeCycleRegister &lfcReg,
    KeyEventRegister &keyReg, MouseEventRegister &mouseReg, GameState &gameState)
{
    try
    {
        LifeCycle::LifeCycleObserver lfcObs;
        EVENT::KeyboardEvtObserver keyObs;
        EVENT::MouseEvtObserver mouseObs;

        try
        {
            _gfxCtrl.reset(new GraphicsController());
            LifeCycle::bindAndRegisterLifeCycleObserver<ZGame::GraphicsController>(lfcReg, lfcObs, *_gfxCtrl,
                LifeCycle::LFC_ON_UPDATE | LifeCycle::LFC_ON_DESTROY | LifeCycle::LFC_ON_FRAME_ENDED);
            EVENT::bindAndRegisterKeyObserver(keyReg, keyObs, *_gfxCtrl);
        }catch(Ogre::Exception e)
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, e.getDescription() + " in GraphicsController.", "");
        }
        try
        {
            if(_guiCtrl.get() == 0)
            {
                _guiCtrl.reset(new Gui::GuiController());
            }
            LifeCycle::bindAndRegisterLifeCycleObserver<Gui::GuiController>(lfcReg, lfcObs, *_guiCtrl, LifeCycle::LFC_ON_UPDATE | LifeCycle::LFC_ON_DESTROY |
                LifeCycle::LFC_ON_RENDER_QUEUE_START | LifeCycle::LFC_ON_RENDER_QUEUE_END);

            EVENT::bindAndRegisterKeyObserver(keyReg, keyObs, *_guiCtrl);
            EVENT::bindAndRegisterMouseObserver(mouseReg, mouseObs, *_guiCtrl);

            gameState.onGuiConfiguration(_guiCtrl.get());

        }catch(Ogre::Exception e)
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, e.getFullDescription() + " in GuiController", "");
        }

        try
        {
            std::auto_ptr<World::CinematicManager> cineMgr(new World::CinematicManager(_scnMgr));
            _cineController.reset(new World::CinematicController(cineMgr, _window));
            gameState.onCinematicControllerConfiguration(_cineController.get());
            _vp = _cineController->getViewport();
        }catch(Ogre::Exception e)
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, e.getFullDescription() + " in CinematicController",
                "");
        }


        try
        {
            if(info.requireRenderEntitiesmanager)
            {
                _rdrEntMgr.reset(new Entities::RenderEntitiesManager());
                //render entities
                LifeCycle::bindAndRegisterLifeCycleObserver<Entities::RenderEntitiesManager>(lfcReg,
                    lfcObs, *_rdrEntMgr, LifeCycle::LFC_ON_INIT);
            }
        }catch(Ogre::Exception e)
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, e.getFullDescription() + " in RenderEntitiesManager", "");
        }

        try
        {
            if(info.requireZCLController)
            {
                _zclCtrl.reset(new ZGame::ZCL::ZCLController());
                //OpenCLController
                LifeCycle::bindAndRegisterLifeCycleObserver<ZGame::ZCL::ZCLController>(lfcReg, 
                    lfcObs, *_zclCtrl, LifeCycle::LFC_ON_DESTROY);
            }
        }catch(Ogre::Exception e)
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, e.getFullDescription() + " in ZCLController", "");
        }
        try
        {
            if(info.requireWorldController)
            {
                _worldController.reset(new ZGame::World::WorldController);
                //world controller
                LifeCycle::bindAndRegisterLifeCycleObserver<ZGame::World::WorldController>(lfcReg, 
                    lfcObs, *_worldController);

                gameState.onWorldControllerConfiguration(_worldController.get());
            }
        }catch(Ogre::Exception e)
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, e.getFullDescription() + " in WorldController", "");
        }
        try
        {
            //CONTROL MODULE
            if(info.requireControlModule)
            {
                _controlMod.reset(new ZGame::ControlModuleProto());
                //control module
                LifeCycle::bindAndRegisterLifeCycleObserver<ZGame::ControlModuleProto>(lfcReg, lfcObs, *_controlMod,
                    LifeCycle::LFC_ON_INIT | LifeCycle::LFC_ON_UPDATE | LifeCycle::LFC_ON_DESTROY);
                EVENT::bindAndRegisterKeyObserver(keyReg, keyObs, *_controlMod);
                EVENT::bindAndRegisterMouseObserver(mouseReg, mouseObs, *_controlMod);
            }
        }catch(Ogre::Exception e)
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, e.getFullDescription() + " in ControlModuleProto", "");
        }
        try
        {
            //WORKSPACE
            if(info.requireWorkspace)
            {
                _workspace.reset(new ZWorkspace(_scnMgr, _entMgr.get(), _rdrEntMgr.get(), 0, _zclCtrl.get(), _worldController.get(),
                    _cineController.get()));
                _workspaceCtrl.reset(new ZGame::ZWorkspaceController);
                _workspaceCtrl->setZWorkspace(_workspace.get());
                //Workspace controller
                LifeCycle::bindAndRegisterLifeCycleObserver<ZGame::ZWorkspaceController>(lfcReg, lfcObs, *_workspaceCtrl,
                    LifeCycle::LFC_ON_INIT | LifeCycle::LFC_ON_DESTROY);
                EVENT::bindAndRegisterKeyObserver(keyReg, keyObs, *_workspaceCtrl);
                EVENT::bindAndRegisterMouseObserver(mouseReg, mouseObs, *_workspaceCtrl);

            }
        }catch(Ogre::Exception e)
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, e.getFullDescription() + " in ZWorkspaceController", "");
        }
        try
        {
            if(info.requireCharacterUtil)
            {
                _charUtil.reset(new ZGame::Util::CharacterUtil());
                LifeCycle::bindAndRegisterLifeCycleObserver<ZGame::Util::CharacterUtil>(lfcReg, lfcObs, *_charUtil,
                    LifeCycle::LFC_ON_INIT);
            }
        }catch(Ogre::Exception e)
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, e.getFullDescription() + " in CharacterUtil", "");
        }
    }catch(Ogre::Exception e)
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, e.getFullDescription(),
            "EngineController::_initSubSystemOnLoadState");
    }

}

void
    EngineController::_removeSubSystemsOnUnloadState()
{
    _gfxCtrl.reset(0);
    _rdrEntMgr.reset(0);
    _zclCtrl.reset(0);
    _worldController.reset(0);
    _controlMod.reset(0);
    _workspaceCtrl.reset(0);
    _workspace.reset(0);
    _charUtil.reset(0);
}

bool
    EngineController::executeCmd(const Ogre::StringVector& params)
{
    const Ogre::String switchState("switchstate");
    const Ogre::String quit("quit");
    //the command starts at [1] because [0] contains the "command" key which is engctrl_execute in this case.
    if(switchState.compare(params[1]) == 0)
    {
        if(params.size() == 3)
            _switchState(params[2]);
        else
            OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "the command is invalid. not enough parameters for switchstate", 
            "EngineController::executeCmd");
    }
    else if(quit.compare(params[1]) == 0)
    {
        _stillRunning = false;
    }
    else
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "the command is invalid.",
        "EngineController::executeCmd");

    return true;
}

/**
* This method will switch the state to the given state in the string state.
**/
void
    EngineController::_switchState(const Ogre::String &state)
{
    //The design for this is a bit wonky at the moment as in this method, there is no state value mapping 
    //to the states map. You will have to look at state.cfg to find the values for the actual keys. Do not
    //mistype them here. And if you add new key values in state.cfg, you will have to update it here also.
    const Ogre::String menuState("mainmenu");
    const Ogre::String mainState("maingamestate");
    const Ogre::String editState("maineditstate");

    if(mainState.compare(state) == 0)
    {
        _switchingState = true;
        _switchToStateKey = "GameMainStateKey";
        return;
    }
    else if(menuState.compare(state) == 0)
    {
        _switchingState = true;
        _switchToStateKey = "GameMainMenuStateKey";
        return;
    }
    else if(editState.compare(state) == 0)
    {
        _switchingState = true;
        _switchToStateKey = "GameEditStateKey";
        return;
    }

    OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "cannot switch to invalid state", 
        "EngineController::_switchState");
}

void
    EngineController::_doSwitchingState()
{
    _switchingState = false;
    onDestroy();
    if(!onInit())
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, "Failed to reinitialize engine!",
        "EngineController::_doSwitchingState");
    transitionState(_switchToStateKey);
}



}
