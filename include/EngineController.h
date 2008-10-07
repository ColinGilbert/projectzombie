/*
 * EngineController.h
 *
 *  Created on: Aug 24, 2008
 *      Author: bey0nd
 */

#ifndef ENGINECONTROLLER_H_
#define ENGINECONTROLLER_H_

#include <string>
#include <memory>
using namespace std;
#include <Ogre.h>
#include <fastdelegate/delegate.h>
#include <OIS/OIS.h>
#include <boost/thread/thread.hpp>
#include <boost/thread/xtime.hpp>

#include "EventDelegates.h"
#include "LifeCycleDelegates.h"
#include "GameStateInfo.h"

namespace ZGame
{
  class EngineView;
  class InputController;
  class GameState;
  class GameStateInfo;
  class LifeCyclePump;
  class KeyboardPump;
  class MousePump;
  class EngineController : Ogre::FrameListener
  {
  public:
    EngineController();
    virtual
    ~EngineController();

    bool frameStarted(const Ogre::FrameEvent &evt);

    void transitionState(const string key);
    bool onInit();
    void run();
    void onDestroy();

    void injectInputSubject();

    //input events
    bool onKeyUp(const OIS::KeyEvent &event);
    bool onKeyDown(const OIS::KeyEvent &evnt);
    bool onMouseMove(const OIS::MouseEvent &event);
    bool onMouseUp(const OIS::MouseEvent &event,const OIS::MouseButtonID id);
    bool onMouseDown(const OIS::MouseEvent &event,const OIS::MouseButtonID id);

    //input observers
    void addMouseObserver(ZGame::EVENT::MouseEvtObserver obs);
    //setters getters
    //Ogre::RenderWindow* getRenderWindow(){return _window.get();}
  protected:
    auto_ptr<Ogre::Root> _root;
    Ogre::SceneManager* _scnMgr;
    Ogre::RenderWindow* _window;
    auto_ptr<ZGame::EngineView> _engineView;

    auto_ptr<ZGame::InputController> _inController;

    GameStateInfoMap _gameSInfoMap;

    //Current state
    auto_ptr<GameStateInfo> _curStateInfo;
    auto_ptr<GameState> _curGameState;

    //LifeCycle pump
    auto_ptr<LifeCyclePump> _lfcPump;
    auto_ptr<KeyboardPump> _keyPump;
    auto_ptr<MousePump> _mousePump;


    void loadAssets();
    void loadStates();

    //state handling
    void loadCurrentState(const string curKey);
    void unloadCurrentState();
    void realizeCurrentState(); //"realize" as in use meta-data to load actual class data.

    Ogre::Camera* createDefaultCamera();

  private:
    string _listenerID;
    bool _stillRunning;

    void loadStartStateToCurrentState(const string curKey);

  };

}

#endif /* ENGINECONTROLLER_H_ */
