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
#include "KeyEventRegister.h"
#include "MouseEventRegister.h"
#include "MessageIdentifiers.h"
#include "ogreconsole.h"
#include "CommandController.h"
#include "DelegatesUtil.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
static const Ogre::String PlatformPath("configs_windows/");
#else 
static const Ogre::String PlatformPath("configs_linux/");
#endif
namespace ZGame
{

    EngineController::EngineController() :
MainController(), _stillRunning(true), _lfcPump(new LifeCyclePump()), _keyPump(new KeyboardPump()), _mousePump(new MousePump()), _curStateInfo(0),
    _curGameState(0), _statsClockVariable(0), _sdkTrayMgr(0)
{
    // TODO Auto-generated constructor stub
    _listenerID = "EngineControllerListenerID";
}

EngineController::~EngineController()
{
    onDestroy();
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
    //Camera* cam =_scnMgr->createCamera(_window->getName());
    Camera* cam = _scnMgr->createCamera("ENGINE_VIEW_CAMERA");
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
    //bootstrap Sdk resources.
    loadAssets(PlatformPath + "bootstrap.cfg");
    _sdkTrayMgr.reset(new SdkTrayManager("ZombieTray", _window, _inController->getMouse(), 0));
}

bool
    EngineController::onInit()
{
    using namespace Ogre;
    cout << "EngineController::onInit()" << endl;
    //_root = new Ogre::Root("plugins.cfg");
    _root.reset(new Ogre::Root(PlatformPath + "plugins.cfg", "pchaos.cfg", "Engine.log"));
    _root->getWorkQueue()->setResponseProcessingTimeLimit(0);
    //_root->setWorkQueue(OGRE_NEW Ogre::DefaultWorkQueue("DefaultWorkerQueue"));
    Ogre::LogManager::getSingleton().createLog("App.log");

    if (_root->showConfigDialog())
    {
        _window = _root->initialise(true);
    }
    else
        return false;

    chooseSceneManager();

    //input
    _inController.reset(new InputController());
    _inController->onInit(_window);
    injectInputSubject();

    loadSdkTrays();
    //turn off loading bar
    _sdkTrayMgr->showLoadingBar();

    loadAssets(PlatformPath + "resources.cfg");

    //Ogre::ResourceGroupManager::getSingleton().loadResourceGroup("Popular");
    Ogre::ResourceGroupManager::getSingleton().loadResourceGroup("PROJECT_ZOMBIE");

    //load states
    loadStates();

    _initPacket = new ZInitPacket(_scnMgr, 0, _window);


    Ogre::LogManager* lm = LogManager::getSingletonPtr();
    lm->setLogDetail(Ogre::LL_NORMAL);

    lm->logMessage(Ogre::LML_TRIVIAL, "States finished loading");

    lm->logMessage(Ogre::LML_TRIVIAL, "Injected input.");

    _root->addFrameListener(this);

    lm->logMessage(Ogre::LML_TRIVIAL, "Starting multiplayer engine!");

    //let's init the console now.
    lm->logMessage(Ogre::LML_NORMAL, "initializing console.");
    initConsole();

    lm->logMessage(Ogre::LML_NORMAL, "Initializing debug overlay.");
    _debugOverlay = OverlayManager::getSingleton().getByName("Core/DebugOverlay");

    //Create the NetClient. Note: This is place-holder code until we get the "service" framework.
    //Everything that uses CommandController depends on Console being initialized. This is bad, need to fix this ASAP.
    //The fix should not be that hard, though. (Requires no major refactoring.)
    _netClient.reset(new ZGame::Networking::NetClientController());

    _sdkTrayMgr->hideLoadingBar();

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
    EngineController::frameRenderingQueued(const Ogre::FrameEvent &evt)
{
    if (!_stillRunning)
        return false;
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
    //_lfcPump->updateOnRenderQueueStartObs(queueGroupId, invocation, skipThisInvocation);
}

void
    EngineController::renderQueueEnded(Ogre::uint8 queueGroupId,
    const Ogre::String& invocation, bool& skipThisInvocation)
{
    _lfcPump->updateOnRenderQueueStartObs(queueGroupId, invocation, skipThisInvocation);
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
    MainController::onDestroy();
    Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL, "EngineController.onDestroy()");
    try
    {
        _inController->onDestroy();
        unloadCurrentState();

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
    //We assume here the ogre console exists (has been created and attached to CommandController). This SHOULD be the case for EngineController, because we
    //created the console in the initialization code in this controller.
    OgreConsole* ogreConsole = CommandController::getSingleton().getConsole();
    bool consoleVis = ogreConsole->isVisible();
    //console
    if (event.key == OIS::KC_GRAVE)
    {
        /*
        if(_vp->getOverlaysEnabled())
            _vp->setOverlaysEnabled(false); 
        else
            _vp->setOverlaysEnabled(true);
            */
        
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

    ogreConsole->onKeyPressed(event);
    //OgreConsole::getSingleton().onKeyPressed(event);
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
    Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL, "In loadStartStateToCurrentstate");

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
            _lfcPump->removeAllObs(); //make sure we clear all LFC observers.
            _keyPump->removeAllObs();
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
            if (_curGameState.get() == 0)
                throw(invalid_argument("Current game state is null when trying to load a new STATELESS current state"));
            unloadCurrentState();
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
    cout << "--------------------------------------------------------------" << endl;
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
        _curGameState->getGameStateBootstrapInfo(info);
        Ogre::Camera* cam = createDefaultCamera(info.initalCameraPos);
        _vp = _window->addViewport(cam);

        _vp->setOverlaysEnabled(true);
        _vp->setBackgroundColour(Ogre::ColourValue(0.3f, 0.0f, 0.0f));

        cam->setAspectRatio(Real(_vp->getActualWidth()) / Real(_vp->getActualHeight()));

        _initPacket->initialCamera = cam;

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

        _lfcPump->updateOnItObs(*_initPacket); //pump on init event to observers.

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

}
