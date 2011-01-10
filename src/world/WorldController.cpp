#include <sstream>
#include <iostream>



using std::cout; using std::endl;

#include "world/WorldController.h"
#include "world/VolumeMap.h"
#include "world/VolumeMapPaging.h"

#include "ZInitPacket.h"

using namespace Ogre;
//#include "cologre.h"
#include "world/WorldMap.h"
#include "world/WorldConfig.h"
#include "world/PhysicsManager.h"
#include "world/WorldDefs.h"
#include "world/WorldConfig.h"

using namespace ZGame;
using namespace ZGame::World;

WorldController::WorldController() :
_scnMgr(0), _volumePaging(0),
    _physicsMgr(0), _worldConfig(0)
{
    //init();

}

WorldController::~WorldController()
{
    _physicsMgr.reset(0);
    _volumeMap.reset(0);
}


//Life cycle events
bool WorldController::onInit(ZGame::ZInitPacket *packet)
{
    _init(packet);
    return true;
}

bool WorldController::onUpdate(const Ogre::FrameEvent &evt)
{
    using namespace std;
    Ogre::Real inc = evt.timeSinceLastFrame*0.8f;
    _physicsMgr->onUpdate(evt);
    _volumeMap->onUpdate(evt);
    return true;
}

bool WorldController::onDestroy()
{
    cout << "WorldController::onDestroy called." << endl;
    cout << "Deleting WorldMap." << endl;
    _pageManager.removeCamera(_cam);
    OGRE_DELETE_T(_volumePaging, VolumeMapPaging, Ogre::MEMCATEGORY_GENERAL);
    cout << "WorldController::onDestroy done." << endl;
    return true;
}

/**
*This method will initialize the world controller.
*
*\note We're putting some world load stuff in the init function because we're testing. 
*When we get to implementing this fully we nee to have people call
*the controller method to accomplish loading the world. ie. to do the world controller thing.
*/
void WorldController::_init(ZGame::ZInitPacket *packet)
{
    _scnMgr = packet->sceneManager;
    _cam = packet->initialCamera;
    _physicsMgr.reset(new PhysicsManager());
    _physicsMgr->onInit(packet);

    if(_worldConfig.get() == 0)
    {
        _worldConfig.reset(new World::WorldConfig());
        _worldConfig->load();
    }
    _loadWorldMap(_worldConfig->getWorldMapConfig());
    _worldConfig.reset(0);
}

void
    WorldController::setWorldConfiguration(std::auto_ptr<WorldConfig> worldConfig)
{
    _worldConfig = worldConfig;
}



void 
    WorldController::_loadWorldMap(WorldMapConfig &config)
{
    //_worldMap.reset(new WorldMap());
    //_worldMap->load(); //We should implement load from configuration file.
    size_t numOfPagesPerAxis = (config.unloadRadius  / WORLD_BLOCK_WIDTH * 2 + 1);
    _volumeMap.reset(new VolumeMap(_scnMgr, numOfPagesPerAxis, config.forceSync));
    _volumeMap->load(_physicsMgr.get());

    _pageManager.setPageProvider(&_pageProvider);
    _pageManager.addCamera(_cam);
    _volumePaging = OGRE_NEW_T(VolumeMapPaging(&_pageManager), 
        Ogre::MEMCATEGORY_GENERAL);
    Ogre::PagedWorld* world = _pageManager.createWorld();
    _volumePaging->createWorldSection(world, _volumeMap.get(), config.loadRadius, 
        config.unloadRadius, config.minx, config.miny, 
        config.maxx, config.maxy, 
        _scnMgr);
}

void
    WorldController::addBlock(Ogre::Real cursorX, Ogre::Real cursorY)
{
    using namespace Ogre;
    using namespace OgreBulletCollisions;
    using namespace OgreBulletDynamics;
    Ogre::Ray rayTo;
    Ogre::Real searchDistance = 10.0f;
    cout << "cursorX, cursorY: " << cursorX << " , " << cursorY << endl;
    rayTo = _cam->getCameraToViewportRay(cursorX, cursorY); 
    _volumeMap->addBlock(rayTo, searchDistance);
}

void
    WorldController::removeBlock(Ogre::Real cursorX, Ogre::Real cursorY)
{
    Ogre::Ray rayTo;
    cout << "cursorX, cursorY: " << cursorX << " , " << cursorY << endl;
    rayTo = _cam->getCameraToViewportRay(cursorX, cursorY);
    Ogre::Real searchDistance = 10.0f;
    _volumeMap->removeBlock(rayTo, searchDistance);
}
