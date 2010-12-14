/*
 * GameMainMenuState.cpp
 *
 *  Created on: Sep 5, 2008
 *      Author: bey0nd
 */

#include "GameMainMenuState.h"
#include "LifeCycleRegister.h"
#include "KeyEventRegister.h"
#include "DelegatesUtil.h"
using namespace ZGame;

GameMainMenuState::GameMainMenuState()
{

}

GameMainMenuState::~GameMainMenuState()
{

}


void GameMainMenuState::regLfcObsForInjection(LifeCycleRegister &lfcReg)
{
  //This
  LifeCycle::LifeCycleObserver lfcObs;
  LifeCycle::bindAndRegisterLifeCycleObserver<GameMainMenuState>(lfcReg, lfcObs,*this);
  lfcReg.registerLfcObs(lfcObs);
}

void GameMainMenuState::regKeyObsForInjection(KeyEventRegister &keyReg)
{
  //This
  EVENT::KeyboardEvtObserver keyObs;
  EVENT::bindKeyObserver(keyObs,*this);
  keyReg.registerKeyObs(keyObs);
  EVENT::clearKeyObs(keyObs);
}

bool GameMainMenuState::onUpdate(const Ogre::FrameEvent& evt)
{
  return true;
}

bool GameMainMenuState::onInit(ZGame::ZInitPacket packet)
{
  return true;
}

bool GameMainMenuState::onDestroy()
{
  return true;
}

bool GameMainMenuState::onKeyDown(const OIS::KeyEvent &evt)
{
  return true;
}

bool GameMainMenuState::onKeyUp(const OIS::KeyEvent &evt)
{
  return true;
}

