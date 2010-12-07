#pragma warning( disable : 4503)
#pragma once

#include <unordered_map>
#include <Ogre.h>
#include <MaterialDensityPair.h>

#include <SurfaceMesh.h>
#include <Volume.h>
#include "world/VolumeMapView.h" //We use it directly here instead of relying on the Observer pattern to update views.
#include <OgreGrid2DPageStrategy.h>
#include <OgrePagedWorldSection.h>
#include <OgrePagedWorld.h>
#include <OgrePageManager.h>
#include "world/PerlinNoiseMapGen.h"
#include "world/TestMapGenerator.h"
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
        * defined as a mapping to Ogre's Paging System's PageIDs to this Volume Map's Volume Pages, which contains PolyVox Volume data. In correspondance with
        * Ogre's paging system, we implement load and unloading of pages. We currently keep a free list of Volume Pages that we use to get a free Volume Page. After
        * a free page is gotten, we then insert it into map structure. This is to facilitate searching for this page based on PageID when Ogre's Paging System
        * asks us to unload a page. That is, we remove this page from the Map and add it to the free list, to be reused. This current system is a first
        * implementation (we did it because it seemed easier.)
        *
        * We directly map our Volume Page to PolyVox's Volume. This may be inefficient (because our pages are small, and we need many pages to represent a
        * large level.) An alternative is to push the paging system to PolyVox volume where we allocate a bigger sized Volume and directly manipulate the volume's
        * data to provide paging. But this means we have to implement our own paging strategy (in Ogre this is called PagingStrategy) in order to figure out
        * when to load/unload PolyVox volumes (of a certain size). So for now this is what we do for we hook direclty into Ogre's paging system. In the future
        * we can optimize.
        */
        class VolumeMap : public Ogre::WorkQueue::RequestHandler, public Ogre::WorkQueue::ResponseHandler
        {
        public:
            VolumeMap(size_t volSideLenInPages = 3, bool ASYNC = false);
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
                loadPage(Ogre::PageID pageID);

            void
                unloadPage(Ogre::PageID pageID);

            void
                addBlock(Ogre::Ray &rayTo, Ogre::Real searchDistance);
            void
                removeBlock(Ogre::Ray &rayTo, Ogre::Real searchDistance);


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

        protected:

            Ogre::uint16 _workQueueChannel;
            struct PageRegion
            {
                VolumeMapView mapView;
            };
            /**
            * This class defines a volume page. It has a correspondance to Ogre Paging's Paging system on PageID (uint32).
            */
            class VolumePage
            {
            public:

                VolumePage(size_t pageSize, size_t pageHeight) :
                  data(pageSize, pageHeight, pageSize, 32), _regionCount(0)
                  {
                      data.setBorderValue(0);
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
                  Ogre::PageID id;
                  PolyVox::UInt8Volume data;
                  PageRegion*
                      createRegion(Ogre::PageID pageId)
                  {
                      REGION_MAP::iterator findMe = _regionMap.find(pageId);
                      _regionCount++;
                      if(findMe == _regionMap.end())
                      {
                          PageRegion* region = OGRE_NEW_T(PageRegion, Ogre::MEMCATEGORY_GENERAL);
                          _regionMap[pageId] = region;
                          return region;
                      }
                      
                      return 0;
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
                      removeRegion()
                  {
                      _regionCount--;
                  }
                  bool
                      isEmpty()
                  {
                      assert(_regionCount > -1 && "Volume Region count is less than 0");
                      return _regionCount == 0;
                  }
                Ogre::Vector3 worldOrigin;
                  
            private:
                typedef std::unordered_map<Ogre::PageID, PageRegion*, hash<Ogre::PageID>, std::equal_to<Ogre::PageID>,
                    Ogre::STLAllocator<std::pair<const Ogre::PageID, PageRegion*>, Ogre::GeneralAllocPolicy> > REGION_MAP;
                //typedef Ogre::map<Ogre::PageID, PageRegion*>::type REGION_MAP;
                REGION_MAP _regionMap;
                size_t _regionCount;
                bool _allocated;
                bool _empty;
            };

            struct LoadRequest
            {
                VolumePage* page;
                VolumeMap* origin;
                Ogre::PageID ogreId;
                PolyVox::SurfaceMesh<PolyVox::PositionMaterial>* surface;
                friend std::ostream&
                    operator<<(std::ostream& o, const LoadRequest& r)
                {
                    return o;
                }
            };



        private:
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

        private:
            
            //PolyVox::Volume<PolyVox::MaterialDensityPair44> _data;
            VolumeMapView _view;
            Ogre::uint32 _volSideLenInPages;
            Ogre::uint32 _volSizeInBlocks;
            Ogre::uint32 _volHeight;
            Ogre::uint32 _volWidthInRegions;
            Ogre::uint32 _volHeightInRegions;
            Ogre::uint32 _volDepthInRegions;
            
            Ogre::Vector3 _origin;
            bool _FORCE_SYNC;
            typedef std::unordered_map<Ogre::PageID, VolumePage*, hash<Ogre::PageID>, std::equal_to<Ogre::PageID>,
                Ogre::STLAllocator<std::pair<const Ogre::PageID, VolumePage*>, Ogre::GeneralAllocPolicy> > PagesMap;
            //typedef Ogre::map<Ogre::PageID, VolumePage*>::type PagesMap;
            PagesMap _pagesMap;
            PhysicsManager* _phyMgr;

        };
    }
}
