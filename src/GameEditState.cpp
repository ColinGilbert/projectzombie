/*
 * GameEditState.cpp
 *
 *  Created on: Aug 28, 2008
 *      Author: bey0nd
 */
#include <Ogre.h>
#include "GameEditState.h"

namespace ZGame
{
GameEditState::GameEditState() : GameState()
{
  // TODO Auto-generated constructor stub

}

GameEditState::~GameEditState()
{
  // TODO Auto-generated destructor stub
}



void GameEditState::initialize()
{
  Ogre::LogManager* lm = Ogre::LogManager::getSingletonPtr();
  lm->logMessage(Ogre::LML_NORMAL,"In GameEditState initialize");
  //add life cycle stuff here
  LifeCycle::LifeCycleSubjectInjector lfSubInjector;
  lfSubInjector.bind(&GameEditState::injectLifeCycleSubject,this);
  addLifeCycleSubjectInjector(lfSubInjector);
}

void GameEditState::injectLifeCycleSubject(ZGame::LifeCycle::LifeCycleSubject &subject)
{
  LifeCycle::LifeCycleObserver lfcObs;
  lfcObs.onInit.bind(&GameEditState::onInit,this);
  lfcObs.onUpdate.bind(&GameEditState::onUpdate,this);
  lfcObs.onDestroy.bind(&GameEditState::onDestroy,this);

  subject(lfcObs);
}

bool GameEditState::onInit()
{
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL,"In GameEditState::onInit");
  return true;
}

bool GameEditState::onUpdate()
{
  //Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL,"In GameEditState::onUpdate");
  return true;
}

bool GameEditState::onDestroy()
{
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL,"In GameEditState::onDestroy");
  return true;
}

}

