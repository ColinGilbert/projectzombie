#pragma warning(disable : 4503)
/*
* VolumeMap.cpp
*
*  Created on: Sep 21, 2010
*      Author: beyzend
*/
//#include <OgreMemoryAllocatorConfig.h>
#include <memory>
#include <iostream>
#include <OgreException.h>
#include <CubicSurfaceExtractor.h>
using std::cout;
using std::endl;
#include "world/VolumeMap.h"
#include "PolyVoxImpl/Utility.h"
#include "world/PerlinNoiseMapGen.h"
#include "world/WorldDefs.h"

#include "world/ZCubicSurfaceExtractor.h"
using ZGame::World::VolumeMap;
using PolyVox::ZCubicSurfaceExtractor;
using PolyVox::MaterialDensityPair44;
using PolyVox::Volume;
using PolyVox::Vector3DFloat;
using PolyVox::Vector3DUint16;
using PolyVox::Vector3DInt16;
using std::shared_ptr;
//using PolyVox::SurfaceExtractor;
//using PolyVox::CubicSurfaceExtractorWithNormals;
using PolyVox::CubicSurfaceExtractor;
using PolyVox::SurfaceMesh;
using namespace ZGame::World;
using namespace Ogre;



const Ogre::uint16 VolumeMap::WORKQUEUE_LOAD_REQUEST = 1;


//const int SW = 320.0;
//const int SH = 256;
//const int SD = 320.0;
VolumeMap::VolumeMap() :
//_regionSideLen(WORLD_BLOCK_WIDTH), _numOfPages(50 * 50), _regionsWidth(WORLD_WIDTH), _regionsHeight(WORLD_HEIGHT), _regionsDepth(WORLD_DEPTH)
_regionSideLen(WORLD_BLOCK_WIDTH), _numOfPages(50 * 50), _regionsWidth(WORLD_WIDTH), _regionsHeight(WORLD_HEIGHT), _regionsDepth(WORLD_DEPTH)
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
    //using Ogre::map;
    //using Ogre::list;
    //Iterate through pagesMap and free that.

    //map<Ogre::PageID, VolumePage*>::iterator iter = _pagesMap.begin();
    PagesMap::iterator iter = _pagesMap.begin();
    for (iter = _pagesMap.begin(); iter != _pagesMap.end(); ++iter)
    {
        delete iter->second;
    }
    _pagesMap.clear();
    //Iterate through free list and free that.
    //list<VolumePage*>::iterator liter;
    FreeList::iterator liter;
    /*
    for (liter = _freeList.begin(); liter != _freeList.end(); ++liter)
    {
    delete *liter;
    }*/
    _freeList.clear();
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

WorkQueue::Response*
    VolumeMap::handleRequest(const WorkQueue::Request* req, const WorkQueue* srcQ)
{
    LoadRequest lreq = any_cast<LoadRequest> (req->getData());
    VolumePage* page = lreq.page;
    WorkQueue::Response* response = 0;
    long x, y;
    _unpackIndex(page->id, &x, &y);
    page->gen->generate(&page->data, x, -y);
    //page->data.tidyUpMemory();
    //_mapGen.generate(&page->data, x, y);
    //MUST MAKE SURE YOU allocate mesh before requesting this request to the WorkerQueue.
    //PolyVox::CubicSurfaceExtractor<PolyVox::Material8> surfExtractor(&page->data, page->data.getEnclosingRegion(), lreq.surface);
    ZCubicSurfaceExtractor<uint8_t> surfExtractor(&page->data, page->data.getEnclosingRegion(), lreq.surface);
    //PolyVox::SurfaceExtractor<PolyVox::MaterialDensityPair44> surfExtractor(&page->data, page->data.getEnclosingRegion(), page->surface);
    surfExtractor.execute();
    //response = OGRE_NEW_T_SIMD (WorkQueue::Response(req, true, Any()), Ogre::MEMCATEGORY_GENERAL);
     Ogre::Vector3 pageWorldPos((float) (x) * _regionSideLen, 0.0, (float) (-y) * _regionSideLen);
        //page->mapView.updateOrigin(pageWorldPos);
        page->mapView.createRegion(pageWorldPos, lreq.surface);
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
        //cout << "Response success!" << endl;
        VolumePage* page = lreq.page;
        long x, z;
        _unpackIndex(page->id, &x, &z);
       
       
        page->mapView.finalizeRegion();
        //OGRE_DELETE_T_SIMD (lreq.surface, SurfaceMesh, Ogre::MEMCATEGORY_GENERAL);
        delete lreq.surface;
        _pagesMap[page->id] = page;
        //page->data.tidyUpMemory();

    }
    else
    {
        Ogre::LogManager::getSingleton().logMessage(Ogre::LML_CRITICAL, "FAILED TO HANDLE RESPONSE in VOLUME_MAP");
    }

    //OGRE_DELETE_T(res, Response, Ogre::MEMCATEGORY_GENERAL);

}

