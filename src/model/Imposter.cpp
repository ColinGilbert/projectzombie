/*
 * Imposter.cpp
 *
 *  Created on: Sep 11, 2008
 *      Author: bey0nd
 */
#include <stringstream>
#include <iostream>
using namespace std;
#include "Imposter.h"
#include "EngineView.h"
using namespace ZGame;

int Imposter::_id = 0;

Imposter::Imposter(const string meshName) : _meshName(meshName),TYPE_NAME("IMPOSTER")
{
  _curId = _id;
  _id++;
  setupTextures();
}

Imposter::~Imposter()
{
  ostringstream oss;
  oss << "Destroying imposter: " << _curId;
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL,oss.str());
  Ogre::TextureManager::getSingleton().remove(_texture->getName());
}

void Imposter::setupTextures()
{
  using namespace Ogre;
  TexturePtr temp;
  ostringstream ss;
  size_t texId = 0;

  ss << TYPE_NAME << _meshName << _curId;
  int height = getHeight(); //PHI varies along vertical of texture.
  int width = getWidth(); //Theta varies along the horizontal of texture.

  _texture = TextureManager::getSingleton().createManual(ss.str(),
      ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,TEX_TYPE_2D,width,height,0,Ogre::PF_BYTE_BGRA,TU_RENDERTARGET);

}


