/**
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
**/
/**
*author: beyzend 
*email: llwijk@gmail.com
**/

#pragma once

#define PROFILE 1 //should move this to prerequisites later.

#include "ZPrerequisites.h"
#include "world/VolumeMapView.h" //We use it directly here instead of relying on the Observer pattern to update views.
#include "world/PerlinNoiseMapGen.h"
#include "world/TestMapGenerator.h"
#include "utilities/Timer.h"
/*
* VolumeMap.h
*
*
*
*  Created on: Sep 21, 2010
*      Author: beyzend
*/

namespace ZGame
{
    namespace World
    {
        class PhysicsManager;
        /**
        * This class defines a representation or a volume map. A volume map is a container for managing a collection of Volume Pages. Volume Pages are
        * defined as a mapping to Ogre's Paging System's PageIDs to this Volume Map's Volume Pages, containing PolyVox Volume data. In correspondance with
        * Ogre's paging system, we implement load and unloading of pages. Volume Page correspond to PolyVox::Volume. Each Volume Page contains multiple VolumeRegion, 
        *these page regions correspond to PolyVox::Region. Finally, there is a one to one mapping between Ogre's PageID and VolumeRegion. This implies that
        *a VolumePage may contain multiple PageIds. 
        *
        **/
        class VolumeMap : public Ogre::WorkQueue::RequestHandler, public Ogre::WorkQueue::ResponseHandler
        {
        public:
            VolumeMap(Ogre::SceneManager* scnMgr, size_t volSideLenInPages = 3, bool ASYNC = false);
            virtual
                ~VolumeMap();
            void
                load(PhysicsManager* phyMgr);
            void
                setOrigin(const Ogre::Vector3& origin)
            {
                _origin = origin;
            }

            const Ogre::Vector3&
                getOrigin() const
            {
                return _origin;
            }

            void
                onUpdate(const Ogre::FrameEvent &evt);
            
            void
                loadPage(Ogre::PageID pageID);

            void
                unloadPage(Ogre::PageID pageID);

            void
                addBlock(Ogre::Ray &rayTo, Ogre::Real searchDistance);
            void
                removeBlock(Ogre::Ray &rayTo, Ogre::Real searchDistance);
            void
                getBlockCenterWithRay(Ogre::Ray &rayTo, Ogre::Real searchDistance,
                Ogre::Vector3& blockCenter);
            /** \brief This method fills the selection AABB with data.**/
            void
                fillSelection(const Ogre::AxisAlignedBox &aabb, uint8_t data);


            //Ogre WorkQueue related overrides
            bool
                canHandleRequest(const Ogre::WorkQueue::Request* req, const Ogre::WorkQueue* srcQ);
            Ogre::WorkQueue::Response*
                handleRequest(const Ogre::WorkQueue::Request* req, const Ogre::WorkQueue* srcQ);
            bool
                canHandleResponse(const Ogre::WorkQueue::Response* res, const Ogre::WorkQueue* srcQ);
            void
                handleResponse(const Ogre::WorkQueue::Response* res, const Ogre::WorkQueue* srcQ);

            static const Ogre::uint16 WORKQUEUE_LOAD_REQUEST;

#if PROFILE
            void
                getProfileStats(std::ostream &out);
#endif

        protected:

            Ogre::uint16 _workQueueChannel;
            struct PageRegion
            {
                PageRegion(Ogre::SceneManager* scnMgr) : mapView(scnMgr), loading(false),
                deferredUnload(false) {}
                VolumeMapView mapView;
                bool loading;
                bool deferredUnload;
            };
            /**
            * This class defines a volume page. It has a correspondance to Ogre Paging's Paging system on PageID (uint32).
            *
            * \note We need to refactor this class to use templating on PolyVox::Volume and Material.
            */
            class VolumePage
            {
            public:

