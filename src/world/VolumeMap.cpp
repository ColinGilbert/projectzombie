/*
* VolumeMap.cpp
*
*  Created on: Sep 21, 2010
*      Author: beyzend
*/
#include "world/VolumeMap.h"
#include "PolyVoxImpl/Utility.h"
#include "world/PerlinNoiseMapGen.h"
#include "world/FillMapGen.h"
#include "world/WorldDefs.h"
#include "world/ZCubicSurfaceExtractor.h"
#include "world/PhysicsManager.h"

#include <OgreBulletCollisionsRay.h>

#include <SurfaceMesh.h>
#include <MeshDecimator.h>
#include <CubicSurfaceExtractor.h>

using std::cout;
using std::endl;

using ZGame::World::VolumeMap;
using PolyVox::ZCubicSurfaceExtractor;
using PolyVox::MaterialDensityPair44;
using PolyVox::Volume;
using PolyVox::Vector3DFloat;
using PolyVox::Vector3DUint16;
using PolyVox::Vector3DInt16;
using std::shared_ptr;
using PolyVox::CubicSurfaceExtractor;
using PolyVox::SurfaceMesh;
using namespace ZGame::World;
using namespace Ogre;

#define ONE_VOLUME_ONE_CHUNK 0

const Ogre::uint16 VolumeMap::WORKQUEUE_LOAD_REQUEST = 1;


VolumeMap::VolumeMap(Ogre::SceneManager* scnMgr, size_t volSideLenInPages, bool FORCE_SYNC) : _scnMgr(scnMgr),
_volSideLenInPages(volSideLenInPages), _volSizeInBlocks(volSideLenInPages*WORLD_BLOCK_WIDTH), _volHeight(WORLD_HEIGHT),
    _FORCE_SYNC(FORCE_SYNC), _totalVolInKB(0.0), _totalNumOfChunks(0), _avgLoadTime(0.0),
    _avgCompression(0.0), _avgGenerationTime(0.0), _avgExtractionTime(0.0), _avgViewTime(0.0),
    _totalVolInKBDeallocated(0.0),
    SHARED_BLOCK_SIZE(8), UNCOMPRESSED_CACHE_SIZE(1)
{
    World::PerlinNoiseMapGen::initGradientPoints();
}

VolumeMap::~VolumeMap()
{
    _freeAll();
}

void
    VolumeMap::_freeAll()
{
   
    PagesMap::iterator iter = _pagesMap.begin();
    for (iter = _pagesMap.begin(); iter != _pagesMap.end(); ++iter)
    {
        OGRE_DELETE_T(iter->second, VolumePage, Ogre::MEMCATEGORY_GENERAL);
    }
    _pagesMap.clear();
   
}

bool
    VolumeMap::canHandleRequest(const WorkQueue::Request* req, const WorkQueue* srcQ)
{
    LoadRequest lreq = any_cast<LoadRequest> (req->getData());
    if (lreq.origin != this)
        return false;
    else
        return RequestHandler::canHandleRequest(req, srcQ);
}
/**
* This method will create a region in local Volume space by transform the volume page id, and Ogre page id.
* \precondition the coordinates to be transformed MUST BE in BLOCK (voxel) units!!! volumeOrigin must be PAGE units.
**/
Ogre::Vector2 
    VolumeMap::_transformToVolumeLocal(Ogre::Vector2 volumeOrigin, Ogre::Vector2 local,
    size_t volSideLenInBlocks)
{
#if !ONE_VOLUME_ONE_CHUNK
    size_t halfVolSide = volSideLenInBlocks / 2;
    Ogre::Vector2 coords = local + Ogre::Vector2(halfVolSide, halfVolSide); //shift it to zero.
    coords -= volumeOrigin * volSideLenInBlocks; //transform into local space.
    return coords;
#else
    return Ogre::Vector2(0.0f, 0.0f);
#endif
}

