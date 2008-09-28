/*
 * GameState.cpp
 *
 *  Created on: Aug 28, 2008
 *      Author: bey0nd
 */

#include <Ogre.h>
#include "GameState.h"

namespace ZGame
{

  GameState::GameState()
  {
    // TODO Auto-generated constructor stub
    _lfcRegister = new LifeCycleRegister();
    _keyRegister = new KeyEventRegister();
  }

  GameState::~GameState()
  {
    // TODO Auto-generated destructor stub
  }

  void GameState::init()
  {
    regLfcObsForInjection(); //register life cycle observers for injection.
    regKeyObsForInjection(); //register key observers for injection.
    //done with registers

  }


}
