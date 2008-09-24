/*
 * GPUEntsGenProps.cpp
 *
 *  Created on: Sep 19, 2008
 *      Author: bey0nd
 */

#include "GPUEntsGenProps.h"

using namespace ZGame;
using namespace Ogre;


GPUEntsGenProps::GPUEntsGenProps(const int texWidth,const int texHeight,Real entsHeight) :
_texWidth(texWidth),_texHeight(texHeight), _entHeight(entsHeight)
{
  computeNumOfEnts();
}

GPUEntsGenProps::~GPUEntsGenProps()
{

}

void GPUEntsGenProps::setExtents(Real minx, Real minz, Real maxx, Real maxz)
{
  _extents.setExtents(minx,0.0f,minz,maxx,0.0,maxz);
}

void GPUEntsGenProps::computeNumOfEnts()
{
  _numOfEntities = _texWidth*_texHeight;
}
