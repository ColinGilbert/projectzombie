/*
* EngineController.cpp
*
*  Created on: Aug 24, 2008
*      Author: bey0nd
*/

//#include <string>
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
#include "MessageIdentifiers.h"
#include "ogreconsole.h"
#include "CommandController.h"
#include "DelegatesUtil.h"

namespace ZGame
{

    EngineController::EngineController() : MainController(),
_stillRunning(true), _lfcPump(new LifeCyclePump()), _keyPump(
    new KeyboardPump()), _mousePump(new MousePump()),
    _curStateInfo(0),_curGameState(0)
{
    // TODO Auto-generated constructor stub
    _listenerID = "EngineControllerListenerID";
}

EngineController::~EngineController()
{
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
EngineController::createDefaultCamera()
{
    using namespace Ogre;
    //Camera* cam =_scnMgr->createCamera(_window->getName());
    Camera* cam = _scnMgr->createCamera("ENGINE_VIEW_CAMERA");
    cam->setNearClipDistance(0.01f); 
    return cam;
}

bool
EngineController::onInit()
{
    using namespace Ogre;
    cout << "EngineController::onInit()" << endl;
    //_root = new Ogre::Root("plugins.cfg");
    _root.reset(new Ogre::Root("plugins.cfg","plsm2_display.cfg","Pchaos.log"));
    if (_root->showConfigDialog())
    {
        _window = _root->initialise(true);
    }
    else
        return false;

    loadAssets();
    chooseSceneManager();

    Ogre::Camera* cam = createDefaultCamera();
    Ogre::Viewport* vp = _window->addViewport(cam);
    vp->setBackgroundColour(Ogre::ColourValue(0.3f, 0.0f, 0.0f));

    cam->setAspectRatio(Real(vp->getActualWidth())
        / Real(vp->getActualHeight()));

    _engineView.reset(new ZGame::EngineView(_window, cam, _scnMgr));

    //set logging lvl
    Ogre::LogManager::getSingleton().setLogDetail(Ogre::LL_BOREME);

    //load states
    loadStates();

    Ogre::LogManager* lm = LogManager::getSingletonPtr();

    lm->logMessage(Ogre::LML_TRIVIAL,"States finished loading");


    //input
    _inController.reset(new InputController());
    _inController->onInit(_window);
    injectInputSubject();
    lm->logMessage(Ogre::LML_TRIVIAL,"Injected input.");

    _root->addFrameListener(this);

    lm->logMessage(Ogre::LML_TRIVIAL,"Starting multiplayer engine!");

    //let's init the console now.
    lm->logMessage(Ogre::LML_NORMAL,"initializing console.");
    initConsole();

    //Create the NetClient. Note: This is place-holder code until we get the "service" framework. 
    //Everything that uses CommandController depends on Console being initialized. This is bad, need to fix this ASAP.
    //The fix should not be that hard, though. (Requires no major refactoring.)
    _netClient.reset(new ZGame::Networking::NetClientController());


    return true;
}

void
EngineController::chooseSceneManager()
{
    bool notFound = true;
    /*
    SceneManagerEnumerator::MetaDataIterator it = _root->getSceneManagerMetaDataIterator();
    while(it.hasMoreElements())
    {
    const SceneManagerMetaData *metaData = it.getNext();
    if(metaData->sceneTypeMask == ST_EXTERIOR_REAL_FAR &&
    metaData->worldGeometrySupported == true &&
    metaData->typeName == "PagingLandScapeSceneManager")
    {
    notFound = false;
    break;
    }
    }*/
    /*
    if(notFound)
    {
    OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, "Could not find Paging Landscape plugin. Check if it is in plugin.cfg",
    "chooseSceneManager");
    }*/
    _scnMgr = _root->createSceneManager(Ogre::ST_GENERIC, "ProjectChaos");
    //_scnMgr = _root->createSceneManager("PagingLandScapeSceneManager","ProjectChaos");
    RenderQueue* rdrQueue = _scnMgr->getRenderQueue();
    rdrQueue->setDefaultQueueGroup(Ogre::RENDER_QUEUE_MAIN);
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
    Ogre::String resourcePath;
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
    MainController::onDestroy();
    Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL,
        "EngineController.onDestroy()");
    try
    {
        _inController->onDestroy();
        //CommandController::getSingleton().onDestroy();
        //CLEAR _curStateInfo manaually. THIS clearly is a hack.
        //The reason being you are using auto_ptr to store the current state information, which you set by 
        //getting the reference from from GameStateInfoMap;  So when destructing,
        //the order of destruction (think auto pointer and actual object on the stack) matters. Currently
        //_curStateInfo is being destructured (it's auto_ptr) after GameStateInfoMap is destryoed. This obviously
        //is going to crash because it is pointing to the already destructed GameStateInfoMap. STUPID!
        //Solution: Use shared pointer or ...but whatever you do do not put put a pointer in an auto_ptr that is pointing to
        //some other crap (setting using & operator.)
        //Ogre::Root* root = _root.release();
        //delete root;
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
        unloadCurrentState();
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
    if(event.key == OIS::KC_TAB)
    {
        if(consoleVis)
        {
            //turn off console
            ogreConsole->setVisible(false);
        }
        else
            ogreConsole->setVisible(true);
    }

    ogreConsole->onKeyPressed(event);
    //OgreConsole::getSingleton().onKeyPressed(event);
    if(!consoleVis)
    {
        _keyPump->updateKeyDownObs(event);
    }
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
EngineController::loadStartStateToCurrentState(const Ogre::String curKey)
{
    Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL,
        "In loadStartStateToCurrentstate");

    for(ZGame::GameStateInfoMapItr it = _gameSInfoMap.begin(); it != _gameSInfoMap.end(); ++it)
    {
        cout << "info keys,keys: " << it->first << " " <<
            it->second.key << "\n";
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
    _lfcPump->updateOnDestroyObs();
    _lfcPump->removeAllObs();
    _keyPump->removeAllObs();
    _mousePump->removeAllObs();
    _curGameState.reset(0);
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
        _lfcPump->updateOnItObs(); //pump on init event to observers.

    }
    logM->logMessage(Ogre::LML_NORMAL,"Realizing current state done");
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
    LifeCycle::bindLifeCycleObserver(lfcObs,*_netClient.get());
    lfcReg.registerLfcObs(lfcObs);
    LifeCycle::clearLfcObs(lfcObs);
}

}
