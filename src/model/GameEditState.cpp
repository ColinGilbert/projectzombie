/*
 * GameEditState.cpp
 *
 *  Created on: Aug 28, 2008
 *      Author: bey0nd
 */
#include <Ogre.h>
#include "GameEditState.h"
#include "GameEditView.h"
#include "LifeCycleRegister.h"
#include "KeyEventRegister.h"
namespace ZGame
{
GameEditState::GameEditState() : GameState(), _editView(new GameEditView())
{
  // TODO Auto-generated constructor stub
}

GameEditState::~GameEditState()
{
  // TODO Auto-generated destructor stub
}


void GameEditState::regLfcObsForInjection(LifeCycleRegister &lfcReg)
{
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL,"In GameEditState regLfcObsForInjection");
  LifeCycle::LifeCycleObserver lfcObs;
  lfcObs.onInit.bind(&GameEditState::onInit, this);
  lfcObs.onUpdate.bind(&GameEditState::onUpdate, this);
  lfcObs.onDestroy.bind(&GameEditState::onDestroy, this);
  lfcReg.registerLfcObs(lfcObs); //register to LifeCycleRegister
  addLfcObsInjector(_editView);
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL,"Out GameEditState regLfcObsForInjection");
}

void GameEditState::regKeyObsForInjection(KeyEventRegister &keyReg)
{
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL,"In GameEditState regKeyObsForInjection");
  EVENT::KeyboardEvtObserver keyObs;
  keyObs.kde.bind(&GameEditState::onKeyDown,this);
  keyObs.kue.bind(&GameEditState::onKeyUp,this);
  keyReg.registerKeyObs(keyObs); //better register else exception
  addKeyObsInjector(_editView);
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL,"In GameEditState regKeyObsForInjection");
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

