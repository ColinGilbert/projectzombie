/*
 * GameState.cpp
 *
 *  Created on: Aug 28, 2008
 *      Author: bey0nd
 */

#include <Ogre.h>
#include <SdkTrays.h>
#include "GameState.h"
#include "LifeCycleRegister.h"
#include "KeyEventRegister.h"
#include "MouseEventRegister.h"
#include "LifeCycleDelegates.h"
#include "DelegatesUtil.h"
#include "GraphicsController.h"

namespace ZGame
{

  GameState::GameState() : _gfxCtrl(new GraphicsController()),
      _tray(0)
  {
    // TODO Auto-generated constructor stub
  }

  GameState::~GameState()
  {
    // TODO Auto-generated destructor stub
  }

  void
  GameState::init(LifeCycleRegister &lfcReg, KeyEventRegister &keyReg,
      MouseEventRegister &mouseReg, OgreBites::SdkTrayManager* tray)
  {
    regLfcObsForInjection(lfcReg); //register life cycle observers for injection.
    regKeyObsForInjection(keyReg); //register key observers for injection.
    regMouseObsForInjection(mouseReg);
    _tray = tray;
  }
}
