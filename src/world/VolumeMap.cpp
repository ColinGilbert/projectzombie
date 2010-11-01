/*
 * VolumeMap.cpp
 *
 *  Created on: Sep 21, 2010
 *      Author: beyzend
 */
#include <OgreMemoryAllocatorConfig.h>
#include <memory>
#include <iostream>
#include <OgreException.h>
using std::cout;
using std::endl;
#include "world/VolumeMap.h"
#include "PolyVoxImpl/Utility.h"
#include "world/PerlinNoiseMapGen.h"
#include "world/WorldDefs.h"
using ZGame::World::VolumeMap;
using PolyVox::MaterialDensityPair44;
using PolyVox::Volume;
using PolyVox::Vector3DFloat;
using PolyVox::Vector3DUint16;
using PolyVox::Vector3DInt16;
using std::shared_ptr;
using PolyVox::SurfaceExtractor;
using PolyVox::CubicSurfaceExtractor;
using PolyVox::SurfaceMesh;
using namespace ZGame::World;
using namespace Ogre;



const Ogre::uint16 VolumeMap::WORKQUEUE_LOAD_REQUEST = 1;

void
createSphereInVolume(Volume<MaterialDensityPair44>& volData, float fRadius, uint8_t uValue)
{
  //This vector hold the position of the center of the volume
  Vector3DFloat v3dVolCenter(volData.getWidth() / 2, volData.getHeight() / 2, volData.getDepth() / 2);

  //This three-level for loop iterates over every voxel in the volume
  for (int z = 0; z < volData.getWidth(); z++)
    {
      for (int y = 0; y < volData.getHeight(); y++)
        {
          for (int x = 0; x < volData.getDepth(); x++)
            {
              //Store our current position as a vector...
              Vector3DFloat v3dCurrentPos(x, y, z);
              //And compute how far the current position is from the center of the volume
              float fDistToCenter = (v3dCurrentPos - v3dVolCenter).length();

              //If the current voxel is less than 'radius' units from the center
              //then we make it solid, otherwise we make it empty space.
              if (fDistToCenter <= fRadius)
                {
                  volData.setVoxelAt(x, y, z, MaterialDensityPair44(uValue, uValue > 0 ? MaterialDensityPair44::getMaxDensity()
                      : MaterialDensityPair44::getMinDensity()));
                }
            }
        }
    }
}

void
createCubeInVolume(Volume<MaterialDensityPair44>& volData, Vector3DUint16 lowerCorner, Vector3DUint16 upperCorner, uint8_t uValue)
{
  //This three-level for loop iterates over every voxel between the specified corners
  for (int z = lowerCorner.getZ(); z <= upperCorner.getZ(); z++)
    {
      for (int y = lowerCorner.getY(); y <= upperCorner.getY(); y++)
        {
          for (int x = lowerCorner.getX(); x <= upperCorner.getX(); x++)
            {
              volData.setVoxelAt(x, y, z, MaterialDensityPair44(uValue, uValue > 0 ? MaterialDensityPair44::getMaxDensity()
                  : MaterialDensityPair44::getMinDensity()));
            }
        }
    }
}

