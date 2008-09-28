/*
 * GPUEntsView.cpp
 *
 *  Created on: Sep 22, 2008
 *      Author: bey0nd
 */
#include "GPUEntsView.h"
#include "GPUEntities.h"
#include "EngineView.h"
#include "GPUEntsMeshBuilder.h"
#include <Ogre.h>
using namespace ZGame;
using namespace Ogre;
GPUEntsView::GPUEntsView() : _meshName("GPUEntsMesh"),_entsOgrEntName("GPUEntsEntities"),
_entsOgrEntMatName("ZGame/GPUEntsView"),_sceneAlphaBld(true)
{

}

GPUEntsView::~GPUEntsView()
{

}

/**
 * Initialize this view.
 */
void GPUEntsView::init()
{
  LogManager* lm = Ogre::LogManager::getSingletonPtr();
  lm->logMessage(Ogre::LML_TRIVIAL,"GPUEntsView::init");
  //generate the GPUEntsMesh
  MeshPtr ptr = GPUEntsMeshBuilder::build(_meshName.c_str(),_ents->getProperties());
  SceneManager* scnMgr = EngineView::getSingleton().getSceneManager();
  _ogrEnt = scnMgr->createEntity(_entsOgrEntName.c_str(),ptr->getName());
  SceneNode* node = scnMgr->getRootSceneNode()->createChildSceneNode(Vector3(0.0f,0.0f,0.0f));
  node->attachObject(_ogrEnt);
  initOgrEnt();
  lm->logMessage(LML_TRIVIAL,"GPUEntsView::init out");
}

void GPUEntsView::alphaBlend()
{
  MaterialPtr mat = MaterialManager::getSingleton().getByName(_entsOgrEntMatName.c_str());
  if(_sceneAlphaBld)
    {
      mat->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_ONE,Ogre::SBF_ZERO);
      _sceneAlphaBld = false;
    }
  else
    {
      mat->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
      _sceneAlphaBld = true;
    }
}


//void GPUEntsView::initOgrEnt()
void GPUEntsView::initOgrEnt()
{
  int pass = 0;
  LogManager* lm = LogManager::getSingletonPtr();
  lm->logMessage(LML_TRIVIAL,"GPUEntsView::initOgrEnt");
  MaterialPtr mat = MaterialManager::getSingleton().getByName(_entsOgrEntMatName.c_str());

  _vertParam = mat->getTechnique(0)->getPass(pass)->getVertexProgramParameters();
  TexturePtr tex = TextureManager::getSingleton().getByName(_ents->getEntsData());
  lm->logMessage(LML_TRIVIAL,"state texture data name:"+_ents->getEntsData());
  //attach state and imposter textures to material
  mat->getTechnique(0)->getPass(pass)->getTextureUnitState(0)->setTextureName(tex->getName()); //state texture
  tex = TextureManager::getSingleton().getByName(_ents->getImposterTex()); //imposter texture
  lm->logMessage(LML_TRIVIAL,"imposter texture name:"+_ents->getImposterTex());
  mat->getTechnique(0)->getPass(pass)->getTextureUnitState(1)->setTextureName(tex->getName());

  //setup shader parameters
  size_t impWidth = tex->getWidth();
  size_t impHeight = tex->getHeight();
  //setup the width and height parameters
  //fragParams->setNamedConstant("texDim",(Real)128.0);
  Real scaleS = 256.0/impWidth;
  Real scaleT = 256.0/impHeight;
  _vertParam->setNamedConstant("scaleS",scaleS);
  _vertParam->setNamedConstant("scaleT",scaleT);


  //fragParams->setNamedConstant("imposterWidth", (Real)impWidth);
  //fragParams->setNamedConstant("imposterHeight", (Real) impHeight);


  _ogrEnt->setMaterialName(_entsOgrEntMatName.c_str());




  lm->logMessage(LML_TRIVIAL,"About to set _ogrEnt visible");
  _ogrEnt->setVisible(true);
  lm->logMessage(LML_TRIVIAL,"GPUEntsView::initOgrEnt out");
}

void GPUEntsView::attachGPUEnts(GPUEntities* ent)
{
  _ents = ent;
  init();
}





