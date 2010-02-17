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
#include "EventDelegates.h"
#include "LifeCycleRegister.h"
#include "KeyEventRegister.h"
#include "MouseEventRegister.h"
#include "ControlModuleProto.h"
#include "WhiteNoiseView.h"
#include "GPUEntsControl.h"
#include "world/WorldController.h"
#include "CommandController.h"
#include "utilities/CharacterUtil.h"

using namespace Ogre;
using namespace ZGame;


GameMainState::GameMainState() :
GameState(), _gpuEntsView(new GPUEntsView()), _cam(0), _dz(1.5f), _forward(
  false), _backward(false), _trans(1.0), _controlMod(
  new ControlModuleProto()), _whtNoiseView(new WhiteNoiseView()),
  _gpuEntsControl(new GPUEntsControl()),
  _worldController(new World::WorldController()),
  _charUtil(new Util::CharacterUtil())
{

}

GameMainState::~GameMainState()
{
}

/**
*This class will register LifeCycle observers (to be later injected into LifeCycle subjects: The Subject Observer pattern.)
*
*
**/
void
GameMainState::regLfcObsForInjection(LifeCycleRegister &lfcReg)
{ 
  //this
  LifeCycle::LifeCycleObserver lfcObs;
  LifeCycle::bindLifeCycleObserver(lfcObs,*this);
  lfcReg.registerLfcObs(lfcObs);
  LifeCycle::clearLfcObs(lfcObs);

  //world controller
  LifeCycle::bindLifeCycleObserver(lfcObs,*_worldController);
  lfcReg.registerLfcObs(lfcObs);
  LifeCycle::clearLfcObs(lfcObs);

  //control module
  LifeCycle::bindLifeCycleObserver(lfcObs,*_controlMod);
  lfcReg.registerLfcObs(lfcObs);
  LifeCycle::clearLfcObs(lfcObs);


  //register objects that belongs in this state
  //addLfcObsInjector(_gpuEntsView);
  //register control module
  //addLfcObsInjector(_controlMod);
  //register white noise
  //addLfcObsInjector(_whtNoiseView);
  //register GPUEntsControl
  //addLfcObsInjector(_gpuEntsControl);

}

/**
*This class will register keyboard observers for later injection into keyboard subjects.
*
**/
void
GameMainState::regKeyObsForInjection(KeyEventRegister &keyReg)
{
  //ControlModule
  EVENT::KeyboardEvtObserver keyObs;
  EVENT::bindKeyObserver(keyObs,*_controlMod);
  keyReg.registerKeyObs(keyObs);
  EVENT::clearKeyObs(keyObs);
}

/**
*This class will register mouse observers for later injection into mouse subjects.
**/
void
GameMainState::regMouseObsForInjection(MouseEventRegister &mouseReg)
{
  //Control Module
  EVENT::MouseEvtObserver mouseObs;
  EVENT::bindMouseObserver(mouseObs,*_controlMod);
  mouseReg.registerMouseObs(mouseObs);
  EVENT::clearMouseObs(mouseObs);
}

bool
GameMainState::onInit()
{
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL,
    "In GameMainState onInit");
  //createGPUEntities();
  Ogre::LogManager::getSingleton().logMessage("Done creating GPU entities");
  createWorld();
  _worldController->init();
  Ogre::LogManager::getSingleton().logMessage("Done creating world");
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
  boost::shared_ptr<ZEntity> zent(new ZEntity("ZombieEntity", //"robot.mesh"));
    "RZR-002.mesh"));
  //"athene.mesh"));
  int texW = 64;
  int texH = 64;
  Real minX, maxX, minZ, maxZ; //the space into which we want to distribute the GPU entities
  minX = -2000.0f;
  maxX = 2000.0f;
  minZ = -500.0;
  maxZ = 500.0f;
  Real entHeight = 2.0f; //1.6 meters
  auto_ptr<GPUEntsGenProps> props(new GPUEntsGenProps(texW, texH, entHeight));
  props->setExtents(minX, minZ, maxX, maxZ);
  GPUEntsGen entsGen(zent, props);
  entsGen.build();
  _gpuEnts = entsGen.getOutput();
  lm->logMessage(LML_TRIVIAL, "About to attach GPU ents");
  _gpuEntsView->attachGPUEnts(_gpuEnts.get());
  lm->logMessage(Ogre::LML_TRIVIAL, "GameMainState::createGPUEntities done");
  _gpuEntsControl->attachGPUEnts(_gpuEnts.get());
  lm->logMessage(LML_TRIVIAL, "Out Gamestate createGPUEntities");
}

