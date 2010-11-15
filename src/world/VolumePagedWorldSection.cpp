/*
 * VolumePagedWorldSection.cpp
 *
 *  Created on: Sep 30, 2010
 *      Author: beyzend
 */
#include <sstream>
#include "world/VolumePagedWorldSection.h"
#include "world/VolumeMap.h"
using namespace Ogre;
using ZGame::World::VolumePagedWorldSection;
using ZGame::World::VolumeMap;
using Ogre::uint16;
using Ogre::int32;
using Ogre::int16;
VolumePagedWorldSection::VolumePagedWorldSection(const Ogre::String& name, Ogre::PagedWorld* parent, Ogre::SceneManager *scnMgr) :
  PagedWorldSection(name, parent, scnMgr), _volumeMap(0)
{
  //We are currently using 2D grid strategy. This strategy should be available.
  setStrategy(parent->getManager()->getStrategy("Grid2D"));
}

VolumePagedWorldSection::~VolumePagedWorldSection()
{
  OGRE_DELETE _volumeMap;
}

void
VolumePagedWorldSection::init(VolumeMap* volumeMap)
{
  if (_volumeMap == volumeMap)
    return;
  if (_volumeMap)
    OGRE_DELETE _volumeMap;

  _volumeMap = volumeMap;
  syncSettings();
}

void
VolumePagedWorldSection::syncSettings()
{
  using namespace Ogre;
  Grid2DPageStrategyData* gridData = getGridStrategyData();
  gridData->setMode(G2D_X_Z); //Assume X,Z alignment for now.
  gridData->setOrigin(Ogre::Vector3(0, 0, 0));
  gridData->setCellSize(_volumeMap->getRegionSideLength());

}

void
VolumePagedWorldSection::setLoadRadius(Real sz)
{
  getGridStrategyData()->setLoadRadius(sz);
}

Real
VolumePagedWorldSection::getLoadRadius() const
{
  return getGridStrategyData()->getLoadRadius();
}

void
VolumePagedWorldSection::setHoldRadius(Real sz)
{
  getGridStrategyData()->setHoldRadius(sz);
}

Real
VolumePagedWorldSection::getHoldRadius()
{
  return getGridStrategyData()->getHoldRadius();
}

using Ogre::int32;
void
VolumePagedWorldSection::setPageRange(Ogre::int32 minX, Ogre::int32 minY, Ogre::int32 maxX, Ogre::int32 maxY)
{
  getGridStrategyData()->setCellRange(minX, minY, maxX, maxY);
}

void
VolumePagedWorldSection::setPageRangeMinX(Ogre::int32 minX)
{
  getGridStrategyData()->setCellRangeMinX(minX);
}

void
VolumePagedWorldSection::setPageRangeMinY(Ogre::int32 minY)
{
  getGridStrategyData()->setCellRangeMinY(minY);
}

void
VolumePagedWorldSection::setPageRangeMaxX(Ogre::int32 maxX)
{
  getGridStrategyData()->setCellRangeMaxX(maxX);
}

void
VolumePagedWorldSection::setPageRangeMaxY(Ogre::int32 maxY)
{
  getGridStrategyData()->setCellRangeMaxY(maxY);
}

Ogre::int32
VolumePagedWorldSection::getPageRangeMinX() const
{
  return getGridStrategyData()->getCellRangeMinX();
}

Ogre::int32
VolumePagedWorldSection::getPageRangeMinY() const
{
  return getGridStrategyData()->getCellRangeMinY();
}

Ogre::int32
VolumePagedWorldSection::getPageRangeMaxX() const
{
  return getGridStrategyData()->getCellRangeMaxX();
}

Ogre::int32
VolumePagedWorldSection::getPageRangeMaxY() const
{
  return getGridStrategyData()->getCellRangeMaxY();
}

Grid2DPageStrategy*
VolumePagedWorldSection::getGridStrategy() const
{
  return static_cast<Grid2DPageStrategy*> (this->getStrategy());
}

Grid2DPageStrategyData*
VolumePagedWorldSection::getGridStrategyData() const
{
  return static_cast<Grid2DPageStrategyData*> (mStrategyData);
}

/**
 * \note Serialization not currently implemented.
 */
void
VolumePagedWorldSection::loadSubtypeData(Ogre::StreamSerialiser &ser)
{
  //N.A
}
/**
 * \note Serialization not currently implemented.
 */
void
VolumePagedWorldSection::saveSubtypeData(Ogre::StreamSerialiser &ser)
{
  //N.A
}
/**
 * THis method hooks into volume maps for creating pages.
 */
void
VolumePagedWorldSection::loadPage(PageID pageID, bool forceSynchronous)
{
  if (!mParent->getManager()->getPagingOperationsEnabled())
    return;

  PageMap::iterator i = mPages.find(pageID);
  if (i == mPages.end())
    {
      long x, y;
      _unpackIndex(pageID, &x, &y);
      std::stringstream ss;
      ss << "++++++++++++++Load PageID: " << x << "," << -y << "++++++++++++++++\n";
      Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL, ss.str());
      _volumeMap->loadPage(pageID);
      //_volumeMap->loadPage(pageID, forceSynchronous);
    }
  PagedWorldSection::loadPage(pageID, forceSynchronous);
}

void
VolumePagedWorldSection::unloadPage(PageID pageID, bool forceSynchronous)
{
  if (!mParent->getManager()->getPagingOperationsEnabled())
    return;

  long x, y;
  _unpackIndex(pageID, &x, &y);
  std::stringstream ss;
  ss << "Unload PageID: " << x << "," << y << "\n";
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL, ss.str());

  PagedWorldSection::unloadPage(pageID, forceSynchronous);

  _volumeMap->unloadPage(pageID);

}

void
VolumePagedWorldSection::_unpackIndex(PageID pageID, long *x, long *y)
{
  using Ogre::uint16;
  using Ogre::int16;
  uint16 y16 = static_cast<uint16> (pageID & 0xFFFFF);
  uint16 x16 = static_cast<uint16> ((pageID >> 16) & 0xFFFF);

  *x = static_cast<int16> (x16);
  *y = static_cast<int16> (y16);
}

