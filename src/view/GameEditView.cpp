/*
 * GameEditView.cpp
 *
 *  Created on: Sep 4, 2008
 *      Author: bey0nd
 */
#include <Ogre.h>
#include "GameEditView.h"
#include "EngineView.h"
#include "ImposterGen.h"
#include "Imposter.h"
#include "ImposterView.h"

using namespace ZGame;

GameEditView::GameEditView() : _imposterGen(0),_imposter(0),_dz(1.0)
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


bool GameEditView::onUpdate(const Ogre::FrameEvent& evt)
{
  return true;
}

bool GameEditView::onInit()
{
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL,"in GameEditView::onInit()");

  _imposter = new Imposter("robot.mesh");
  _imposterGen = new ImposterGen();
  _imposterGen->setInput(_imposter);
  _imposterGen->build();
  delete _imposterGen; //we done with generator
  _imposterGen = 0;
  _imposterView = new ImposterView();
  _imposterView->setInput(_imposter);
  return true;
}

bool GameEditView::onDestroy()
{
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL,"In GameEditView::onDestroy");
  if(_imposterGen)
    delete _imposterGen;
  if(_imposterView)
    delete _imposterView;
  if(_imposter)
    delete _imposter;
  return true;
}

bool GameEditView::onKeyUp(const OIS::KeyEvent &evt)
{
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL,"GameEditView::keyUpEvt");
  return true;
}

bool GameEditView::onKeyDown(const OIS::KeyEvent &evt)
{
  using namespace Ogre;
  Camera* cam = EngineView::getSingleton().getCurrentCamera();

  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL,"GameEditView::keyDownEvt");
  if(evt.key == OIS::KC_W)
    {
      cam->moveRelative(Vector3(0.0f,0.0f,-_dz));
    }
  else if(evt.key == OIS::KC_S)
    {
      cam->moveRelative(Vector3(0.0f,0.0f,_dz));
    }
  else if(evt.key == OIS::KC_A)
    {
      _dz += 1.0;
    }
  else if(evt.key == OIS::KC_D)
    {
      _dz -= 1.0;
    }
  return true;
}
