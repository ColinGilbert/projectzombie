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

using namespace ZGame;

int Imposter::_id = 0;

Imposter::Imposter(const string meshName) : _meshName(meshName),TYPE_NAME("IMPOSTER")
{
  _curId = _id;
  _id++;
  _texs.reserve(SEGPHI);
  for(int i = 0; i <= SEGPHI; i++)
    {
      ImposterTexVec vec;
      vec.reserve(SEGTHETA);
      _texs.push_back(vec);
    }
  setupTextures();
}

Imposter::~Imposter()
{

}

void Imposter::setupTextures()
{
  using namespace Ogre;
  TexturePtr temp;
  ostringstream ss;
  size_t texId = 0;
  for(size_t i=0; i <= SEGPHI; i++)
    {

      for(size_t j=0; j< SEGTHETA; j++)
        {
          ss << TYPE_NAME << _curId << texId++;
          temp = TextureManager::getSingleton().createManual(ss.str(),ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
              TEX_TYPE_2D,TEXDIM,TEXDIM,0,Ogre::PF_BYTE_BGRA,TU_RENDERTARGET);
          _texs[i].push_back(temp);
          ss.str("");
        }

    }
}


