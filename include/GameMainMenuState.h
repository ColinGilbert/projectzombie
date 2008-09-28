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

    //life cycle methods
    bool onUpdate(const Ogre::FrameEvent& evt);
    bool onInit();
    bool onDestroy();

    //control methods
    bool onKeyUp(const OIS::KeyEvent &evt);
    bool onKeyDown(const OIS::KeyEvent &evt);

  protected:
    virtual void regLfcObsForInjection();
    virtual void regKeyObsForInjection();

  };
}

#endif /* GAMEMAINMENUSTATE_H_ */
