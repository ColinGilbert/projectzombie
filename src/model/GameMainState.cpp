/*
 * GameMainState.cpp
 *
 *  Created on: Sep 18, 2008
 *      Author: bey0nd
 */

#include <cmath>
using namespace std;
#include <boost/shared_ptr.hpp>
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
#include "GPUEntsControl.h"
using namespace Ogre;
using namespace ZGame;

GameMainState::GameMainState() :
  GameState(), _gpuEntsView(new GPUEntsView()), _cam(0), _dz(1.5f), _forward(
      false), _backward(false), _trans(1.0), _controlMod(
      new ControlModuleProto()), _whtNoiseView(new WhiteNoiseView()),
      _gpuEntsControl(new GPUEntsControl())
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

  lfcReg.registerLfcObs(lfcObs);
  //register objects that belongs in this state
  addLfcObsInjector(_gpuEntsView);
  //register control module
  addLfcObsInjector(_controlMod);
  //register white noise
  //addLfcObsInjector(_whtNoiseView);
  //register GPUEntsControl
  addLfcObsInjector(_gpuEntsControl);

}

void
GameMainState::regKeyObsForInjection(KeyEventRegister &keyReg)
{
  addKeyObsInjector(_controlMod);
}

void
GameMainState::regMouseObsForInjection(MouseEventRegister &mouseReg)
{
  addMouseObsInjector(_controlMod);
}

bool
GameMainState::onInit()
{
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL,
      "In GameMainState onInit");
  createGPUEntities();
  createWorld();
  cout << "Create world done!" << endl;
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
  LogManager* lm = LogManager::getSingletonPtr();
  lm->logMessage(LML_NORMAL, "GameMainState::createGPUEntities");
  //note: we are using shared_ptr here is because later we will have an entity resource manager.
  boost::shared_ptr<ZEntity> zent(new ZEntity("ZombieEntity", "robot.mesh"));
  int texW = 256;
  int texH = 256;
  Real minX, maxX, minZ, maxZ; //the space into which we want to distribute the GPU entities
  minX = -100.0f;
  maxX = 100.0f;
  minZ = -250.0;
  maxZ = 250.0f;
  Real entHeight = 2.0f; //1.6 meters
  auto_ptr<GPUEntsGenProps> props(new GPUEntsGenProps(texW, texH, entHeight));
  props->setExtents(minX, minZ, maxX, maxZ);
  GPUEntsGen entsGen(zent, props);
  entsGen.build();
  _gpuEnts = entsGen.getOutput();
  lm->logMessage(LML_TRIVIAL, "About to attach GPU ents");
  _gpuEntsView->attachGPUEnts(_gpuEnts.get());
  lm->logMessage(Ogre::LML_TRIVIAL, "GameMainState::createGPUEntities done");
  cout << "Calling gpuEntsControl attach GPUEnts" << endl;
  _gpuEntsControl->attachGPUEnts(_gpuEnts.get());
  cout << "Done calling" << endl;
  lm->logMessage(LML_TRIVIAL, "Out Gamestate createGPUEntities");
}

void
GameMainState::createWorld()
{
  cout << "Create world!" << endl;
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
}