void
    VolumeMap::_addToList(VolumePage* page)
{
    _freeList.push_back(page);
}

VolumeMap::VolumePage*
    VolumeMap::_getFree()
{
    //cout << "_getFree: _freeList size: " << _freeList.size() << endl;
    VolumeMap::VolumePage* ret = _freeList.front();
    _freeList.pop_front();

    return ret;
    //cout << "_getFree: _freeList size: " << _freeList.size() << endl;
    //return ret;
}
void
    VolumeMap::_initLists()
{
    for (size_t i = 0; i < _numOfPages; ++i)
    {
        VolumePage* page = new VolumePage(_regionSideLen, _regionsHeight,
            new PerlinNoiseMapGen());
        //new TestMapGenerator());
        _addToList(page);
    }
}

void
    VolumeMap::load()
{
    WorkQueue* wq = Root::getSingleton().getWorkQueue();
    _workQueueChannel = wq->getChannel("PROJECT_ZOMBIE/VolumeMap");
    wq->addRequestHandler(_workQueueChannel, this);
    wq->addResponseHandler(_workQueueChannel, this);
    _initLists();
}

void
    VolumeMap::loadPage(Ogre::PageID pageID)
{

    //long x, y;
    //_unpackIndex(pageID, &x, &y);
    //y = -y;
    //pageID = _packIndex(x, y);
    //using std::map;
    //map<Ogre::PageID, VolumePage*>::iterator findMe = _pagesMap.find(pageID);
    //cout << "Loading PageID: " << pageID << endl;
    PagesMap::iterator findMe = _pagesMap.find(pageID);
    if (findMe == _pagesMap.end())
    {
        VolumePage* page = _getFree();
        if (!page)
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, "No more free volume pages in the buffer!",
                "VolumeMap::loadPage");
            //return; //no more free page. For now just completely ignore.
        }
        page->id = pageID;//_packIndex(x, y);
        LoadRequest req;
        req.origin = this;
        req.page = page;
        //req.surface = OGRE_NEW_T_SIMD (PolyVox::SurfaceMesh<PolyVox::PositionMaterial>, Ogre::MEMCATEGORY_GENERAL);
        req.surface = new PolyVox::SurfaceMesh<PolyVox::PositionMaterial>;
        Root::getSingleton().getWorkQueue()->addRequest(_workQueueChannel, WORKQUEUE_LOAD_REQUEST, Any(req), 0, false);

        //_loadPage(page, true);
        //_pagesMap[page->id] = page;
    }
    else
    {
        cout << "In load page and page was already loaded!" << endl;
    }
}

void
    VolumeMap::unloadPage(Ogre::PageID pageID)
{
    //cout << "Unloading PageID: " << pageID << endl;
    long x, y;
    _unpackIndex(pageID, &x, &y);
    //y = -y;
    //pageID = _packIndex(x, y);
    using std::map;
    //map<Ogre::PageID, VolumePage*>::iterator findMe = _pagesMap.find(pageID);
    PagesMap::iterator findMe = _pagesMap.find(pageID);
    if (findMe == _pagesMap.end())
    {
        //cout << "!!unloading a page that was not found!!" << endl;
        return;
    }
    //If it's not in _pagesMap then we dont' care. Probably due to thread not catching up.
    //OGRE_EXCEPT( Ogre::Exception::ExceptionCodes::ERR_ITEM_NOT_FOUND, "unload a pageid that is not found in pagesMap", "VolumeMap::unloadPage");

    VolumePage* page = findMe->second;
    page->mapView.unloadRegion(page->isEmpty());

    _pagesMap.erase(findMe);
    _addToList(page);

}

Ogre::uint32
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

void
    VolumeMap::_unpackIndex(Ogre::PageID pageID, long *x, long *y)
{
    using Ogre::uint16;
    using Ogre::int16;
    uint16 y16 = static_cast<uint16> (pageID & 0xFFFFF);
    uint16 x16 = static_cast<uint16> ((pageID >> 16) & 0xFFFF);

    *x = static_cast<int16> (x16);
    *y = static_cast<int16> (y16);
}