WorkQueue::Response*
    VolumeMap::handleRequest(const WorkQueue::Request* req, const WorkQueue* srcQ)
{
#if PROFILE
    CPerfCounter counter;
    counter.Start();
#endif
    LoadRequest lreq = any_cast<LoadRequest> (req->getData());

    VolumePage* page = lreq.page;
    WorkQueue::Response* response = 0;
    long volx, voly; long localx, localy;
    _unpackIndex(page->id, &volx, &voly);
    _unpackIndex(lreq.ogreId, &localx, &localy);
    //transform ids
    Ogre::Vector2 volumeOrigin(volx, voly);
    Ogre::Vector2 pageCoords(static_cast<Ogre::Real>(localx) * WORLD_BLOCK_WIDTH, 
        static_cast<Ogre::Real>(localy) * WORLD_BLOCK_WIDTH);
    Ogre::Vector2 localOrigin = _transformToVolumeLocal(volumeOrigin, pageCoords, _volSizeInBlocks);
    Ogre::Vector2 upperCorner = localOrigin + Ogre::Vector2(WORLD_BLOCK_WIDTH, WORLD_BLOCK_WIDTH);
    PolyVox::Region pageRegion(PolyVox::Vector3DInt16(localOrigin.x, 0, localOrigin.y),
        PolyVox::Vector3DInt16(upperCorner.x, WORLD_HEIGHT, upperCorner.y));
    //World::TestMapGenerator gen;
    World::PerlinNoiseMapGen gen;
    gen.generate(&page->data, pageRegion, static_cast<Ogre::Real>(localx), 
        static_cast<Ogre::Real>(localy));
#if PROFILE
    counter.Stop();
    page->generateTime = counter.GetElapsedTime();
    counter.Start();
#endif
    //PolyVox::CubicSurfaceExtractor<PolyVox::Material8> surfExtractor(&page->data, pageRegion, lreq.surface);
    //ZCubicSurfaceExtractor<PolyVox::Material8> surfExtractor(&page->data, pageRegion, lreq.tempSurface);
    ZCubicSurfaceExtractor<PolyVox::Material8> surfExtractor(&page->data, pageRegion, lreq.surface);
    surfExtractor.execute();
    //PolyVox::MeshDecimator<PolyVox::PositionMaterial> decimator(lreq.tempSurface, lreq.surface);
    //decimator.execute();
#if PROFILE
    counter.Stop();
    page->extractionTime = counter.GetElapsedTime() - page->generateTime;
#endif
    response = new WorkQueue::Response(req, true, Any());
    return response;
}

bool
    VolumeMap::canHandleResponse(const WorkQueue::Response* res, const WorkQueue* srcQ)
{
    LoadRequest lreq = any_cast<LoadRequest> (res->getRequest()->getData());
    if (lreq.origin != this)
        return false;
    else
        return true;
}
/**
** \note Please review this code to see if they are better ways for error handling (see the allocation code).  
*/
void
    VolumeMap::handleResponse(const WorkQueue::Response* res, const Ogre::WorkQueue* srcQ)
{
    using Ogre::WorkQueue;
    using PolyVox::SurfaceMesh;
    LoadRequest lreq = any_cast<LoadRequest> (res->getRequest()->getData());
    if (res->succeeded())
    {
        VolumePage* page = lreq.page;
        long x, z;
        _unpackIndex(lreq.ogreId, &x, &z);
        
        PageRegion* region = page->getRegion(lreq.ogreId);
        if(!region->deferredUnload)
        {
#if PROFILE
            _perfCounter.Reset();
            _perfCounter.Start();
#endif
            region->loading = false;
            region->mapView.createRegion(page->worldOrigin, lreq.surface);
            region->mapView.finalizeRegion();
        
            region->mapView.createPhysicsRegion(_phyMgr);
#if PROFILE
            _perfCounter.Stop();
            //double compressionRatio = page->data.calculateCompressionRatio();
            //_avgCompression = (compressionRatio + _totalNumOfChunks * _avgCompression) / (_totalNumOfChunks + 1);
            //_totalVolInKB += page->data.calculateSizeInBytes() / 1000.0;
            
            //time
            _avgViewTime = (_perfCounter.GetElapsedTime() + _totalNumOfChunks * _avgViewTime) / (_totalNumOfChunks + 1);
            _avgGenerationTime = (page->generateTime + _totalNumOfChunks * _avgGenerationTime) / (_totalNumOfChunks + 1);
            _avgExtractionTime = (page->extractionTime + _totalNumOfChunks * _avgExtractionTime) / (_totalNumOfChunks + 1);
            _avgLoadTime = _avgViewTime + _avgGenerationTime + _avgExtractionTime;
            _totalNumOfChunks++;
              //cout << "compression for chunk: " << compressionRatio << endl;
            //cout << "avgCompression: " << _avgCompression << endl;
            //cout << "total volume size in kb: " << _totalVolInKB << endl;    
            //cout << "average view time: " << _avgViewTime << endl;
            //cout << "average generation time: " << _avgGenerationTime << endl;
            //cout << "average extraction time: " << _avgExtractionTime << endl;
            //cout << "average elapsed time: " << _avgLoadTime << endl;         
#endif 
        }
        else
        {
            _unloadPageRegion(page, lreq.ogreId);
        }
        OGRE_DELETE_T(lreq.surface, SurfaceMesh, Ogre::MEMCATEGORY_GENERAL);
        //OGRE_DELETE_T(lreq.tempSurface, SurfaceMesh, Ogre::MEMCATEGORY_GENERAL);
    }
    else
    {
        Ogre::LogManager::getSingleton().logMessage(Ogre::LML_CRITICAL, "FAILED TO HANDLE RESPONSE in VOLUME_MAP");
    }

    //OGRE_DELETE_T(res, Response, Ogre::MEMCATEGORY_GENERAL);

}

