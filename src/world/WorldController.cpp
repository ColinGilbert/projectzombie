#include <sstream>
#include <iostream>



using std::cout; using std::endl;

#include "world/WorldController.h"
#include "world/VolumeMap.h"
#include "world/VolumeMapPaging.h"


using namespace Ogre;
//#include "cologre.h"
#include "EngineView.h"
#include "world/WorldMap.h"
#include "world/WorldConfig.h"
#include "world/PhysicsManager.h"
#include "world/WorldDefs.h"


using namespace ZGame;
using namespace ZGame::World;

WorldController::WorldController() :
_worldMap(0), _scnMgr(0), _volumePaging(0),
    _physicsMgr(0)
{
    //init();

}

WorldController::~WorldController()
{
    _physicsMgr.reset(0);
    _volumeMap.reset(0);
}


//Life cycle events
bool WorldController::onInit(ZGame::ZInitPacket packet)
{
    _init();
    return true;
}

bool WorldController::onUpdate(const Ogre::FrameEvent &evt)
{
    using namespace std;
    Ogre::Real inc = evt.timeSinceLastFrame*0.8f;
    _physicsMgr->onUpdate(evt);
    _volumeMap->onUpdate(evt);
    //_animState->addTime(inc);
    //_animState->setWeight(1.0);

    //cout << "WorldController::onUpdate" << endl;
    return true;
}

bool WorldController::onDestroy()
{
    cout << "WorldController::onDestroy called." << endl;
    cout << "Deleting WorldMap." << endl;
    _worldMap.reset(0);
    OGRE_DELETE_T(_volumePaging, VolumeMapPaging, Ogre::MEMCATEGORY_GENERAL);
    cout << "WorldController::onDestroy done." << endl;
    return true;
}

/**
*This method will initialize the world controller.
*
*\note We're putting some world load stuff in the init function because we're testing. When we get to implementing this fully we nee to have people call
*the controller method to accomplish loading the world. ie. to do the world controller thing.
*/
void WorldController::_init()
{
    WorldConfig config;
    config.load();
    _scnMgr = EngineView::getSingleton().getSceneManager();
    _physicsMgr.reset(new PhysicsManager());
    _physicsMgr->onInit();
    _loadWorldMap(config.getWorldMapConfig());
    //log->logMessage(Ogre::LML_TRIVIAL,"Out WorldController::init().");
}

void 
    WorldController::_loadWorldMap(WorldMapConfig &config)
{
    //_worldMap.reset(new WorldMap());
    //_worldMap->load(); //We should implement load from configuration file.
    size_t numOfPagesPerAxis = (config.unloadRadius  / WORLD_BLOCK_WIDTH * 2 + 1);
    _volumeMap.reset(new VolumeMap(numOfPagesPerAxis, config.forceSync));
    _volumeMap->load(_physicsMgr.get());

    _pageManager.setPageProvider(&_pageProvider);
    _pageManager.addCamera(EngineView::getSingleton().getCurrentCamera());
    _volumePaging = OGRE_NEW_T(VolumeMapPaging(&_pageManager), 
        Ogre::MEMCATEGORY_GENERAL);
    Ogre::PagedWorld* world = _pageManager.createWorld();
    _volumePaging->createWorldSection(world, _volumeMap.get(), config.loadRadius, 
        config.unloadRadius, config.minx, config.miny, 
        config.maxx, config.maxy, 
        EngineView::getSingleton().getSceneManager());
     //_volumePaging->createWorldSection(world, _volumeMap.get(), 320.0, 
        //_volumeMap->getRegionsHalfWidth(), -32768, -32768, 32768, 32768, EngineView::getSingleton().getSceneManager());
        //384.0, -2300, -2300, 2300, 2300, EngineView::getSingleton().getSceneManager());

}

void
    WorldController::addBlock(Ogre::Real cursorX, Ogre::Real cursorY)
{
    using namespace Ogre;
    using namespace OgreBulletCollisions;
    using namespace OgreBulletDynamics;
    Ogre::Ray rayTo;
    Ogre::Real searchDistance = 10.0f;
    Ogre::Camera* cam = EngineView::getSingleton().getCurrentCamera();
    cout << "cursorX, cursorY: " << cursorX << " , " << cursorY << endl;
    rayTo = cam->getCameraToViewportRay(cursorX, cursorY); 
    _volumeMap->addBlock(rayTo, searchDistance);
}

void
    WorldController::removeBlock(Ogre::Real cursorX, Ogre::Real cursorY)
{
    Ogre::Ray rayTo;
    Ogre::Camera* cam = EngineView::getSingleton().getCurrentCamera();
    cout << "cursorX, cursorY: " << cursorX << " , " << cursorY << endl;
    rayTo = cam->getCameraToViewportRay(cursorX, cursorY);
    Ogre::Real searchDistance = 10.0f;
    _volumeMap->removeBlock(rayTo, searchDistance);
}