//const int SW = 320.0;
//const int SH = 256;
//const int SD = 320.0;
VolumeMap::VolumeMap() :
  _regionSideLen(WORLD_BLOCK_WIDTH), _numOfPages(30 * 30), _regionsWidth(WORLD_WIDTH), _regionsHeight(WORLD_HEIGHT), _regionsDepth(WORLD_DEPTH)
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
  using Ogre::map;
  using Ogre::list;
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
  for (liter = _freeList.begin(); liter != _freeList.end(); ++liter)
    {
      delete *liter;
    }
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
  PerlinNoiseMapGen gen;
  LoadRequest lreq = any_cast<LoadRequest> (req->getData());

  VolumePage* page = lreq.page;
  WorkQueue::Response* response = 0;
  int x, y;
  _unpackIndex(page->id, &x, &y);
  page->gen.generate(&page->data, x, y);
  //_mapGen.generate(&page->data, x, y);
  //MUST MAKE SURE YOU allocate mesh before requesting this request to the WorkerQueue.
  PolyVox::CubicSurfaceExtractor<PolyVox::MaterialDensityPair44> surfExtractor(&page->data, page->data.getEnclosingRegion(), lreq.surface);
  //PolyVox::SurfaceExtractor<PolyVox::MaterialDensityPair44> surfExtractor(&page->data, page->data.getEnclosingRegion(), page->surface);
  surfExtractor.execute();
  response = OGRE_NEW_T_SIMD (WorkQueue::Response(req, true, Any()), Ogre::MEMCATEGORY_GENERAL);
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
      int x, z;
      _unpackIndex(page->id, &x, &z);

      //cout << "Page loaded response: " << x << ", " << z << endl;
      Ogre::Vector3 pageWorldPos((float) (x) * _regionSideLen, 0.0, (float) (z) * _regionSideLen);
      //cout << "PageWorldPos: " << pageWorldPos << endl;
      //pageWorldPos = pageWorldPos; //- _origin*2;
      page->mapView.updateOrigin(pageWorldPos);
      //if (create)
      page->mapView.createRegion(page->isEmpty(), lreq.surface);
      OGRE_DELETE_T_SIMD (lreq.surface, SurfaceMesh, Ogre::MEMCATEGORY_GENERAL);
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
  VolumeMap::VolumePage* ret = _freeList.front();
  //cout << "_getFree: _freeList size: " << _freeList.size() << endl;
  _freeList.pop_front();
  //cout << "_getFree: _freeList size: " << _freeList.size() << endl;
  return ret;
}
void
VolumeMap::_initLists()
{
  for (size_t i = 0; i < _numOfPages; ++i)
    {
      VolumePage* page = new VolumePage(_regionSideLen, _regionsHeight);
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
  //_defineRegions();
}

void
VolumeMap::loadPage(Ogre::PageID pageID)
{

  int x, y;
  _unpackIndex(pageID, &x, &y);
  y = -y;
  pageID = _packIndex(x, y);
  //using std::map;
  //map<Ogre::PageID, VolumePage*>::iterator findMe = _pagesMap.find(pageID);
  PagesMap::iterator findMe = _pagesMap.find(pageID);
  if (findMe == _pagesMap.end())
    {
      VolumePage* page = _getFree();
      if (!page)
        return; //no more free page. For now just completely ignore.
      page->id = pageID;//_packIndex(x, y);
      LoadRequest req;
      req.origin = this;
      req.page = page;
      req.surface = OGRE_NEW_T_SIMD (PolyVox::SurfaceMesh<PolyVox::PositionMaterial>, Ogre::MEMCATEGORY_GENERAL);
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

  int x, y;
  _unpackIndex(pageID, &x, &y);
  y = -y;
  pageID = _packIndex(x, y);
  using std::map;
  //map<Ogre::PageID, VolumePage*>::iterator findMe = _pagesMap.find(pageID);
  PagesMap::iterator findMe = _pagesMap.find(pageID);
  if (findMe == _pagesMap.end())
    return; //If it's not in _pagesMap then we dont' care. Probably due to thread not catching up.
  //OGRE_EXCEPT( Ogre::Exception::ExceptionCodes::ERR_ITEM_NOT_FOUND, "unload a pageid that is not found in pagesMap", "VolumeMap::unloadPage");

  VolumePage* page = findMe->second;
  page->mapView.unloadRegion(page->isEmpty());

  _pagesMap.erase(findMe);
  _addToList(page);

}

void
VolumeMap::_loadPage(VolumeMap::VolumePage* page, bool create)
{
  int x, z;
  _unpackIndex(page->id, &x, &z);
  cout << "_loadPage: unpacked index: x,y: " << x << ", " << z << endl;
  page->setAllocated();
  _mapGen.generate(&page->data, x, z);
  //Extract the surface. We are creating surface mesh on stack. Maybe look into optimizing this for reuse instead.
  SurfaceMesh<PolyVox::PositionMaterial> mesh;
  //Vector3DInt16 regLowerCorner(0, 0, 0);
  //Vector3DInt16 regUpperCorner(page->data.getDepth(), page->data.getHeight(), page->data.getWidth());

  PolyVox::CubicSurfaceExtractor<PolyVox::MaterialDensityPair44> surfExtractor(&page->data, page->data.getEnclosingRegion(), &mesh);
  surfExtractor.execute();
  page->setEmpty(mesh.m_vecTriangleIndices.size() == 0);
  //Now for all those with actual surfaces extracted, create it in View.
  Ogre::Vector3 pageWorldPos((float) (x) * _regionSideLen, 0, (float) (z) * _regionSideLen);
  //cout << "PageWorldPos: " << pageWorldPos << endl;
  //pageWorldPos = pageWorldPos; //- _origin*2;
  page->mapView.updateOrigin(pageWorldPos);
  //if (create)
  page->mapView.createRegion(page->isEmpty(), &mesh);
  //else
  //page->mapView.updateRegion(page->isEmpty(), &mesh);
}

void
VolumeMap::_defineRegions()
{
  using namespace Ogre;
  using Ogre::int32;
  int32 startX, startZ, endX, endZ;
  //Figure out the Page x,y. X goes left to right, Z goes from negative to positive. Note: This way of doing this may be stupid but I have a headache right now.
  int halfRegions = _regionsWidth / 2 / _regionSideLen;
  startX = 0 - halfRegions;
  startZ = startX;
  endX = 0 + halfRegions;
  endZ = endX;
  int halfHeight = _regionsHeight / 2;
  //Loop through each page regions.
  for (int z = startZ; z <= endZ; ++z)
    {
      for (int x = startX; x <= endX; x++)
        {

          //For each region, define this region.
          VolumePage* page = _getFree();
          page->id = _packIndex(x, z);
          _loadPage(page, true);
          //Add this region to the pages map.
          _pagesMap[page->id] = page;
        }
    }

}

Ogre::uint32
VolumeMap::_packIndex(Ogre::int32 x, Ogre::int32 y)
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
VolumeMap::_unpackIndex(Ogre::PageID pageID, Ogre::int32 *x, Ogre::int32 *y)
{
  using Ogre::uint16;
  using Ogre::int16;
  uint16 y16 = static_cast<uint16> (pageID & 0xFFFFF);
  uint16 x16 = static_cast<uint16> ((pageID >> 16) & 0xFFFF);

  *x = static_cast<int16> (x16);
  *y = static_cast<int16> (y16);
}
