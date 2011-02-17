/*
 * VolumePagedWorldSection.h
 *
 *  Created on: Sep 30, 2010
 *      Author: beyzend
 */

#pragma once
#include <Ogre.h>
#include <OgreGrid2DPageStrategy.h>
#include <OgrePagedWorldSection.h>
#include <OgrePagedWorld.h>
#include <OgrePageManager.h>

namespace ZGame
{
  namespace World
  {
    class VolumeMap;
    using Ogre::uint16;
    using Ogre::int16;
    using Ogre::int32;
    class VolumePagedWorldSection : public Ogre::PagedWorldSection
    {
    public:
      VolumePagedWorldSection(const Ogre::String& name, Ogre::PagedWorld* parent, Ogre::SceneManager* scnMgr);
      virtual
      ~VolumePagedWorldSection();

      virtual void
      init(VolumeMap* volumeMap);

      virtual VolumeMap*
      getVolumeMap()
      {
        return _volumeMap;
      }

      virtual void
      setLoadRadius(Ogre::Real sz);
      virtual Ogre::Real
      getLoadRadius() const;
      virtual void
      setHoldRadius(Ogre::Real sz);
      virtual Ogre::Real
      getHoldRadius();
      /// Set the index range of all Pages (values outside this will be ignored)
      virtual void
      setPageRange(int32 minX, int32 minY, int32 maxX, int32 maxY);
      /// Set the index range of all Pages (values outside this will be ignored)
      virtual void
      setPageRangeMinX(int32 minX);
      /// Set the index range of all Pages (values outside this will be ignored)
      virtual void
      setPageRangeMinY(int32 minY);
      /// Set the index range of all Pages (values outside this will be ignored)
      virtual void
      setPageRangeMaxX(int32 maxX);
      /// Set the index range of all Pages (values outside this will be ignored)
      virtual void
      setPageRangeMaxY(int32 maxY);
      /// get the index range of all Pages (values outside this will be ignored)
      virtual int32
      getPageRangeMinX() const;
      /// get the index range of all Pages (values outside this will be ignored)
      virtual int32
      getPageRangeMinY() const;
      /// get the index range of all Pages (values outside this will be ignored)
      virtual int32
      getPageRangeMaxX() const;
      /// get the index range of all Pages (values outside this will be ignored)
      virtual int32
      getPageRangeMaxY() const;

      virtual Ogre::Grid2DPageStrategy*
      getGridStrategy() const;
      virtual Ogre::Grid2DPageStrategyData*
      getGridStrategyData() const;

      void
      loadPage(Ogre::PageID pageID, bool forceSynchronous = false);
      void
      unloadPage(Ogre::PageID pageID, bool forceSynchronous = false);

    protected:

      /// Overridden from PagedWorldSection
      void
      loadSubtypeData(Ogre::StreamSerialiser& ser);
      void
      saveSubtypeData(Ogre::StreamSerialiser& ser);

      virtual void
      syncSettings();

    private:
      VolumeMap* _volumeMap;
      void
      _unpackIndex(Ogre::PageID pageID, long* x, long* y);

    };
  }
}
