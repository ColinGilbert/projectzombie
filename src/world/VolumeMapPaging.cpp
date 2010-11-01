/*
 * VolumeMapPaging.cpp
 *
 *  Created on: Sep 30, 2010
 *      Author: beyzend
 */

#include "world/VolumeMapPaging.h"
#include "world/VolumePagedWorldSection.h"
#include "world/VolumeMap.h"

using ZGame::World::VolumeMapPaging;
using ZGame::World::VolumeMap;
using namespace Ogre;
using Ogre::int32;
VolumeMapPaging::VolumeMapPaging(PageManager* pageMgr) :
  _pageMgr(pageMgr)
{
  _pageMgr->addWorldSectionFactory(&_sectionFactory);
}

VolumeMapPaging::~VolumeMapPaging()
{
  _pageMgr->removeWorldSectionFactory(&_sectionFactory);
}

using ZGame::World::VolumePagedWorldSection;

VolumePagedWorldSection*
VolumeMapPaging::createWorldSection(PagedWorld* world, VolumeMap* volumeMap, Real loadRadius, Real holdRadius, Ogre::int32 minX, Ogre::int32 minY, Ogre::int32 maxX, Ogre::int32 maxY,
    SceneManager* scnMgr, const String& sectionName)
{
  VolumePagedWorldSection* ret = static_cast<VolumePagedWorldSection*> (world->createSection(scnMgr, SectionFactory::FACTORY_NAME, sectionName));

  ret->init(volumeMap);
  ret->setLoadRadius(loadRadius);
  ret->setHoldRadius(holdRadius);
  ret->setPageRange(minX, minY, maxX, maxY);

  return ret;
}

const String Ogre::SectionFactory::FACTORY_NAME("VolumeMap");

const String&
Ogre::SectionFactory::getName() const
{
  return FACTORY_NAME;
}

PagedWorldSection*
Ogre::SectionFactory::createInstance(const String& name, PagedWorld* parent, SceneManager* scnMgr)
{
  return OGRE_NEW VolumePagedWorldSection(name, parent, scnMgr);
}

void
Ogre::SectionFactory::destroyInstance(PagedWorldSection* s)
{
  OGRE_DELETE s;
}
