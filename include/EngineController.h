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

#include "EventDelegates.h"
#include "LifeCycleDelegates.h"
#include "InputController.h"
#include "GameStateInfo.h"
#include "GameState.h"

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

    //left cycles
    void addLifeCycleObserver(ZGame::LifeCycle::LifeCycleObserver obs);

    //setters getters
    Ogre::RenderWindow* getRenderWindow(){return _window;}
  protected:
    Ogre::Root* _root;
    Ogre::SceneManager* _scnMgr;
    Ogre::RenderWindow* _window;
    ZGame::EngineView* _engineView;

    ZGame::InputController* _inController;

    GameStateInfoMap _gameSInfoMap;

    //Current state
    GameStateInfo* _curStateInfo;
    GameState* _curGameState;
    //GameStat _curState;

    //life cycle
    typedef vector<ZGame::LifeCycle::LifeCycleEvent>::iterator LifeCycleObsItr;
    vector<ZGame::LifeCycle::LifeCycleEvent> _onInitObs; //on init observers
    vector<ZGame::LifeCycle::LifeCycleEvent> _onUpdateObs; //on update observers
    vector<ZGame::LifeCycle::LifeCycleEvent> _onDestroyObs; //on destroy observers

    void loadAssets();
    void loadStates();

    //state handling
    void loadCurrentState(const string curKey);
    void unloadCurrentState();
    void realizeCurrentState(); //"realize" as in use meta-data to load actual class data.

    //life cycle updates
    void updateOnItObs();
    void updateOnUpdateObs();
    void updateOnDestroyObs();
    void removeAllLifeCycleObs();

    Ogre::Camera* createDefaultCamera();

  private:
    string _listenerID;
    bool _stillRunning;

    void loadStartStateToCurrentState(const string curKey);

  };

}

#endif /* ENGINECONTROLLER_H_ */
