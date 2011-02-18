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
    _physicsMgr(0), _worldConfig(0), _viewport(0)
{
    //init();

}

WorldController::~WorldController()
{
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
    cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << endl;
    cout << "World Map config: " << endl;
    WorldMapConfig config = _worldConfig->getWorldMapConfig();
    cout << "on-screen map dimensions: " << config.loadRadius * 2 << "^2" << endl;
    cout << "unload radius: " << config.unloadRadius << endl;
    cout << "Outputing Volume Map stats: " << endl;
    _volumeMap->getProfileStats(cout);
    cout << endl;
    cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;

    cout << "WorldController::onDestroy called." << endl;
    cout << "Deleting WorldMap." << endl;



    _physicsMgr.reset(0);
    _volumeMap.reset(0);



    _pageManager.removeCamera(_cam);
    _viewport->removeListener(this);
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
    _viewport = _cam->getViewport();
    _viewport->addListener(this);
    _physicsMgr.reset(new PhysicsManager());
    _physicsMgr->onInit(packet);

    if(_worldConfig.get() == 0)
    {
        _worldConfig.reset(new World::WorldConfig());
        _worldConfig->load();
    }
    _loadWorldMap(_worldConfig->getWorldMapConfig());
    
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
    //_volumeMap.reset(new VolumeMap(_scnMgr, numOfPagesPerAxis, config.forceSync));
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
    WorldController::getCursor3dPosition(Ogre::Ray rayTo, Ogre::Vector3& position, Ogre::Real searchDistance)
{
    _volumeMap->getBlockCenterWithRay(rayTo, searchDistance, position);
}

void
    WorldController::getCursor3dPosition(Ogre::Ray rayTo, Ogre::Vector3& position, const Ogre::Plane &constraintPlane)
{
    //I don't think the case for non-intersection will occur. 
    Ogre::Real searchDistance = 64.0f;
    std::pair<bool, Ogre::Real> retStruct = rayTo.intersects(constraintPlane);
    if(retStruct.first == true)
    {
        position = rayTo.getPoint(retStruct.second);
    }
    else
        position = rayTo.getPoint(searchDistance);
    _volumeMap->toVoxelCoords(position); //convert to voxel coords.
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

void
    WorldController::viewportCameraChanged(Ogre::Viewport* viewport)
{
    Ogre::Camera* tempCam = viewport->getCamera();

    //Here we assume pageManager always has a single camera and it is _cam.
    if(!_pageManager.hasCamera(tempCam))
    {
        _pageManager.removeCamera(_cam);
        _pageManager.addCamera(tempCam);
    }
    _cam = tempCam;
}

void
    WorldController::onSelectionRegion(const Ogre::AxisAlignedBox &selectBox, size_t materialId)
{
    //ask the volume map to handle this selection box. 
    _volumeMap->fillSelection(selectBox, materialId);
}