/*
 * GameMainMenuState.h
 *
 *  Created on: Sep 5, 2008
 *      Author: bey0nd
 */

#ifndef GAMEMAINMENUSTATE_H_
#define GAMEMAINMENUSTATE_H_

#include "GameState.h"

namespace ZGame
{
  class GameMainMenuState : public GameState
  {
  public:
    GameMainMenuState();
    virtual ~GameMainMenuState();

    virtual void initialize();
    void injectLifeCycleSubject(ZGame::LifeCycle::LifeCycleSubject &subject);
    void injectKeyEvtSubject(ZGame::EVENT::KeyEvtSubject &subject);

    //life cycle methods
    bool onUpdate();
    bool onInit();
    bool onDestroy();

    //control methods
    bool onKeyUp(const OIS::KeyEvent &evt);
    bool onKeyDown(const OIS::KeyEvent &evt);


  };
}

#endif /* GAMEMAINMENUSTATE_H_ */
