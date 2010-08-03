#include <sstream>
#include <iostream>

using std::cout; using std::endl;

#include "world/WorldController.h"
//#include "cologre.h"
#include "EngineView.h"
#include "world/WorldMap.h"
using namespace ZGame;
using namespace ZGame::World;

WorldController::WorldController() :
_bobEnt(0), _animState(0), _worldMap(0)
{
    //init();
}

WorldController::~WorldController()
{
}


//Life cycle events
bool WorldController::onInit()
{
    init();
    return true;
}

bool WorldController::onUpdate(const Ogre::FrameEvent &evt)
{
    using namespace std;
    Ogre::Real inc = evt.timeSinceLastFrame*0.8f;

    //_animState->addTime(inc);
    //_animState->setWeight(1.0);

    //cout << "WorldController::onUpdate" << endl;
    return true;
}

bool WorldController::onDestroy()
{
    cout << "WorldController::onDestroy called." << endl;
    cout << "Deleting WorldMap." << endl;
    delete _worldMap;
    return true;
}

/**
*This method will initialize the world controller.
*
*\note We're putting some world load stuff in the init function because we're testing. When we get to implementing this fully we nee to have people call
*the controller method to accomplish loading the world. ie. to do the world controller thing.
*/
void WorldController::init()
{
    loadWorld();
    //disable collada ogre support because we need to compile it's dependencies using VS 2010. Will do that later.
    /*
    CColladaDatabase db;
    Ogre::SceneManager* sceneManager = EngineView::getSingleton().getSceneManager();
    Ogre::LogManager* log = Ogre::LogManager::getSingletonPtr();

    //log->logMessage(Ogre::LML_TRIVIAL,"In WorldController::init");
    //log->logMessage(Ogre::LML_TRIVIAL,"Load DAE file.");
    int ret = 0;

    log->logMessage(Ogre::LML_TRIVIAL,"Loading citymesh.");

    Ogre::Entity *cityEnt = sceneManager->createEntity("CityEnt","cityMesh.mesh");
    Ogre::StaticGeometry *sg = sceneManager->createStaticGeometry("CityArea");
    //sg->setCastShadows(true);
    sg->setRegionDimensions(Ogre::Vector3(200,200,200));
    sg->setOrigin(Ogre::Vector3(-200/2,0,-200/2));

    sg->addEntity(cityEnt,Ogre::Vector3::ZERO,Ogre::Quaternion::IDENTITY,Ogre::Vector3(100.0f,100.0f,100.0f));

    //sg->build();




    */

    /*
    //if(ret = db.load("data/characters/bob/bob.dae"))
    if(ret = db.load("data/citytestworld.dae"))
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
    */
    //setup the camera
    Ogre::Camera* cam = EngineView::getSingleton().getCurrentCamera();
    //cam->setPosition(-6.97490,-7.11573,4.90586);

    //cam->roll(Ogre::Radian(Ogre::Math::DegreesToRadians(120.82256)));
    //cam->roll(Ogre::Math::DegreesToRadians(120.82256)).valueDegrees());
    //cam->yaw(Ogre::Radian(Ogre::Math::DegreesToRadians(-39.75333)));
    //cam->pitch(Ogre::Radian(Ogre::Math::DegreesToRadians(37.26105)));

    //EngineView::getSingleton().setCurrentCamera(sceneManager->getCamera("Camera"));

    //log->logMessage(Ogre::LML_NORMAL,"Finished setting dae camera to current camera.");

    //log->logMessage(Ogre::LML_NORMAL,"Loading bob character mesh.");

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




    //log->logMessage(Ogre::LML_TRIVIAL,"Out WorldController::init().");
}

void WorldController::loadWorld()
{
    _worldMap = new WorldMap();
    _worldMap->load(); //We should implement load from configuration file.
}
