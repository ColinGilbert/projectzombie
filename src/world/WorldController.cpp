#include <sstream>
#include <iostream>

#include "world/WorldController.h"
#include "cologre.h"
#include "EngineView.h"
using namespace ZGame;
using namespace ZGame::World;

WorldController::WorldController() :
_bobEnt(0), _animState(0)
{
}

WorldController::~WorldController()
{
}

int WorldController::execute(Command)
{
  return 0;
}

//Life cycle events
bool WorldController::onInit()
{
  return true;
}

bool WorldController::onUpdate(const Ogre::FrameEvent &evt)
{
  using namespace std;
  Ogre::Real inc = evt.timeSinceLastFrame*0.8;

  //_animState->addTime(inc);
  //_animState->setWeight(1.0);
  
  //cout << "WorldController::onUpdate" << endl;
  return true;
}

bool WorldController::onDestroy()
{
  return true;
}


void WorldController::init()
{
  using namespace std;

  std::ostringstream oss;

  CColladaDatabase db;
  Ogre::SceneManager* sceneManager = EngineView::getSingleton().getSceneManager();
  Ogre::LogManager* log = Ogre::LogManager::getSingletonPtr();
  
  log->logMessage(Ogre::LML_TRIVIAL,"In WorldController::init");
  log->logMessage(Ogre::LML_TRIVIAL,"Load DAE file.");
  int ret = 0;
  //if(ret = db.load("data/characters/bob/bob.dae"))
  if(ret = db.load("data/nukeonetestworld.dae"))
  {
    throw std::invalid_argument("Cannot find a dae to load in WorldController::init.");
    return;
  }
  log->logMessage(Ogre::LML_NORMAL,"Finished loading dae.");

  //Create the missing custom resource group
  Ogre::ResourceGroupManager::getSingleton().createResourceGroup("Custom");
  Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Custom");

  CResourceConverter::m_convOptions.flipTextureH = true;
  db.convertResources();
  log->logMessage(Ogre::LML_NORMAL,"Finished coverting resources.");
  db.convertScene(sceneManager);
  log->logMessage(Ogre::LML_NORMAL,"Finished converting scene.");

  //setup the camera
  Ogre::Camera* cam = EngineView::getSingleton().getCurrentCamera();
  //cam->setPosition(-6.97490,-7.11573,4.90586);
  
  //cam->roll(Ogre::Radian(Ogre::Math::DegreesToRadians(120.82256)));
  //cam->roll(Ogre::Math::DegreesToRadians(120.82256)).valueDegrees());
  //cam->yaw(Ogre::Radian(Ogre::Math::DegreesToRadians(-39.75333)));
  //cam->pitch(Ogre::Radian(Ogre::Math::DegreesToRadians(37.26105)));

  //EngineView::getSingleton().setCurrentCamera(sceneManager->getCamera("Camera"));
  
  log->logMessage(Ogre::LML_NORMAL,"Finished setting dae camera to current camera.");

  log->logMessage(Ogre::LML_NORMAL,"Loading bob character mesh.");
  
  /*
  Ogre::SceneNode* node;
  Ogre::SceneNode* root = sceneManager->getRootSceneNode();
  node = root->createChildSceneNode("BOB_SCN_NODE");
  _bobEnt = sceneManager->createEntity("bobent","Helmet.mesh");

  node->attachObject(_bobEnt);
  _animState = _bobEnt->getAnimationState("wlk_s");
  _animState->setEnabled(true);
  oss << "Animation: " << _animState->getAnimationName() << endl;
  oss << "Length: " << _animState->getLength() << endl;
  log->logMessage(Ogre::LML_NORMAL,oss.str());
  _animState->setLoop(true);
  */
  

  

  log->logMessage(Ogre::LML_TRIVIAL,"Out WorldController::init().");
}

void WorldController::loadWorld()
{
}
