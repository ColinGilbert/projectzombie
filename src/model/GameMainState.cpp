/*
 * GameMainState.cpp
 *
 *  Created on: Sep 18, 2008
 *      Author: bey0nd
 */

#include "GameMainState.h"
#include "GPUEntities.h"
#include "GPUEntsGenProps.h"
#include "GPUEntsView.h"
#include "GPUEntsGen.h"
#include "ZEntity.h"
using namespace Ogre;
using namespace ZGame;

GameMainState::GameMainState() : GameState(), _gpuEntsView(0)
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
  createGPUEntities();
  return true;
}

bool GameMainState::onUpdate(const Ogre::FrameEvent &evt)
{
 return true;
}

bool GameMainState::onDestroy()
{
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL,"In GameMainState::noDestroy()");
  if(_gpuEntsView)
    delete _gpuEntsView;
  return true;
}

bool GameMainState::onKeyDown(const OIS::KeyEvent &evt)
{
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL,"In GameMainState::onKeyDown");
  if(evt.key == OIS::KC_A)
    _gpuEntsView->alphaBlend();
  return true;
}

bool GameMainState::onKeyUp(const OIS::KeyEvent &evt)
{
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL,"In GameMainState::onKeyUp");
  return true;
}

void GameMainState::createGPUEntities()
{
  Ogre::LogManager::getSingleton().logMessage(LML_NORMAL,"GameMainState::createGPUEntities");
  ZEntity zent("ZombieEntity","robot.mesh");
  int texW = 512;
  int texH = 512;
  Real minX,maxX,minZ,maxZ; //the space into which we want to distribute the GPU entities
  minX = -10.0f; maxX = 10.0f; minZ = -30.0f; maxZ = 0.0f;
  Real entHeight = 2.0f; //1.6 meters
  auto_ptr<GPUEntsGenProps> props(new GPUEntsGenProps(texW,texH,entHeight));
  props->setExtents(minX,minZ,maxX,maxZ);
  GPUEntsGen entsGen(&zent,props);
  entsGen.build();
  _gpuEnts = entsGen.getOutput();
  _gpuEntsView = new GPUEntsView();
  _gpuEntsView->attachGPUEnts(_gpuEnts.get());
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL,"GameMainState::createGPUEntities done");
}

