/*
 * GameEditState.cpp
 *
 *  Created on: Aug 28, 2008
 *      Author: bey0nd
 */
#include <Ogre.h>
#include "GameEditState.h"
#include "GameEditView.h"

namespace ZGame
{
GameEditState::GameEditState() : GameState(),_editView(0)
{
  // TODO Auto-generated constructor stub

}

GameEditState::~GameEditState()
{
  // TODO Auto-generated destructor stub
}



void GameEditState::initialize()
{
  _editView = new GameEditView();
  Ogre::LogManager* lm = Ogre::LogManager::getSingletonPtr();
  lm->logMessage(Ogre::LML_NORMAL,"In GameEditState initialize");
  //add life cycle stuff here
  LifeCycle::LifeCycleSubjectInjector lfSubInjector;
  lfSubInjector.bind(&GameEditState::injectLifeCycleSubject,this);
  addLifeCycleSubjectInjector(lfSubInjector); //add the injector
  EVENT::KeyEvtSubjectInjector keySubInjector;
  keySubInjector.bind(&GameEditState::injectKeyEvtSubject,this);
  addKeySubjectInjector(keySubInjector); //add the injector
  //add gameview subject injector
  _editView->fillLifeCycleSubjectInjector(lfSubInjector);
  _editView->fillKeySubjectInjector(keySubInjector);
  addLifeCycleSubjectInjector(lfSubInjector);
  addKeySubjectInjector(keySubInjector);

}

void GameEditState::injectLifeCycleSubject(ZGame::LifeCycle::LifeCycleSubject &subject)
{
  LifeCycle::LifeCycleObserver lfcObs;
  lfcObs.onInit.bind(&GameEditState::onInit,this);
  lfcObs.onUpdate.bind(&GameEditState::onUpdate,this);
  lfcObs.onDestroy.bind(&GameEditState::onDestroy,this);
  subject(lfcObs);


}

void GameEditState::injectKeyEvtSubject(ZGame::EVENT::KeyEvtSubject &subject)
{
  EVENT::KeyboardEvtObserver keyObs;
  keyObs.kde.bind(&GameEditState::onKeyDown,this);
  keyObs.kue.bind(&GameEditState::onKeyUp,this);
  subject(keyObs); //make delegate call
}

bool GameEditState::onInit()
{
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL,"In GameEditState::onInit");
  return true;
}

bool GameEditState::onUpdate(const Ogre::FrameEvent& evt)
{
  //Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL,"In GameEditState::onUpdate");
  return true;
}

bool GameEditState::onDestroy()
{
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL,"In GameEditState::onDestroy");
  delete _editView;
  return true;
}

bool GameEditState::onKeyDown(const OIS::KeyEvent &evt)
{
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL,"In GameEditState::onKeyDown");
  return true;
}

bool GameEditState::onKeyUp(const OIS::KeyEvent &evt)
{
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL,"In GameEditState::onKeyUp");
  return true;
}

}