#if PROFILE
    void
        VolumeMap::getProfileStats(std::ostream &out)
    {
        cout << "Shared Block Size: " << SHARED_BLOCK_SIZE << endl;
        cout << "Uncompressed Cache Size: " << UNCOMPRESSED_CACHE_SIZE << endl;
        //We planned to do something more complicated but screw it. Let's just look at this in console output and manually paste it to a file.
        out << "avgCompression: " << _avgCompression << endl;
        out << "total volume size in kb: " << _totalVolInKB << endl;    
        out << "average graphics and physics upload time: " << _avgViewTime << endl;
        out << "average generation time: " << _avgGenerationTime << endl;
        out << "average extraction time: " << _avgExtractionTime << endl;
        out << "average elapsed time: " << _avgLoadTime << endl;         
    }
#endif

void
    VolumeMap::_initVolumes()
{

    assert(_volSizeInBlocks >= WORLD_BLOCK_WIDTH && "Invalid num of pages.");
    assert((_volSizeInBlocks % WORLD_BLOCK_WDITH == 0) && "numOfPages per axis MUST be a multiple of WORLD_BLOCK_SIZE");
    size_t powerOfTwoSize = WORLD_BLOCK_WIDTH; //It should be no less than this.
    while(_volSizeInBlocks != powerOfTwoSize && _volSizeInBlocks % powerOfTwoSize != _volSizeInBlocks)
    {
        powerOfTwoSize *= 2;
    }
    _volSideLenInPages = powerOfTwoSize / WORLD_BLOCK_WIDTH;
    Ogre::PageID pageId = _packIndex(0, 0);
    _allocateVolume(pageId, powerOfTwoSize, _volHeight);
    _volSizeInBlocks = powerOfTwoSize;
}

VolumeMap::VolumePage*
    VolumeMap::_allocateVolume(Ogre::PageID pageId, size_t size, size_t height)
{
    PagesMap::iterator findMe = _pagesMap.find(pageId);
    if(findMe == _pagesMap.end())
    {
        VolumePage* page = OGRE_NEW_T(VolumePage, Ogre::MEMCATEGORY_GENERAL)(size, height,
            SHARED_BLOCK_SIZE, UNCOMPRESSED_CACHE_SIZE);
        page->id = pageId;
        long x,y;
        _unpackIndex(pageId, &x, &y);
#if !ONE_VOLUME_ONE_CHUNK
        // Use this if MORE THAN ONE chunk per VOLUME
        Ogre::Vector3 volWorldOrigin(size * (static_cast<Ogre::Real>(x) - 1.0f / 2.0f), 
            0.0f, size * (static_cast<Ogre::Real>(y) - 1.0 / 2.0f) );
            
#else
        //size = 16;
        Ogre::Vector3 volWorldOrigin(size * (static_cast<Ogre::Real>(x)), 
            0.0f, size * (static_cast<Ogre::Real>(y)));
#endif
        page->worldOrigin = volWorldOrigin;
        _pagesMap[pageId] = page;
        return page;
    }
    else
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Trying to allocate an existing Volume.", "VolumeMap::_allocateVolume");
    return 0;
}

void
    VolumeMap::load(PhysicsManager* phyMgr)
{
    _phyMgr = phyMgr;
    WorkQueue* wq = Root::getSingleton().getWorkQueue();
    _workQueueChannel = wq->getChannel("PROJECT_ZOMBIE/VolumeMap");
    wq->addRequestHandler(_workQueueChannel, this);
    wq->addResponseHandler(_workQueueChannel, this);
    _initVolumes();

}

void
    VolumeMap::onUpdate(const Ogre::FrameEvent &evt)
{
    _processLoadQueue();
    _processFillCommands();
    _processDirtyRegions();
}

/**
* This method will convert Ogre's paging system's ID into Volume ID, which is used to reference Volumes for this VolumeMap.
**/
Ogre::PageID
    VolumeMap::_pageIdToVolumeId(Ogre::PageID pageId, size_t volSideLenInPages)
{
    long x, z;
    _unpackIndex(pageId, &x, &z);
    x = Ogre::Math::Floor(static_cast<Ogre::Real>(x) / volSideLenInPages + 0.5f);
    z = Ogre::Math::Floor(static_cast<Ogre::Real>(z) / volSideLenInPages + 0.5f);
    return _packIndex(x, z);
}

void VolumeMap::loadPage(Ogre::PageID pageID)
{
    long x, y;
    _unpackIndex(pageID, &x, &y);
    y = -y;
    pageID = _packIndex(x, y);
    Ogre::PageID volumeId = _pageIdToVolumeId(pageID, _volSideLenInPages);
    PagesMap::iterator findMe = _pagesMap.find(volumeId);
    VolumePage* page;
    if(findMe == _pagesMap.end())
    {
        page = _allocateVolume(volumeId, _volSizeInBlocks, _volHeight);
    }
    else
        page = findMe->second;
    PageRegion* region = page->createRegion(pageID, _scnMgr);
    if(region)
    {
        region->loading = true;
        _loadQueue.push_back(std::make_pair(page, std::make_pair(pageID, region)));
    }
}

