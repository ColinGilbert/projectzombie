/*
 * GameState.cpp
 *
 *  Created on: Aug 28, 2008
 *      Author: bey0nd
 */

#include <Ogre.h>
#include "GameState.h"
#include "LifeCycleRegister.h"
#include "KeyEventRegister.h"
#include "MouseEventRegister.h"
namespace ZGame
{

  GameState::GameState()
  {
    // TODO Auto-generated constructor stub
  }

  GameState::~GameState()
  {
    // TODO Auto-generated destructor stub
  }

  void GameState::init(LifeCycleRegister &lfcReg, KeyEventRegister &keyReg, MouseEventRegister &mouseReg)
  {
    regLfcObsForInjection(lfcReg); //register life cycle observers for injection.
    regKeyObsForInjection(keyReg); //register key observers for injection.
    regMouseObsForInjection(mouseReg);
  }




}
