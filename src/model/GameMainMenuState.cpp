/*
 * GameMainMenuState.cpp
 *
 *  Created on: Sep 5, 2008
 *      Author: bey0nd
 */

#include "GameMainMenuState.h"

using namespace ZGame;

GameMainMenuState::GameMainMenuState()
{

}

GameMainMenuState::~GameMainMenuState()
{

}


void GameMainMenuState::regLfcObsForInjection()
{
  LifeCycle::LifeCycleObserver lfcObs;
  lfcObs.onInit.bind(&GameMainMenuState::onInit,this);
  lfcObs.onUpdate.bind(&GameMainMenuState::onUpdate,this);
  lfcObs.onDestroy.bind(&GameMainMenuState::onDestroy,this);
  registerLfcObs(lfcObs);
}

void GameMainMenuState::regKeyObsForInjection()
{
  EVENT::KeyboardEvtObserver keyObs;
  keyObs.kde.bind(&GameMainMenuState::onKeyDown,this);
  keyObs.kue.bind(&GameMainMenuState::onKeyUp,this);
  registerKeyObs(keyObs); //better register else exception is thrown
}

bool GameMainMenuState::onUpdate(const Ogre::FrameEvent& evt)
{
  return true;
}

bool GameMainMenuState::onInit()
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

