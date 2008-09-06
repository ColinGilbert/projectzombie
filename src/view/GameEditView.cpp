/*
 * GameEditView.cpp
 *
 *  Created on: Sep 4, 2008
 *      Author: bey0nd
 */
#include <Ogre.h>
#include "GameEditView.h"


using namespace ZGame;

GameEditView::GameEditView()
{

}

GameEditView::~GameEditView()
{

}

void GameEditView::injectLifeCycleSubject(ZGame::LifeCycle::LifeCycleSubject &subject)
{
  LifeCycle::LifeCycleObserver lfcObs;
  lfcObs.onInit.bind(&GameEditView::onInit,this);
  lfcObs.onUpdate.bind(&GameEditView::onUpdate,this);
  lfcObs.onDestroy.bind(&GameEditView::onDestroy,this);

  subject(lfcObs); //make fast delegate call
}

void GameEditView::injectKeyEvtSubject(ZGame::EVENT::KeyEvtSubject &subject)
{
  EVENT::KeyboardEvtObserver keyObs;
  keyObs.kde.bind(&GameEditView::onKeyDown,this);
  keyObs.kue.bind(&GameEditView::onKeyUp,this);
  subject(keyObs);
}

void GameEditView::fillKeySubjectInjector(EVENT::KeyEvtSubjectInjector &injector)
{
  injector.bind(&GameEditView::injectKeyEvtSubject,this);
}

void GameEditView::fillLifeCycleSubjectInjector(LifeCycle::LifeCycleSubjectInjector &injector)
{
  injector.bind(&GameEditView::injectLifeCycleSubject,this);
}


bool GameEditView::onUpdate()
{
  return true;
}

bool GameEditView::onInit()
{
  return true;
}

bool GameEditView::onDestroy()
{
  return true;
}

bool GameEditView::onKeyUp(const OIS::KeyEvent &evt)
{
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL,"GameEditView::keyUpEvt");
  return true;
}

bool GameEditView::onKeyDown(const OIS::KeyEvent &evt)
{
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL,"GameEditView::keyDownEvt");
  return true;
}
