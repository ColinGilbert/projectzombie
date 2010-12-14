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
#include "entities/ZEntity.h"
using namespace Ogre;
using namespace ZGame;

GameEditView::GameEditView() :
_imposter(0), _imposterView(0), _dz(1.0),_ent(new Entities::ZEntity("ZombieEntity","RZR-002.mesh"))
{

}

GameEditView::~GameEditView()
{
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL,"In ~GameDitView");
  delete _imposter;
}
bool
GameEditView::onUpdate(const Ogre::FrameEvent& evt)
{
  return true;
}

bool
    GameEditView::onInit(ZGame::ZInitPacket initPacket)
{
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL,
      "in GameEditView::onInit()");

  std::auto_ptr<ImposterGen> imposterGen(new ImposterGen());
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL,"Creating Imposter");
  _imposter = new Imposter(_ent->getResource());
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL,"Finished creating imposter");
  _imposter->init();
  imposterGen->setInput(_imposter);
  imposterGen->build();
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
