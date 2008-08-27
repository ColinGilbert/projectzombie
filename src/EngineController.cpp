/*
 * EngineController.cpp
 *
 *  Created on: Aug 24, 2008
 *      Author: bey0nd
 */

#include <string>
#include <iostream>
using namespace std;


#include "EngineController.h"
#include "EngineView.h"

namespace ZGame
{

  EngineController::EngineController() : _root(0),_scnMgr(0),_stillRunning(true),_engineView(0)
  {
    // TODO Auto-generated constructor stub
    _listenerID = "EngineControllerListenerID";
  }

  EngineController::~EngineController()
  {
    // TODO Auto-generated destructor stub
  }

  void EngineController::transitionState()
  {
  }

  Ogre::Camera* EngineController::createDefaultCamera()
  {
    using namespace Ogre;
    Camera* cam =_scnMgr->createCamera(_window->getName());
    cam->setPosition(2000,2000,2000);
    cam->lookAt(0,0,0);
    return cam;
  }

  bool EngineController::onInit()
  {
    _root = new Ogre::Root("plugins.cfg");
    if(_root->showConfigDialog())
      {
        _window = _root->initialise(true);
      }
    else
      return false;
    _scnMgr = _root->createSceneManager(Ogre::ST_GENERIC);

    loadAssets();
    Ogre::Camera* cam = createDefaultCamera();

    _engineView = new ZGame::EngineView(_window,cam);

    return true;
  }

  void EngineController::injectInputSubject(ZGame::InputController* inControl)
  {
    ZGame::EVENT::KeyboardEvtObserver keyObs;
    keyObs.kde.bind(&ZGame::EngineController::onKeyDown,this);
    keyObs.kue.bind(&ZGame::EngineController::onKeyUp,this);
    inControl->addKeyListeners(_listenerID,keyObs);
  }


  void EngineController::loadAssets()
  {
    Ogre::ConfigFile cf;
    string resourcePath;
    resourcePath = "";
    cf.load(resourcePath + "resources.cfg");
    //go thourhg all sections & settings in the file
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

    Ogre::String secName, typeName, archName;
    while(seci.hasMoreElements())
      {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for(i=settings->begin();i!=settings->end();++i)
          {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                resourcePath + archName, typeName, secName);
          }
      }
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
  }

  void EngineController::run()
  {
    while(_stillRunning)
      {
        _root->renderOneFrame();
        //boost::thread::sleep(0);
      }
  }

  void EngineController::onDestroy()
  {
    if(_root)
      delete _root;
  }

  void EngineController::loadStates()
  {
  }

  void EngineController::addKeyboardListener(ZGame::EVENT::KeyboardEvtObserver keo)
  {

  }
  void EngineController::addMouseListener(ZGame::EVENT::MouseEvtObserver meo)
  {

  }

  bool EngineController::onKeyUp(const OIS::KeyEvent &event)
  {
    cout << "In EngineController::onKeyUp" << endl;
    if(event.key == OIS::KC_ESCAPE)
      _stillRunning = false;
    return true;
  }

  bool EngineController::onKeyDown(const OIS::KeyEvent &event)
  {
    return true;
  }






}