void
    VolumeMap::_processLoadQueue()
{
    while(!_loadQueue.empty())
    {
        std::pair<VolumePage*, std::pair<Ogre::PageID, PageRegion*> > pair = _loadQueue.front();
        _loadQueue.pop_front();
        if(!pair.second.second->deferredUnload)
        {
            LoadRequest req;
            req.origin = this;
            req.page = pair.first;
            req.ogreId = pair.second.first;
            req.surface = OGRE_NEW_T (PolyVox::SurfaceMesh<PolyVox::PositionMaterial>, Ogre::MEMCATEGORY_GENERAL);
            //req.tempSurface = OGRE_NEW_T (PolyVox::SurfaceMesh<PolyVox::PositionMaterial>, Ogre::MEMCATEGORY_GENERAL);
            //stats track
            Root::getSingleton().getWorkQueue()->addRequest(_workQueueChannel, WORKQUEUE_LOAD_REQUEST, Any(req), 10, _FORCE_SYNC);
        }
        else
        {
            _unloadPageRegion(pair.first, pair.second.first);
        }
    }
}

void 
    VolumeMap::unloadPage(Ogre::PageID pageID)
{
    long x, y;
    _unpackIndex(pageID, &x, &y);
    y = -y;
    pageID = _packIndex(x, y);
    cout << "Unloading PageID: " << pageID << endl;
    cout << "pageID: " << x << " " << y << endl;
    Ogre::PageID volumeId = _pageIdToVolumeId(pageID, _volSideLenInPages);

    PagesMap::iterator findMe = _pagesMap.find(volumeId);
    
    if(findMe == _pagesMap.end())
    {
        return;
        //Why is this so? Situation can arise where the task system is unable to cope 
        //with loading under certain loading constraints (it will throw unhandled exceptions).
        //In this case it tries to unload a page that it thinks is in the system, when in fact
        //it is not, due to the thread system not being able to actually create that page.
        //OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, 
          //  "Trying to unload a page from a non-existent Volume", "VolumeMap::unloadPage");
    }
    PageRegion* region = findMe->second->getRegion(pageID);
    if(region)
    {
        if(!region->loading)
        {
            
            region->mapView.unloadRegion(_phyMgr);
            _unloadPageRegion(findMe->second, pageID);        
        }
        else//else deferred unloading to handle response, which unloads right after it loads
            region->deferredUnload = true;
    }
}

/**
* \note invalid pointers are not checked!
**/
void
    VolumeMap::_unloadPageRegion(VolumePage* page, Ogre::PageID regionId)
{ 
    page->removeRegion(regionId);
    if(page->isEmpty())
    {
        cout << "deleting page" << endl;
        Ogre::PageID id = page->id;
        OGRE_DELETE_T(page, VolumePage, Ogre::MEMCATEGORY_GENERAL);
        _pagesMap.erase(id);

    }
}

inline Ogre::uint32
    VolumeMap::_packIndex(long x, long y)
{
    using namespace Ogre;
    using Ogre::uint32;
    using Ogre::int16;
    using Ogre::uint16;
    // Convert to signed 16-bit so sign bit is in bit 15
    int16 xs16 = static_cast<int16> (x);
    int16 ys16 = static_cast<int16> (y);

    // convert to unsigned because we do not want to propagate sign bit to 32-bits
    uint16 x16 = static_cast<uint16> (xs16);
    uint16 y16 = static_cast<uint16> (ys16);

    uint32 key = 0;
    key = (x16 << 16) | y16;

    return key;
}

inline void
    VolumeMap::_unpackIndex(Ogre::PageID pageID, long *x, long *y)
{
    using Ogre::uint16;
    using Ogre::int16;
    uint16 y16 = static_cast<uint16> (pageID & 0xFFFFF);
    uint16 x16 = static_cast<uint16> ((pageID >> 16) & 0xFFFF);

    *x = static_cast<int16> (x16);
    *y = static_cast<int16> (y16);
}

 static const Vector3 faces[6] = {
    Ogre::Vector3(0.5f, 0.0f, 0.0f),
    Ogre::Vector3(-0.5f, 0.0f, 0.0f),
    Ogre::Vector3(0.0f, 0.5f, 0.0f),
    Ogre::Vector3(0.0f, -0.5f, 0.0f),
    Ogre::Vector3(0.0f, 0.0f, 0.5f),
    Ogre::Vector3(0.0f, 0.0f, -0.5f)};

