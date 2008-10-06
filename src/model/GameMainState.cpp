/*
 * GameMainState.cpp
 *
 *  Created on: Sep 18, 2008
 *      Author: bey0nd
 */

#include <cmath>
using namespace std;
#include "GameMainState.h"
#include "GPUEntities.h"
#include "GPUEntsGenProps.h"
#include "GPUEntsView.h"
#include "GPUEntsGen.h"
#include "ZEntity.h"
#include "EngineView.h"
#include "DelegatesUtil.h"
#include "LifeCycleRegister.h"
#include "KeyEventRegister.h"
#include "MouseEventRegister.h"
#include "ControlModuleProto.h"
#include "WhiteNoiseView.h"
using namespace Ogre;
using namespace ZGame;

GameMainState::GameMainState() :
  GameState(), _gpuEntsView(new GPUEntsView()), _cam(0), _dz(1.5f), _forward(
      false), _backward(false), _trans(1.0), _controlMod(
      new ControlModuleProto()), _whtNoiseView(new WhiteNoiseView())
{

}

GameMainState::~GameMainState()
{
}

void
GameMainState::regLfcObsForInjection(LifeCycleRegister &lfcReg)
{

  LifeCycle::LifeCycleObserver lfcObs;
  lfcObs.onInit.bind(&GameMainState::onInit, this);
  lfcObs.onUpdate.bind(&GameMainState::onUpdate, this);
  lfcObs.onDestroy.bind(&GameMainState::onDestroy, this);
  //don't forget to call helper method to register, else exception is thrown.
  lfcReg.registerLfcObs(lfcObs);
  //register objects that belongs in this state
  LifeCycle::clearLfcObs(lfcObs);
  _gpuEntsView->fillLfcObservers(lfcObs);
  lfcReg.registerLfcObs(lfcObs);
  //register control module
  LifeCycle::clearLfcObs(lfcObs);
  _controlMod->fillLfcObs(lfcObs);
  lfcReg.registerLfcObs(lfcObs);
  LifeCycle::clearLfcObs(lfcObs);
  _whtNoiseView->fillLfcObservers(lfcObs);
  lfcReg.registerLfcObs(lfcObs);
}

void
GameMainState::regKeyObsForInjection(KeyEventRegister &keyReg)
{

  EVENT::KeyboardEvtObserver keyObs;
  /*
   keyObs.kde.bind(&GameMainState::onKeyDown,this);
   keyObs.kue.bind(&GameMainState::onKeyUp,this);
   keyReg.registerKeyObs(keyObs);//make sure we register else exception!
   */
  _controlMod->fillKeyObs(keyObs);
  keyReg.registerKeyObs(keyObs);

}

void
GameMainState::regMouseObsForInjection(MouseEventRegister &mouseReg)
{
  EVENT::MouseEvtObserver mouseObs;
  _controlMod->fillMouseObs(mouseObs);
  mouseReg.registerMouseObs(mouseObs);
}

bool
GameMainState::onInit()
{
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL,
      "In GameMainState onInit");
  createGPUEntities();
  _cam = EngineView::getSingleton().getCurrentCamera();
  Plane plane(Vector3::UNIT_Y, 0);
  //ground testing plane
  MeshManager::getSingleton().createPlane("TempGroundPlane",
      ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, 5000, 5000, 1,
      1, true, 1, 1, 1, Vector3::UNIT_Z);
  SceneManager* scnMgr = EngineView::getSingleton().getSceneManager();
  string planeName = "TempGroundPlaneEntity";
  Entity* texEnt = scnMgr->createEntity(planeName, "TempGroundPlane");
  texEnt->setMaterialName("Examples/OgreLogo");
  string name;
  name = "TempGroundPlaneNode";
  SceneNode* texNode = scnMgr->getRootSceneNode()->createChildSceneNode(name,
      Vector3(0.0f, -1.0f, 0.0f));
  texNode->attachObject(texEnt);
  texNode->setVisible(true, true);
  texNode->yaw(Radian(Math::DegreesToRadians(90.0f)));
  //white noise
  _whtNoiseView->init();
  return true;
}

bool
GameMainState::onUpdate(const Ogre::FrameEvent &evt)
{
  return true;
}

bool
GameMainState::onDestroy()
{
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL,
      "In GameMainState::noDestroy()");
  if (_gpuEntsView)
    delete _gpuEntsView;
  if (_whtNoiseView)
    delete _whtNoiseView;
  return true;
}

bool
GameMainState::onKeyDown(const OIS::KeyEvent &evt)
{
  return true;
}

bool
GameMainState::onKeyUp(const OIS::KeyEvent &evt)
{
  return true;
}

void
GameMainState::createGPUEntities()
{
  Ogre::LogManager::getSingleton().logMessage(LML_NORMAL,
      "GameMainState::createGPUEntities");
  ZEntity zent("ZombieEntity", "robot.mesh");
  int texW = 512;
  int texH = 512;
  Real minX, maxX, minZ, maxZ; //the space into which we want to distribute the GPU entities
  minX = -800.0f;
  maxX = 800.0f;
  minZ = -1500.0;
  maxZ = 1500.0f;
  Real entHeight = 2.0f; //1.6 meters
  auto_ptr<GPUEntsGenProps> props(new GPUEntsGenProps(texW, texH, entHeight));
  props->setExtents(minX, minZ, maxX, maxZ);
  GPUEntsGen entsGen(&zent, props);
  entsGen.build();
  _gpuEnts = entsGen.getOutput();
  //_gpuEntsView = new GPUEntsView();
  _gpuEntsView->attachGPUEnts(_gpuEnts.get());
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL,
      "GameMainState::createGPUEntities done");
}