                VolumePage(size_t pageSize, size_t pageHeight,
                    size_t sharedBlockSize = 32, 
                    size_t uncompressedCacheSize = 4) :
                  data(pageSize, pageHeight, pageSize, 32)
                  {
                      data.setBorderValue(PolyVox::Material8(0));
                  }
                  virtual ~VolumePage()
                  {
                      REGION_MAP::iterator it;
                      for(it = _regionMap.begin(); it != _regionMap.end(); ++it)
                      {
                          OGRE_DELETE_T(it->second, PageRegion, Ogre::MEMCATEGORY_GENERAL);
                          //delete it->second;
                      }
                      //_regionMap.clear();
                  }
                  //keep time stat. here. Putting in LoadRequest doesn't work.
#if PROFILE
                  double generateTime;
                  double extractionTime;
#endif
                  Ogre::PageID id;
                  //PolyVox::UInt8Volume data;
                  PVolume data;
                  PageRegion*
                      createRegion(Ogre::PageID pageId, Ogre::SceneManager* scnMgr)
                  {
                      REGION_MAP::iterator findMe = _regionMap.find(pageId);
                      if(findMe == _regionMap.end())
                      {
                          PageRegion* region = OGRE_NEW_T(PageRegion, Ogre::MEMCATEGORY_GENERAL)(scnMgr);
                          region->loading = true;
                          _regionMap[pageId] = region;
                          return region;
                      }
                      return 0;
                      /*
                      * Note: THe below exception can happen. I'm not yet sure why or how it
                      *happens. However, we are going to ignore this for now because of the
                      *fact that PageID is unique. If a PageID is in a region already, 
                      *then it exists, we can just safely ignore that we are asked to 
                      *load this existing PageID again. My guess on why this happens is
                      *either it's the fault of Ogre's paging system upstream. Or there is
                      *still some logic bug I haven't figured out yet.
                      */
                      /*
                      OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE,
                          "Tring to create an exisiting PageRegion in a VolumePage!",
                          "PageRegion::createRegion");
                      */
                      //if(findMe->second->loading)
                      //return 0; //Return 0 if this is found. We just don't reload it again.
                  }
                  /** \note We are using hash map right now. In the future we can 
                  switch this to arrays if needed.**/
                  PageRegion*
                      getRegion(Ogre::PageID pageId)
                  {
                      REGION_MAP::iterator findMe = _regionMap.find(pageId);
                      if(findMe != _regionMap.end())
                      {
                          return findMe->second;
                      }
                      return 0;
                  }
                  void
                      removeRegion(Ogre::PageID pageId)
                  {
                      REGION_MAP::iterator findMe = _regionMap.find(pageId);
                      if(findMe != _regionMap.end())
                      {
                          OGRE_DELETE_T(findMe->second, PageRegion, Ogre::MEMCATEGORY_GENERAL);
                          _regionMap.erase(findMe);
                      }
                  }
                  bool
                      isEmpty()
                  {
                      return (_regionMap.size() == 0);
                  }
                Ogre::Vector3 worldOrigin;
                  
            private:
                typedef std::unordered_map<Ogre::PageID, PageRegion*, hash<Ogre::PageID>, std::equal_to<Ogre::PageID>,
                    Ogre::STLAllocator<std::pair<const Ogre::PageID, PageRegion*>, Ogre::GeneralAllocPolicy> > REGION_MAP;
                //typedef Ogre::map<Ogre::PageID, PageRegion*>::type REGION_MAP;
                REGION_MAP _regionMap;
            };

            struct LoadRequest
            {
                VolumePage* page;
                VolumeMap* origin;
                Ogre::PageID ogreId;
                PolyVox::SurfaceMesh<PolyVox::PositionMaterial>* tempSurface;
                PolyVox::SurfaceMesh<PolyVox::PositionMaterial>* surface;
                
                friend std::ostream&
                    operator<<(std::ostream& o, const LoadRequest& r)
                {
                    return o;
                }
            };

         private:
            
            Ogre::SceneManager* _scnMgr;
            Ogre::uint32 _volSideLenInPages;
            Ogre::uint32 _volSizeInBlocks;
            Ogre::uint32 _volHeight;
            Ogre::uint32 _volWidthInRegions;
            Ogre::uint32 _volHeightInRegions;
            Ogre::uint32 _volDepthInRegions;
            size_t SHARED_BLOCK_SIZE;
            size_t UNCOMPRESSED_CACHE_SIZE;
            
