/*
 * GPUEntsGenProps.cpp
 *
 *  Created on: Sep 19, 2008
 *      Author: bey0nd
 */

#include "GPUEntsGenProps.h"
#include "Imposter.h"
using namespace ZGame;
using namespace Ogre;

GPUEntsGenProps::GPUEntsGenProps(const int texWidth, const int texHeight,
    Real entsHeight) :
  _texWidth(texWidth), _texHeight(texHeight), _entHeight(entsHeight)
{
  computeNumOfEnts();
}

GPUEntsGenProps::~GPUEntsGenProps()
{

}

void
GPUEntsGenProps::setExtents(Real minx, Real minz, Real maxx, Real maxz)
{
  _extents.setExtents(minx, 0.0f, minz, maxx, 0.0, maxz);
}

void
GPUEntsGenProps::computeNumOfEnts()
{
  _numOfEntities = _texWidth * _texHeight;
}

const Ogre::AxisAlignedBox&
GPUEntsGenProps::getExtents()
{
  return _extents;
}

int
GPUEntsGenProps::getTexWidth()
{
  return _texWidth;
}

int
GPUEntsGenProps::getTexHeight()
{
  return _texHeight;
}

Ogre::Real
GPUEntsGenProps::getEntHeight()
{
  return _entHeight;
}

void
GPUEntsGenProps::setImposter(auto_ptr<Imposter> &imposter)
{
  _imposter = imposter;
}

Imposter*
GPUEntsGenProps::getImposter()
{
  return _imposter.get();
}

