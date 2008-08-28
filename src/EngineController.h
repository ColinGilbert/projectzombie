/*
 * EngineController.h
 *
 *  Created on: Aug 24, 2008
 *      Author: bey0nd
 */

#ifndef ENGINECONTROLLER_H_
#define ENGINECONTROLLER_H_

#include <string>
using namespace std;
#include <Ogre.h>
#include <fastdelegate/delegate.h>
#include <OIS/OIS.h>
#include <boost/thread/thread.hpp>
#include <boost/thread/xtime.hpp>

#include "ZDelegates.h"
#include "InputController.h"
#include "GameStateInfo.h"

namespace ZGame
{
  class EngineView;
  class EngineController
  {
  public:
    EngineController();
    virtual
    ~EngineController();

    void transitionState(const string key);
    bool onInit();
    void run();
    void onDestroy();
    void addKeyboardListener(ZGame::EVENT::KeyboardEvtObserver keo);
    void addMouseListener(ZGame::EVENT::MouseEvtObserver meo);

    void injectInputSubject(InputController* inControl);

    //input events
    bool onKeyUp(const OIS::KeyEvent &event);
    bool onKeyDown(const OIS::KeyEvent &evnt);
    bool onMouseMove(const OIS::MouseEvent &event);
    bool onMouseUp(const OIS::MouseEvent &event,const OIS::MouseButtonID id);
    bool onMouseDown(const OIS::MouseEvent &event,const OIS::MouseButtonID id);

    //setters getters
    Ogre::RenderWindow* getRenderWindow(){return _window;}
  protected:
    Ogre::Root* _root;
    Ogre::SceneManager* _scnMgr;
    Ogre::RenderWindow* _window;
    ZGame::EngineView* _engineView;

    GameStateInfoMap _gameSInfoMap;

    //Current state
    GameStateInfo* _curStateInfo;
    //GameStat _curState;

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

  };

}

#endif /* ENGINECONTROLLER_H_ */
