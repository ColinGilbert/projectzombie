/*
 * GPUEntsGen.cpp
 *
 *  Created on: Sep 18, 2008
 *      Author: bey0nd
 */
#include <string>
#include <ostringstream>
#include <logic_error>
using namespace std;
#include <boost/random.hpp>
#include <OgreBitwise.h>
#include "GPUEntsGen.h"
#include "ZEntity.h"
#include "GPUEntities.h"
#include "Imposter.h"
#include "ImposterGen.h"
#include "GPUEntsGenProps.h"
#include "GPUEntsDistributor.h"
#include "EngineView.h"
using namespace ZGame;
using namespace Ogre;

size_t GPUEntsGen::_uniqueId = 0;

GPUEntsGen::GPUEntsGen(ZEntity* ent,auto_ptr<GPUEntsGenProps> props) :_output(0),_ent(ent),_props(props),
_gpuEntsName("gpuEnts")
{
}

GPUEntsGen::~GPUEntsGen()
{
}

auto_ptr<GPUEntities> GPUEntsGen::getOutput()
{
  if(!_output)
    throw logic_error("GPUEntsGen output is null! Try calling build first.");
  return auto_ptr<GPUEntities>(_output);
}

void GPUEntsGen::build()
{
  LogManager* lm = LogManager::getSingletonPtr();
  lm->logMessage(Ogre::LML_TRIVIAL,"In GPUENtsGen::build()");
  genImposters(); //gen the imposters
  lm->logMessage(Ogre::LML_NORMAL,"Finished generating imposters.");
  loadPositions(); //load the positions into the state texture
  lm->logMessage(Ogre::LML_NORMAL,"Finished loading state texture.");
  genEntsGeom();
  lm->logMessage(Ogre::LML_NORMAL,"Finished generating entities geometry");
  _output = new GPUEntities(_gpuEntsName,_stateTex->getName().c_str(),
      _imposterTex->getName().c_str(),_props); //now we have enough information to build output.
}

void GPUEntsGen::genImposters()
{
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL,"In GPUEntsGen::genImposters");
  Imposter imposter(_ent->getMeshName());
  ImposterGen impGen;
  impGen.setInput(&imposter);
  impGen.build();
  _imposterTex = imposter.getTextures();
}

void GPUEntsGen::loadPositions()
{
  LogManager* lm = LogManager::getSingletonPtr();
  lm->logMessage(Ogre::LML_TRIVIAL,"In GPUEntsGen::loadPositions");
  createStateTexture();
  lm->logMessage(LML_NORMAL,"Finished creating textures");
  boost::minstd_rand rng;
  Vector3 min = _props->getExtents().getMinimum();
  Vector3 max = _props->getExtents().getMaximum();

  boost::uniform_int<> xDist(min.x,max.x);
  boost::uniform_int<> zDist(min.z,max.z);
  GPUEntsDistributor<boost::minstd_rand,boost::uniform_int<> > dist(rng,xDist,zDist);

  Vector3 pos;
  HardwarePixelBufferSharedPtr pixBuffer = _stateTex->getBuffer();
  pixBuffer->lock(HardwareBuffer::HBL_DISCARD);
  const PixelBox &pixBox = pixBuffer->getCurrentLock();
  Real* pDest = static_cast<Real*>(pixBox.data);
  int pitchY = pixBox.rowPitch;
  int pitchX = pixBox.slicePitch;
  ostringstream oss;
  oss << "rowPitch,slicePitch: " << pitchY << "," << pitchX << endl;
  lm->logMessage(Ogre::LML_TRIVIAL,oss.str());
  oss.str("");
  int w = pixBox.getWidth();
  int h = pixBox.getHeight();
  oss << "width,height: " << w << "," << h << endl;
  lm->logMessage(Ogre::LML_TRIVIAL,oss.str());
  //for each texel in the state texture. ASSUMING FLOAT_RGBA. We need to address this assumption later, so we do NOT ASSUME it.
  for(int j=0; j<h; j++)
    {
      for(int i=0;i<w;i++)
        {
          dist.nextPosition(pos); //get randomly distributed position
          PixelUtil::packColour((float)pos.x,(float)pos.y,(float)pos.z,1.0f,Ogre::PF_FLOAT32_RGBA,(void*)(&pDest[pitchY*j+i]));
        }
    }
  pixBuffer->unlock();
}

/**
 * This is a place holder for testing.
 */
void GPUEntsGen::createStateTexture()
{

  string texName = _gpuEntsName + "statettex"+nextId();

  _stateTex = TextureManager::getSingleton().createManual(texName,ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME,TEX_TYPE_2D,
      _props->getTexWidth(),_props->getTexHeight(),0,Ogre::PF_FLOAT32_RGBA,TU_RENDERTARGET);

}


void GPUEntsGen::genEntsGeom()
{

}

size_t GPUEntsGen::nextId()
{
  return _uniqueId++;
}