inline void
    VolumeMap::_getCenterOfIntersectedPoint(Ogre::Vector3& point, Ogre::Vector3 rayDir)
{
    Ogre::Vector3 cubeCenter(Ogre::Math::Floor(point.x + 0.5f),
    Ogre::Math::Floor(point.y + 0.5f),
    Ogre::Math::Floor(point.z + 0.5f));
   
    for(size_t i = 0; i < 6; ++i)
    {
        Ogre::Vector3 pointOnFace = cubeCenter + faces[i];
        
        Ogre::Vector3 canadidateVecInPlane = point - pointOnFace; 
        //If canadidate and faceNormal is orthgonal then onTheFace is in the plane of the face.
        Ogre::Real dotp = canadidateVecInPlane.dotProduct(faces[i]);
        //Ogre::Real epsilon = std::numeric_limits<Ogre::Real>::epsilon(); //Are you retarded? The float tolrance from dotp can be very
        //small yet still greater than epsilon.
        const Ogre::Real epsilon = 0.0001f;
        
        if(dotp <= epsilon && dotp >= -epsilon)
        {
            //Depending on the direction of ray we pick the cube next to it.
            if(faces[i].dotProduct(rayDir) >= 0.0f)
            {
               cubeCenter += faces[i]*2.0f;
            }
            break;
        }
    }

    point = cubeCenter;
}

void
    VolumeMap::fillSelection(const Ogre::AxisAlignedBox& aabb, uint8_t data)
{
    using Ogre::Vector3;
    //Get the total length of the box along Z.
    Vector3 size = aabb.getSize();
    Vector3 leftCorner = aabb.getCorner(Ogre::AxisAlignedBox::NEAR_LEFT_BOTTOM);
    cout << "VolumMap::fillSelection" << endl;
    cout << "size: " << size << endl;
    cout << "leftCorner: " << leftCorner << endl;
    _fillSelection(Ogre::Math::Abs(size.z), Ogre::Math::Abs(size.x), Ogre::Math::Abs(size.y), leftCorner, data);
}

inline void
    VolumeMap::_fillSelection(Ogre::Real totalZLenInVoxels, Ogre::Real totalXLenInVoxels, Ogre::Real totalYLenInVoxels,
    const Ogre::Vector3 &leftCorner, uint8_t data)
{
    //Note: There is a more optimized method that gets rid of the if branching during ever loop iteration. We can do that in the future.
    //basically, snip off the ends and process the full "tiles" in between. THen process the snipped ends. 
    /*
    *This algorithm will loop through all voxel regions for the given length along Z and X. It does this by adding offsets from the
    *length and subtracting this offset from the length until the length is gone. The offsets are tried to be filled by
    *BLOCK_WIDTH (the width of the chunk). So this means basically the ends are are the only regions not the size
    *of full region chunks. By the end of iterations all regions touched by the given lenghs have been visited. 
    */
    int curZ = static_cast<int>(leftCorner.z);
    do
    {
        int zOffset = totalZLenInVoxels;
        int bottomZ = (curZ / WORLD_BLOCK_WIDTH) * WORLD_BLOCK_WIDTH + WORLD_BLOCK_WIDTH - 1;
        int regionEndZ = curZ + zOffset;
        if(curZ + totalZLenInVoxels > bottomZ)
        {
            zOffset = bottomZ - curZ;
            regionEndZ = curZ + zOffset;
        }
        
        int curX = static_cast<int>(leftCorner.x);
        Ogre::Real tempXLen = totalXLenInVoxels;

        do
        {
            int xOffset = tempXLen;
            int rightX = (curX / WORLD_BLOCK_WIDTH) * WORLD_BLOCK_WIDTH + WORLD_BLOCK_WIDTH -1;
            int regionEndX = curX + xOffset;
            if(curX + tempXLen > rightX)
            {
                xOffset = rightX - curX;
                regionEndX = curX + xOffset;
            }

            //For now, Y is constant and is totalYLenInVoxels
            _addFillRegionCommand(curX, static_cast<int>(leftCorner.y), curZ, regionEndX, 
                static_cast<int>(leftCorner.y) + totalYLenInVoxels, regionEndZ, data);

            tempXLen -= xOffset + 1;
            curX = regionEndX + 1;

            

        }while(tempXLen > 0);

        totalZLenInVoxels -= zOffset + 1;
        curZ = regionEndZ + 1;

    }while(totalZLenInVoxels > 0);
}

inline void
    VolumeMap::_addFillRegionCommand(int startX, int startY, int startZ, 
    int endX, int endY, int endZ, 
    uint8_t data)
{
    PageFillCmd cmd;
    cmd.regionStart = Ogre::Vector3(startX, startY, startZ);
    cmd.regionEnd = Ogre::Vector3(endX, endY, endZ);
    cmd.data = data;

    _fillVolumeCmd.push_back(cmd);
}

/**
* \note this method must be executed in the same thread as _addFillRegionCommand.
**/
void
    VolumeMap::_processFillCommands()
{
    while(!_fillVolumeCmd.empty())
    {
        PageFillCmd& cmd = _fillVolumeCmd.front();
        _executeFillCommand(cmd);
        _fillVolumeCmd.pop_front();
    }
}

