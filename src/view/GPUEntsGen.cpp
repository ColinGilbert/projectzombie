/*
 * GPUEntsGen.cpp
 *
 *  Created on: Sep 18, 2008
 *      Author: bey0nd
 */

#include "GPUEntsGen.h"
#include "ZEntity.h"
#include "GPUEntities.h"
#include "Imposter.h"
#include "ImposterGen.h"
using namespace ZGame;

GPUEntsGen::GPUEntsGen(ZEntity* ent,GPUEntities* input,GPUEntsGenProps* props) : _input(input),_ent(ent),_props(props)
{
}

GPUEntsGen::~GPUEntsGen()
{
}

void GPUEntsGen::build()
{
  //generate the imposters
  genImposters();
  //generate the gpu entity position
}

void GPUEntsGen::genImposters()
{
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL,"In GPUEntsGen::genImposters");
  Imposter imposter(_ent->getMeshName());
  ImposterGen impGen;
  impGen.setInput(&imposter);
  impGen.build();
  _input->setImposterTex(imposter.getTextures());
}

void GPUEntsGen::genPosition()
{

}



