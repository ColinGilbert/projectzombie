/*
 * VolumeMapPaging.h
 *
 *  Created on: Sep 30, 2010
 *      Author: beyzend
 */

#pragma once
//#include <Terrain/OgreTerrainPrerequisites.h>
#include <OgrePageManager.h>
#include <OgrePagedWorldSection.h>
#include <OgrePagedWorld.h>
#include <OgrePageManager.h>
//#include "world/VolumePagedWorldSection.h"

using Ogre::int32;

namespace Ogre
{
  class SectionFactory : public Ogre::PagedWorldSectionFactory
  {
  public:
    static const Ogre::String FACTORY_NAME;
    const Ogre::String&
    getName() const;
    Ogre::PagedWorldSection*
    createInstance(const Ogre::String& name, Ogre::PagedWorld* parent, Ogre::SceneManager* scnMgr);
    void
    destroyInstance(Ogre::PagedWorldSection*);
  };
}

namespace ZGame
{
  namespace World
  {
    class VolumeMap;
    class VolumePagedWorldSection;
    class VolumeMapPaging
    {
    public:
      VolumeMapPaging(Ogre::PageManager* pageMgr);
      virtual
      ~VolumeMapPaging();

      VolumePagedWorldSection*
      createWorldSection(Ogre::PagedWorld* world, VolumeMap* volumeMap, Ogre::Real loadRadius, Ogre::Real holdRadius, Ogre::int32 minX, Ogre::int32 minY, Ogre::int32 maxX,
          Ogre::int32 maxY, Ogre::SceneManager* scnMgr, const Ogre::String& sectionName = Ogre::StringUtil::BLANK);

    protected:
      Ogre::SectionFactory _sectionFactory;

    private:
    private:
      Ogre::PageManager* _pageMgr;

    };
  }
}
