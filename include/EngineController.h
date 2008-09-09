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
#include "LifeCyclePump.h"
#include "KeyboardPump.h"
#include "InputController.h"
#include "GameStateInfo.h"
#include "GameState.h"

#include "InputController.h"

namespace ZGame
{
  class EngineView;
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

    void injectInputSubject(InputController* inControl);

    //input events
    bool onKeyUp(const OIS::KeyEvent &event);
    bool onKeyDown(const OIS::KeyEvent &evnt);
    bool onMouseMove(const OIS::MouseEvent &event);
    bool onMouseUp(const OIS::MouseEvent &event,const OIS::MouseButtonID id);
    bool onMouseDown(const OIS::MouseEvent &event,const OIS::MouseButtonID id);

    //input observers
    void addMouseObserver(ZGame::EVENT::MouseEvtObserver obs);
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

    //LifeCycle pump
    LifeCyclePump _lfcPump;
    KeyboardPump _keyPump;

    //input
    typedef vector<ZGame::EVENT::ZMouseDownEvt>::iterator MouseDownObsItr;
    typedef vector<ZGame::EVENT::ZMouseUpEvt>::iterator MouseUpObsItr;
    typedef vector<ZGame::EVENT::ZMouseMoveEvt>::iterator MouseMoveObsItr;
    vector<ZGame::EVENT::ZMouseDownEvt> _onMouseDownObs;
    vector<ZGame::EVENT::ZMouseUpEvt> _onMouseUpObs;
    vector<ZGame::EVENT::ZMouseMoveEvt> _onMouseMoveObs;


    void loadAssets();
    void loadStates();

    //state handling
    void loadCurrentState(const string curKey);
    void unloadCurrentState();
    void realizeCurrentState(); //"realize" as in use meta-data to load actual class data.

    //input updates

    void updateMouseUpObs();
    void updateMouseDownObs();
    void updateMouseMoveObs();


    Ogre::Camera* createDefaultCamera();

  private:
    string _listenerID;
    bool _stillRunning;

    void loadStartStateToCurrentState(const string curKey);

  };

}

#endif /* ENGINECONTROLLER_H_ */
