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

void GameMainMenuState::initialize()
{

}

void GameMainMenuState::injectLifeCycleSubject(ZGame::LifeCycle::LifeCycleSubject &subject)
{
  LifeCycle::LifeCycleObserver lfcObs;
  lfcObs.onInit.bind(&GameMainMenuState::onInit,this);
  lfcObs.onUpdate.bind(&GameMainMenuState::onUpdate,this);
  lfcObs.onDestroy.bind(&GameMainMenuState::onDestroy,this);
  subject(lfcObs);
}

void GameMainMenuState::injectKeyEvtSubject(ZGame::EVENT::KeyEvtSubject &subject)
{
  EVENT::KeyboardEvtObserver keyObs;
  keyObs.kde.bind(&GameMainMenuState::onKeyDown,this);
  keyObs.kue.bind(&GameMainMenuState::onKeyUp,this);
  subject(keyObs);
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

