/*
 * Imposter.cpp
 *
 *  Created on: Sep 11, 2008
 *      Author: bey0nd
 */
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include <sstream>
#else
#include <stringstream>
#endif

#include <iostream>
using namespace std;
#include "Imposter.h"
#include "EngineView.h"
using namespace ZGame;
int Imposter::_id = 0;

Imposter::Imposter(const Ogre::String meshName) : _meshName(meshName),TYPE_NAME("IMPOSTER")
{
  _curId = _id;
  _id++;
  setupTextures();
}

Imposter::~Imposter()
{
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL,"Out of Imposter::~Imposter");
}

void Imposter::init()
{
  //setupTextures();
}

const Ogre::String Imposter::getMeshName()
{
  return _meshName;
}

Ogre::TexturePtr Imposter::getTextures()
{
  return _texture;
}

int Imposter::getWidth()
{
  return SEGTHETA*TEXDIM;
}

int Imposter::getHeight()
{
  return SEGPHI*TEXDIM;
}

int Imposter::getDim()
{
  return TEXDIM;
}

void Imposter::setupTextures()
{
  using namespace Ogre;
  //TexturePtr temp;
  ostringstream ss;
  size_t texId = 0;

  //ss << TYPE_NAME << _meshName << _curId;
  ss << TYPE_NAME << _meshName << 0;
  int height = getHeight(); //PHI varies along vertical of texture.
  int width = getWidth(); //Theta varies along the horizontal of texture.
  
  _texture = TextureManager::getSingleton().createManual("IMPOSTER_TEST_MESH_TEX",
    ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,TEX_TYPE_2D,width,height,0,Ogre::PF_BYTE_RGBA,TU_RENDERTARGET);
  
}


