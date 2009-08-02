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
#include "LifeCycleDelegates.h"
#include "EventDelegates.h"
#include "ZEntity.h"
using namespace Ogre;
using namespace ZGame;

GameEditView::GameEditView() :
  _imposter(0), _imposterView(0), _dz(1.0),_ent(new ZEntity("ZombieEntity","RZR-002.mesh"))
{

}

GameEditView::~GameEditView()
{
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL,"In ~GameDitView");
  delete _imposter;
}

void
GameEditView::fillLfcObservers(LifeCycle::LifeCycleObserver &obs)
{
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL,"In GameEditView fillLfcObservers");
  obs.onInit.bind(&GameEditView::onInit,this);
  obs.onUpdate.bind(&GameEditView::onUpdate,this);
  obs.onDestroy.bind(&GameEditView::onDestroy,this);
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL,"Out GameEditView fillLfcObservers");
}

void
GameEditView::fillKeyObservers(EVENT::KeyboardEvtObserver &obs)
{
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL,"In GameEditView fillKeyObservers");
  obs.kde.bind(&GameEditView::onKeyDown,this);
  obs.kue.bind(&GameEditView::onKeyUp,this);
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL,"Out GameEditView fillKeyObservers");
}

/*
void
GameEditView::injectLifeCycleSubject(
    const ZGame::LifeCycle::LifeCycleSubject &subject)
{
  LifeCycle::LifeCycleObserver lfcObs;
  lfcObs.onInit.bind(&GameEditView::onInit, this);
  lfcObs.onUpdate.bind(&GameEditView::onUpdate, this);
  lfcObs.onDestroy.bind(&GameEditView::onDestroy, this);

  subject(lfcObs); //make fast delegate call
}

void
GameEditView::injectKeyEvtSubject(const ZGame::EVENT::KeyEvtSubject &subject)
{
  EVENT::KeyboardEvtObserver keyObs;
  keyObs.kde.bind(&GameEditView::onKeyDown, this);
  keyObs.kue.bind(&GameEditView::onKeyUp, this);
  subject(keyObs);
}

void
GameEditView::fillKeySubjectInjector(EVENT::KeyEvtSubjectInjector &injector)
{
  injector.bind(&GameEditView::injectKeyEvtSubject, this);
}

void
GameEditView::fillLifeCycleSubjectInjector(
    LifeCycle::LifeCycleSubjectInjector &injector)
{
  injector.bind(&GameEditView::injectLifeCycleSubject, this);
}
*/
bool
GameEditView::onUpdate(const Ogre::FrameEvent& evt)
{
  return true;
}

bool
GameEditView::onInit()
{
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL,
      "in GameEditView::onInit()");

  std::auto_ptr<ImposterGen> imposterGen(new ImposterGen());
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL,"Creating Imposter");
  _imposter = new Imposter(_ent->getMeshName());
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL,"Finished creating imposter");
  _imposter->init();
    if(_imposter == 0)
    Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL,"!!!!!!!!!!!!!imp is fucking null! after init");
  imposterGen->setInput(_imposter);
   if(_imposter == 0)
    Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL,"!!!!!!!!!!!!!imp is fucking null! after setinput");
  imposterGen->build();
  if(_imposter == 0)
    Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL,"imp is fucking null! after build");
  _imposterView.reset(new ImposterView());
  _imposterView->setInput(_imposter);
  imposterGen.reset(0); //we done with generator
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL,"Out of GameEditView onInit");
  return true;
}

bool
GameEditView::onDestroy()
{
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL,
      "In GameEditView::onDestroy");

  return true;
}

bool
GameEditView::onKeyUp(const OIS::KeyEvent &evt)
{
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL,
      "GameEditView::keyUpEvt");
  return true;
}

bool
GameEditView::onKeyDown(const OIS::KeyEvent &evt)
{
  using namespace Ogre;
  Camera* cam = EngineView::getSingleton().getCurrentCamera();

  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL,
      "GameEditView::keyDownEvt");
  if (evt.key == OIS::KC_W)
    {
      cam->moveRelative(Vector3(0.0f, 0.0f, -_dz));
    }
  else if (evt.key == OIS::KC_S)
    {
      cam->moveRelative(Vector3(0.0f, 0.0f, _dz));
    }
  else if (evt.key == OIS::KC_A)
    {
      _dz += 1.0;
    }
  else if (evt.key == OIS::KC_D)
    {
      _dz -= 1.0;
    }
  else if(evt.key == OIS::KC_F)
  {
    _imposterView->flip();
  }
  return true;
}
