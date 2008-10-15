/*
 * GPUEntsView.cpp
 *
 *  Created on: Sep 22, 2008
 *      Author: bey0nd
 */
#include <iostream>
using namespace std;
#include "GPUEntsView.h"
#include "GPUEntities.h"
#include "EngineView.h"
#include "GPUEntsMeshBuilder.h"
#include "LifeCycleDelegates.h"
#include "Imposter.h"
using namespace ZGame;
using namespace Ogre;
GPUEntsView::GPUEntsView() :
  _ents(0), _meshName("GPUEntsMesh"), _entsOgrEntName("GPUEntsEntities"),
      _entsOgrEntMatName("ZGame/GPUEntsView"), _sceneAlphaBld(true), _cam(0)
{

}

GPUEntsView::~GPUEntsView()
{

}

/**
 * Initialize this view.
 */
void
GPUEntsView::init()
{
  LogManager* lm = Ogre::LogManager::getSingletonPtr();
  lm->logMessage(Ogre::LML_TRIVIAL, "GPUEntsView::init");
  try
    {
      initGPUEntsMesh();
    }
  catch (Exception e)
    {
      ostringstream oss;
      oss << " .Exeception in GpUEntsView::init() on initGPUEntsMesh()."
          << endl;
      throw Exception(e.getNumber(), e.getDescription() + oss.str(),
          e.getSource());
    }
  initOgrEnt();
  initCamera();
  lm->logMessage(LML_TRIVIAL, "GPUEntsView::init out");

}

void
GPUEntsView::initGPUEntsMesh()
{
  Ogre::LogManager* lm = LogManager::getSingletonPtr();
  lm->logMessage(LML_TRIVIAL, "In GPUEntsView::initGPUEntsMesh");
  //generate the GPUEntsMesh
  MeshPtr ptr = GPUEntsMeshBuilder::build(_meshName.c_str(),
      _ents->getProperties());
  SceneManager* scnMgr = EngineView::getSingleton().getSceneManager();
  _ogrEnt = scnMgr->createEntity(_entsOgrEntName.c_str(), ptr->getName());
  //_ogrEnt->setRenderQueueGroup(Ogre::RENDER_QUEUE_MAIN);
  SceneNode* node = scnMgr->getRootSceneNode()->createChildSceneNode(Vector3(
      0.0f, 0.0f, 0.0f));
  node->attachObject(_ogrEnt);
  lm->logMessage(LML_TRIVIAL, "Out GPUEntsView::initGPUEntsMesh");
}

void
GPUEntsView::initCamera()
{
  _cam = EngineView::getSingleton().getCurrentCamera();
}

bool
GPUEntsView::onUpdate(const Ogre::FrameEvent &evt)
{
  //MaterialPtr mat = MaterialManager::getSingleton().getByName(
      // _entsOgrEntMatName.c_str());
  Camera* cam = EngineView::getSingleton().getCurrentCamera();
  Vector3 camPos = cam->getPosition();

  _vertParam->setNamedConstant("vPos", camPos);

  return true;
}

void
GPUEntsView::fillLfcObservers(LifeCycle::LifeCycleObserver &obs)
{
  obs.onUpdate.bind(&GPUEntsView::onUpdate, this);
}

void
GPUEntsView::alphaBlend()
{
  MaterialPtr mat = MaterialManager::getSingleton().getByName(
      _entsOgrEntMatName.c_str());
  if (_sceneAlphaBld)
    {
      mat->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_ONE,
          Ogre::SBF_ZERO);
      _sceneAlphaBld = false;
    }
  else
    {
      mat->getTechnique(0)->getPass(0)->setSceneBlending(
          Ogre::SBT_TRANSPARENT_ALPHA);
      _sceneAlphaBld = true;
    }
}

//void GPUEntsView::initOgrEnt()
void
GPUEntsView::initOgrEnt()
{
  int pass = 0;
  LogManager* lm = LogManager::getSingletonPtr();
  lm->logMessage(LML_TRIVIAL, "GPUEntsView::initOgrEnt");
  MaterialPtr mat = MaterialManager::getSingleton().getByName(
      _entsOgrEntMatName.c_str());

  _vertParam
      = mat->getTechnique(0)->getPass(pass)->getVertexProgramParameters();
  TexturePtr tex = TextureManager::getSingleton().getByName(
      _ents->getEntsData());
  lm->logMessage(LML_TRIVIAL, "state texture data name:" + _ents->getEntsData());
  //attach state and imposter textures to material
  //state
  mat->getTechnique(0)->getPass(pass)->getTextureUnitState(0)->setTextureName(
      tex->getName()); //state texture
  //imposter
  tex = TextureManager::getSingleton().getByName(_ents->getImposterTex()); //imposter texture
  lm->logMessage(LML_TRIVIAL, "imposter texture name:"
      + _ents->getImposterTex());
  size_t impWidth = tex->getWidth();
    size_t impHeight = tex->getHeight();

  mat->getTechnique(0)->getPass(pass)->getTextureUnitState(1)->setTextureName(
      tex->getName());
  //dir
  tex = TextureManager::getSingleton().getByName(_ents->getGpuEntsDirData());
  mat->getTechnique(0)->getPass(pass)->getTextureUnitState(2)->setTextureName(
      tex->getName());

  //setup shader parameters

  //setup the width and height parameters
  //fragParams->setNamedConstant("texDim",(Real)128.0);
  Real scaleS = 128.0/ impWidth;
  Real scaleT = 128.0/ impHeight;

  _vertParam->setNamedConstant("scaleS", scaleS);
  _vertParam->setNamedConstant("scaleT", scaleT);

  _vertParam->setNamedConstant("segPhi", int(Imposter::SEGPHI)); //number of segments for phi
  _vertParam->setNamedConstant("segTheta", int(Imposter::SEGTHETA)); //number of segments for theta



  _ogrEnt->setMaterialName(_entsOgrEntMatName.c_str());

  lm->logMessage(LML_TRIVIAL, "About to set _ogrEnt visible");
  _ogrEnt->setVisible(true);
  lm->logMessage(LML_TRIVIAL, "GPUEntsView::initOgrEnt out");
}

void
GPUEntsView::attachGPUEnts(GPUEntities* ent)
{
  _ents = ent;
  init();
}