/**
* This method will iterate through the dirty regions list and unload then reload those regions. 
*
*/
void
    VolumeMap::_processDirtyRegions()
{
    //WARNING: This method is not 100% safe. In the other modifiy volume method (eventually we should merge these) we only modify a single voxel.
    //In this method IT IS POSSIBLE--though unlikely--that we can fill many regions at one, and many regions get dirtied. But we may try to update a dirted region
    //that is in the process of being deferred loaded. We are not taking that into account here. It is unlikely we will modify so many regions (at least for now) 
    //so this is unlikely to happen. However, we need to keep this in mind, such as if we use this to generate terrain.
    while(!_dirtyRegionsList.empty())
    {
        PageDirtyCmd &cmd = _dirtyRegionsList.front();
        cmd.region->mapView.unloadRegion(_phyMgr);
        PolyVox::Region pageRegion(PolyVox::Vector3DInt16(cmd.localStart.x, 0, cmd.localStart.y),
            PolyVox::Vector3DInt16(cmd.localEnd.x, WORLD_HEIGHT, cmd.localEnd.y));
        PolyVox::SurfaceMesh<PolyVox::PositionMaterial> surface;
        ZCubicSurfaceExtractor<Material8> surfExtractor(&cmd.page->data, pageRegion, &surface);
        surfExtractor.execute();
        cmd.region->mapView.updateRegion(&surface);
        cmd.region->mapView.finalizeRegion();
        cmd.region->mapView.createPhysicsRegion(_phyMgr);
        _dirtyRegionsList.pop_front();
    }
}


inline void
    VolumeMap::_getPage(Ogre::Vector3 cubeCenter, long &pageX, long &pageZ, Ogre::PageID &pageID,
    long &volX, long &volZ, Ogre::PageID &volID, 
    VolumePage** volPage, PageRegion** region)
{
    _cubeCoordsToPageXY(pageX, pageZ, cubeCenter);
    pageID = _packIndex(pageX, pageZ);
    volID = _pageIdToVolumeId(pageID, _volSideLenInPages);
    _unpackIndex(volID, &volX, &volZ);
    PagesMap::iterator findMe = _pagesMap.find(volID);
    if(findMe != _pagesMap.end())
    {
        *volPage = findMe->second;
        *region = (*volPage)->getRegion(pageID);
    }
}

inline void
    VolumeMap::_executeFillCommand(const PageFillCmd &cmd)
{
    //We may need to refactor entire class to work with only x and y. instead of page id.
    //Find the page region id and Volume ID for this.
    long pageX,pageZ,volX,volZ;
    Ogre::PageID pageID, volID;
    VolumePage* page = 0;
    PageRegion* region = 0;
    Ogre::Vector3 cubeCenter = cmd.regionStart;
    _getPage(cubeCenter, pageX, pageZ, pageID, volX, volZ, volID, 
        &page, &region);
    if(page && region)
    {
        //Now we want to define the region. The region we want to hit is regionStart, to regionEnd, but in local space of the Page.
        Ogre::Vector2 volumeOrigin(volX, volZ);
        Ogre::Vector2 cubeLocal = Ogre::Vector2(cubeCenter.x, cubeCenter.z);
        Ogre::Vector2 regionStart = _transformToVolumeLocal(volumeOrigin, cubeLocal, _volSizeInBlocks);
        cubeLocal = Ogre::Vector2(cmd.regionEnd.x, cmd.regionEnd.z);
        Ogre::Vector2 regionEnd = _transformToVolumeLocal(volumeOrigin, cubeLocal, _volSizeInBlocks);
        PolyVox::Region pageRegion(PolyVox::Vector3DInt16(regionStart.x, cubeCenter.y, regionStart.y),
            PolyVox::Vector3DInt16(regionEnd.x, cmd.regionEnd.y, regionEnd.y));
        World::FillMapGen gen;
        gen.generate(&page->data, pageRegion, cmd.data);
        //dirty the map
        PageDirtyCmd pageDirty;
        Ogre::Vector2 localPageWorld = _pageCoordToWorldCoord(pageX, pageZ);
        pageDirty.localStart = _transformToVolumeLocal(volumeOrigin, localPageWorld, _volSizeInBlocks);
        pageDirty.localEnd = pageDirty.localStart + Ogre::Vector2(WORLD_BLOCK_WIDTH, WORLD_BLOCK_WIDTH);
        pageDirty.page = page;
        pageDirty.region = region;
        _dirtyRegionsList.push_back(pageDirty);
    }
}

inline Ogre::Vector2
    VolumeMap::_pageCoordToWorldCoord(long pageX, long pageZ)
{
    return Ogre::Vector2(pageX * static_cast<long> (WORLD_BLOCK_WIDTH), pageZ * static_cast<long>(WORLD_BLOCK_WIDTH));
}

inline void
    VolumeMap::_cubeCoordsToPageXY(long &x, long &z, const Vector3 &cubeCenter)
{
 
    x = Ogre::Math::Floor(cubeCenter.x / WORLD_BLOCK_WIDTH);
    z = Ogre::Math::Floor(cubeCenter.z / WORLD_BLOCK_WIDTH);
}

