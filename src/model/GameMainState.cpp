/*
 * GameMainState.cpp
 *
 *  Created on: Sep 18, 2008
 *      Author: bey0nd
 */

#include "GameMainState.h"

using namespace ZGame;

GameMainState::GameMainState() : GameState()
{

}

GameMainState::~GameMainState()
{

}

void GameMainState::initialize()
{
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL,"In GameMainState::initialize()");
  //add life cycle stuff here
  LifeCycle::LifeCycleSubjectInjector lfSubInjector;
  lfSubInjector.bind(&GameMainState::injectLifeCycleSubject,this);
  addLifeCycleSubjectInjector(lfSubInjector);
  EVENT::KeyEvtSubjectInjector keySubInjector;
  keySubInjector.bind(&GameMainState::injectKeyEvtSubject,this);
  addKeySubjectInjector(keySubInjector);
}

void GameMainState::injectLifeCycleSubject(ZGame::LifeCycle::LifeCycleSubject &subject)
{
  LifeCycle::LifeCycleObserver lfcObs;
  lfcObs.onInit.bind(&GameMainState::onInit,this);
  lfcObs.onUpdate.bind(&GameMainState::onUpdate,this);
  lfcObs.onDestroy.bind(&GameMainState::onDestroy,this);
  subject(lfcObs);
}

void GameMainState::injectKeyEvtSubject(ZGame::EVENT::KeyEvtSubject &subject)
{
  EVENT::KeyboardEvtObserver keyObs;
  keyObs.kde.bind(&GameMainState::onKeyDown,this);
  keyObs.kue.bind(&GameMainState::onKeyUp,this);
  subject(keyObs);
}

bool GameMainState::onInit()
{
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL,"In GameMainState onInit");
  return true;
}

bool GameMainState::onUpdate(const Ogre::FrameEvent &evt)
{
 return true;
}

bool GameMainState::onDestroy()
{
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL,"In GameMainState::noDestroy()");
  return true;
}

bool GameMainState::onKeyDown(const OIS::KeyEvent &evt)
{
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL,"In GameMainState::onKeyDown");
  return true;
}

bool GameMainState::onKeyUp(const OIS::KeyEvent &evt)
{
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL,"In GameMainState::onKeyUp");
  return true;
}