void
GameMainState::createCharacters()
{
}

void
GameMainState::createWorld()
{
  Ogre::LogManager *lm = Ogre::LogManager::getSingletonPtr();
  lm->logMessage(Ogre::LML_TRIVIAL,"In GameMainState::createWorld");
  SceneManager *scnMgr = EngineView::getSingleton().getSceneManager();
  //scnMgr->setWorldGeometry(Ogre::String("paginglandscape2.cfg"));
  //Set ambient light
  scnMgr->setAmbientLight(ColourValue(0.7f,0.7f,0.7f));

  Vector3 LightPos(0.0f, 10.0f, 20.0f);
  Ogre::Light *mLight;
  Ogre::SceneNode *mLightNode;

  mLight = scnMgr->createLight("MainLight");

  mLight->setPosition(LightPos);

  mLight->setDiffuseColour(0.93f, 0.93f, 0.93f);
  mLight->setAttenuation(10000.0f, 1.0f, 1.0f, 0.0f);

  mLight->setType (Light::LT_DIRECTIONAL);
  mLight->setDirection (Vector3( -1, -1, 0 ));

  SceneNode *BaseNode = scnMgr->getRootSceneNode()->createChildSceneNode ("LightHandler");
  BaseNode->setPosition (-3500.0f, 0.0f, -2600.0f);
  mLightNode = BaseNode->createChildSceneNode ("LightHandlerSon");
  mLightNode->setPosition (0.0f, 15000.0f, 0.0f);
  mLightNode->attachObject (mLight);

  BillboardSet* LightSet = scnMgr->createBillboardSet("RedYellowLights");
  LightSet->setMaterialName("Examples/Flare");
  mLightNode->attachObject(LightSet);

  Billboard* LightBoard = LightSet->createBillboard(LightPos);
  //LightBoard->setDimensions(10.0f, 10.0f);
  LightBoard->setColour(ColourValue(1.0f, 1.0f, 0.2f, 1.0f));

  scnMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
  scnMgr->setShadowColour(Ogre::ColourValue(0.5,0.5,0.5));
  scnMgr->setShadowFarDistance(60);
  scnMgr->setShadowTextureSettings(512,2);

  Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);
  Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(1);





  //createCharacters();


  
  //_cam = EngineView::getSingleton().getCurrentCamera();
  
  Plane plane(Vector3::UNIT_Y, 0);
  //ground testing plane
  MeshManager::getSingleton().createPlane("TempGroundPlane",
  ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, 5000, 5000, 1,
  1, true, 1, 1, 1, Vector3::UNIT_Z);
  //SceneManager* scnMgr = EngineView::getSingleton().getSceneManager();
  string planeName = "TempGroundPlaneEntity";
  Entity* texEnt = scnMgr->createEntity(planeName, "TempGroundPlane");
  texEnt->setMaterialName("Examples/OgreLogo");
  lm->logMessage("texEnt material set",Ogre::LML_TRIVIAL);
  string name;
  name = "TempGroundPlaneNode";
  SceneNode* texNode = scnMgr->getRootSceneNode()->createChildSceneNode(name,
  Vector3(0.0f, 0.0f, 0.0f));
  texNode->attachObject(texEnt);

  //texNode->setVisible(true, true);
  texNode->setPosition(Vector3(0.0f,-5.2f,0.0f));
  texNode->yaw(Radian(Math::DegreesToRadians(90.0f)));
  
  

  lm->logMessage(Ogre::LML_TRIVIAL,"Out of GameMainState::createWorld");
}