/**
*This method will add a block to the volume. This method assumes that the passed in point is a point on a face of a cube,
*and the ray direction corresponds to the picking direction. (Note: This still works for empty space because the point of
*intersection is still assumed to be on the face of a cube, albiet an empty one.
*
**/
inline void
    VolumeMap::_modifyVolume(Ogre::Vector3 point, size_t blockType, Ogre::Vector3 rayDir,
    VOLUME_MODIFY_MODE mode)
{
    Ogre::PageID pageID;
    //We need to figure out which face. We need to hash the coordinate into a cube first,
    //this will determine the center of a cube. We then iterate through the faces of this cube
    //and tests whether the intersected point is in the face (point in plane). 
    Ogre::Vector3 cubeCenter(Ogre::Math::Floor(point.x + 0.5f),
        Ogre::Math::Floor(point.y + 0.5f),
        Ogre::Math::Floor(point.z + 0.5f));
    Vector3 faces[6];
    faces[0] = Ogre::Vector3(0.5f, 0.0f, 0.0f);
    faces[1] = Ogre::Vector3(-0.5f, 0.0f, 0.0f);
    faces[2] = Ogre::Vector3(0.0f, 0.5f, 0.0f);
    faces[3] = Ogre::Vector3(0.0f, -0.5f, 0.0f);
    faces[4] = Ogre::Vector3(0.0f, 0.0f, 0.5f);
    faces[5] = Ogre::Vector3(0.0f, 0.0f, -0.5f);
    cout << "-------------------" << endl;
    cout << "Cube center: " << cubeCenter << endl;
    cout << "point: " << point << endl;
    for(size_t i = 0; i < 6; ++i)
    {
        Ogre::Vector3 pointOnFace = cubeCenter + faces[i];
        cout << "pointOnFace: " << pointOnFace << endl;
        Ogre::Vector3 canadidateVecInPlane = point - pointOnFace; 
        cout << "canadidate vector is: " << canadidateVecInPlane << endl;
        //If canadidate and faceNormal is orthgonal then onTheFace is in the plane of the face.
        cout << "face normal: " << faces[i] << endl;
        Ogre::Real dotp = canadidateVecInPlane.dotProduct(faces[i]);
        cout << "dotp is " << dotp << endl;
        const Ogre::Real epsilon = 0.0001f;
        if(dotp <= epsilon && dotp >= -epsilon)
        {
            cout << "canadidate vector is in plane" << endl;
            //Depending on the direction of ray we pick the cube next to it.
            if(faces[i].dotProduct(rayDir) < 0.0f)
            {
                if(mode == ADD_BLOCK)
                    cubeCenter += faces[i]*2.0f; //move away from the current cube in the direction of face normal.
                cout << "ray dir and normal not same direction. Cube center: " << cubeCenter << endl;
            }
            else
            {
                if(mode == REMOVE_BLOCK)
                    cubeCenter += faces[i]*2.0f;
                cout << "ray dir and normal not same direction. Cube center: " << cubeCenter << endl;
            }
            break;
        }    
    }
    //first hash point to page id. We are mapping natural numbers into into natural numbers grouped by WORLD_BLOCK_WIDTH
    //and negative of the natural number (excluding zero) to the negative of natural numbers grouped by WORLD_BLOCK_WDITH
    long x, z;


    _cubeCoordsToPageXY(x, z, cubeCenter);

    pageID = _packIndex(x, z);
    Ogre::PageID volumeId = _pageIdToVolumeId(pageID, _volSideLenInPages);
    long vx, vy;
    _unpackIndex(volumeId, &vx, &vy);

   
    //Find this page id.
    cout << "Page idx: " << x << " " << z << endl;
    cout << "Volume idx: " << vx << " " << vy << endl;
    cout << "Cube space world coordinate: " << cubeCenter << endl;

    PagesMap::iterator findMe = _pagesMap.find(volumeId);
    if(findMe != _pagesMap.end())
    {
        //Found this page let's add block to it.
        VolumePage* page = findMe->second;

        //Transform into cube space local coordinates.
        Ogre::Vector2 volumeOrigin(vx, vy);
        
        Ogre::Vector2 cubeLocal = Ogre::Vector2(cubeCenter.x, cubeCenter.z);

        cubeLocal = _transformToVolumeLocal(volumeOrigin, cubeLocal, _volSizeInBlocks);
        
        PageRegion* region = page->getRegion(pageID);
        PageRegion* regionLeft = 0;
        PageRegion* regionBelow = 0;
        region->mapView.unloadRegion(_phyMgr);
     
        if(static_cast<long>(cubeLocal.x) % WORLD_BLOCK_WIDTH == 0)
        {
            Ogre::PageID leftId = _packIndex(x - 1, z);
            regionLeft = page->getRegion(leftId);
            if(regionLeft)
                regionLeft->mapView.unloadRegion(_phyMgr);
        }
        if(static_cast<long>(cubeLocal.y) % WORLD_BLOCK_WIDTH == 0)
        {
            Ogre::PageID belowId = _packIndex(x, z - 1);
            regionBelow = page->getRegion(belowId);
            if(regionBelow)
                regionBelow->mapView.unloadRegion(_phyMgr);

        }

        cout << "cube in local space: " << cubeLocal << endl;;
        
        //NOTE: THis is WIP. The better idea is to queue up modification commands. So we modify and mark as dirty. THEN generate
        //all dirtied regions in one get-to. This is much better because we eliminate branching.
        page->data.setVoxelAt(static_cast<uint16_t>(cubeLocal.x), static_cast<uint16_t>(cubeCenter.y), static_cast<uint16_t>(cubeLocal.y), 
                blockType);
        if(regionLeft) //ALL THIS just to remove a block on the border??? WTF. Okay, I store multiple pages per PolyVox::Volume to have better cache utilization.
            //As in this case, the page->data contains multiple pages and they are contingous in memory.
        {
            _updatePageRegion(x - 1, z, regionLeft, page, volumeOrigin);
            
        }
        if(regionBelow)
        {
            _updatePageRegion(x, z - 1, regionBelow, page, volumeOrigin);
        }

        _updatePageRegion(x, z, region, page, volumeOrigin);
    }
    //else throw exeception here this should never happen.
    assert(findMe != _pagesMap.end() && "VolumeMap::_addBlockToVolume trying to add to non-existent Volume page.");
}
/** \note Not checking region null.**/
void
    VolumeMap::_updatePageRegion(long pageX, long pageZ, PageRegion* region, 
    VolumePage* page, Ogre::Vector2 volumeOrigin)
{
    assert(region != 0 && "the region you are updating is null!");
    Ogre::Vector2 pageCoords(static_cast<Ogre::Real>(pageX) * WORLD_BLOCK_WIDTH, static_cast<Ogre::Real>(pageZ) * WORLD_BLOCK_WIDTH);
    Ogre::Vector2 localOrigin = _transformToVolumeLocal(volumeOrigin, pageCoords, _volSizeInBlocks);
    Ogre::Vector2 upperCorner = localOrigin + Ogre::Vector2(WORLD_BLOCK_WIDTH, WORLD_BLOCK_WIDTH);
    PolyVox::Region pageRegion(PolyVox::Vector3DInt16(localOrigin.x, 0, localOrigin.y),
        PolyVox::Vector3DInt16(upperCorner.x, WORLD_HEIGHT, upperCorner.y));
    PolyVox::SurfaceMesh<PolyVox::PositionMaterial> surface;
    ZCubicSurfaceExtractor<Material8> surfExtractor(&page->data, pageRegion, &surface);
    //PolyVox::CubicSurfaceExtractor<Material8> surfExtractor(&page->data, pageRegion, &surface);
    surfExtractor.execute();
    region->mapView.updateRegion(&surface);
    region->mapView.finalizeRegion();
    region->mapView.createPhysicsRegion(_phyMgr);
}