            Ogre::Vector3 _origin;
            bool _FORCE_SYNC;
            typedef std::unordered_map<Ogre::PageID, VolumePage*, hash<Ogre::PageID>, std::equal_to<Ogre::PageID>,
                Ogre::STLAllocator<std::pair<const Ogre::PageID, VolumePage*>, Ogre::GeneralAllocPolicy> > PagesMap;
            //typedef Ogre::map<Ogre::PageID, VolumePage*>::type PagesMap;
            
            PagesMap _pagesMap;
            PhysicsManager* _phyMgr;
            typedef Ogre::deque<std::pair<VolumePage*, std::pair<Ogre::PageID, PageRegion* >>>::type LoadQueue;
            LoadQueue _loadQueue;

        private:

            struct PageXY
            {
                long x;
                long y;
            };

            typedef PageXY VolXY;

            struct PageFillCmd
            {
                Ogre::Vector3 regionStart;
                Ogre::Vector3 regionEnd;
                uint8_t data;
            };

            struct PageDirtyCmd
            {
                Ogre::Vector2 localStart;
                Ogre::Vector2 localEnd;
                VolumePage* page;
                PageRegion* region;
            };

            typedef Ogre::list<PageFillCmd>::type PageRegionList;
            PageRegionList _fillVolumeCmd;

            typedef Ogre::list<PageDirtyCmd>::type DirtyRegionList;

            DirtyRegionList _dirtyRegionsList;


            enum VOLUME_MODIFY_MODE
            {
                ADD_BLOCK=0, REMOVE_BLOCK
            };
            void
                _initVolumes();
            void
                _freeAll();
            Ogre::uint32
                _packIndex(long x, long y);
            void
                _unpackIndex(Ogre::PageID pageID, long *x, long *y);
            
            void
                _modifyVolume(Ogre::Vector3 point, size_t blockType, Ogre::Vector3 rayDir,
                VOLUME_MODIFY_MODE mode);
            void
                _removeBlockFromVolume(const Ogre::Vector3 &point);

            void
                _getCenterOfIntersectedPoint(Ogre::Vector3& point, Ogre::Vector3 rayDir);

            VolumePage*
                _allocateVolume(Ogre::PageID pageId, size_t size, size_t height);
            Ogre::PageID 
                _pageIdToVolumeId(Ogre::PageID pageId, size_t volSideLen);
            Ogre::Vector2 
                _transformToVolumeLocal(Ogre::Vector2 volumeOrigin, Ogre::Vector2 local,
                size_t volSideLenInBlocks);
            void
                _updatePageRegion(long pageX, long pageZ,
                PageRegion* region, VolumePage* page, Ogre::Vector2 volumeOrigin);
            void _unloadPageRegion(VolumePage* page, Ogre::PageID regionId);
            void
                _processLoadQueue();

            void
                _processFillCommands();
            void
                _processDirtyRegions();
            void
                _executeFillCommand(const PageFillCmd& cmd);
            void
                _fillSelection(Ogre::Real totalZLenInVoxels, Ogre::Real totalXLenInVoxels, 
                Ogre::Real totalYLenInVoxels,
                const Ogre::Vector3 &leftCorner, uint8_t data);

            void
                _addFillRegionCommand(int startX, int startY, int startZ, 
                int endX, int endY, int endZ, 
                uint8_t data);

            void
                _cubeCoordsToPageXY(long &x, long &z, const Ogre::Vector3 &cubeCenter);

            void
                _getPage(Ogre::Vector3 cubeCenter, long &pageX, long &pageZ, Ogre::PageID &pageID,
                long &volX, long &volZ, Ogre::PageID &volID, 
                VolumePage** volPage, PageRegion** region);

            Ogre::Vector2
                _pageCoordToWorldCoord(long pageX, long pageZ);
#if PROFILE
            //Used for stats tracking
            double _totalVolInKB; //total volume processed in kilobytes
            double _totalVolInKBDeallocated;
            long _totalNumOfChunks; //total number of chunks processed.
            double _avgLoadTime; //average of loading time. Running average.
            double _avgGenerationTime;
            double _avgExtractionTime;
            double _avgViewTime; 
            double _avgCompression; //running average of compression ratio.
            CPerfCounter _perfCounter;
#endif
        };
    }
}
