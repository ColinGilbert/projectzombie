/*
* GPUEntities.cpp
*
*  Created on: Sep 18, 2008
*      Author: bey0nd
*/

#include "GPUEntities.h"
#include "GPUEntsGenProps.h"

using namespace ZGame;
using namespace Ogre;
using namespace std;
GPUEntities::GPUEntities(const Ogre::String entsName, const Ogre::String entsData,
                         const Ogre::String dirData, const Ogre::String imposterTex,
                         auto_ptr<GPUEntsGenProps> props) :
_dirData(dirData), _gpuEntsData(entsData), _imposterTex(imposterTex),
_entsName(entsName), _props(props)
{
}

GPUEntities::~GPUEntities()
{
    TextureManager* texMgr = TextureManager::getSingletonPtr();
    texMgr->remove(_gpuEntsData.c_str());
    texMgr->remove(_imposterTex.c_str());
}

const string
GPUEntities::getEntsData()
{
    return _gpuEntsData;
}

const string
GPUEntities::getImposterTex()
{
    return _imposterTex;
}

const Ogre::String
GPUEntities::getGpuEntsDirData()
{
    return _dirData;
}

GPUEntsGenProps*
GPUEntities::getProperties()
{
    return _props.get();
}

void
GPUEntities::setEntsData(const Ogre::String texName)
{
    _gpuEntsData = texName;
}

void
GPUEntities::setImposterTex(const Ogre::String texName)
{
    _imposterTex = texName;
}

const Ogre::String
GPUEntities::getName()
{
    return _entsName;
}