void
    VolumeMap::toVoxelCoords(Ogre::Vector3 &position)
{
    position.x = Ogre::Math::Floor(position.x + 0.5f); //Need to write a function for this
    position.y = Ogre::Math::Floor(position.y + 0.5f);
    position.z = Ogre::Math::Floor(position.z + 0.5f);
}

void
    VolumeMap::getBlockCenterWithRay(Ogre::Ray &rayTo, Ogre::Real searchDistance, Ogre::Vector3& blockCenter)
{
    if(_phyMgr->getCollisionPoint(blockCenter, rayTo, searchDistance))
    {
        _getCenterOfIntersectedPoint(blockCenter, rayTo.getDirection());
    }
    else
    {
        blockCenter = rayTo.getPoint(searchDistance);
        //But also hash this center into block unots;
        toVoxelCoords(blockCenter);
    }
}

void
    VolumeMap::addBlock(Ogre::Ray &rayTo, Ogre::Real searchDistance)
{
    
    Ogre::Vector3 intersectPoint;
    if(_phyMgr->getCollisionPoint(intersectPoint, rayTo, searchDistance))
    {
        _modifyVolume(intersectPoint, 1, rayTo.getDirection(), ADD_BLOCK);
    }
    else //No intersection. Just make searchDistance to be 2 cube units in ray direction so you add a cube in front of you.
    {
        //2.0f may not correspond to 2 cubes..
        _modifyVolume(rayTo.getPoint(2.0f), 1, rayTo.getDirection(), ADD_BLOCK);
    }
}

void
    VolumeMap::removeBlock(Ogre::Ray &rayTo, Ogre::Real searchDistance)
{
    
    Ogre::Vector3 intersectPoint;
    cout << "VolumeMap::removeBlock" << endl;
    if(_phyMgr->getCollisionPoint(intersectPoint, rayTo, searchDistance))
    {
        _modifyVolume(intersectPoint, 0, rayTo.getDirection(), REMOVE_BLOCK);
    }
    else
    {
    }
}