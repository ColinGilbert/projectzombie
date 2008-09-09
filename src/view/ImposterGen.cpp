/*
 * ImposterGen.cpp
 *
 *  Created on: Sep 4, 2008
 *      Author: bey0nd
 */

#include "ImposterGen.h"
#include "EngineView.h"
using namespace ZGame;

ImposterGen::ImposterGen() : _texId(0),_CAMERA_NAME("IMPOSTER_CAMERA"),_IMPOSTER_NODE("IMPOSTER_SCN_NODE"),
_IMPOSTER_ENTITY("IMPOSTER_ENTITY")
{
  _scnMgr = ZGame::EngineView::getSingleton().getSceneManager();
}

ImposterGen::~ImposterGen()
{
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL,"in ~ImposterGen");
  Ogre::SceneNode* root = _scnMgr->getRootSceneNode();
  root->removeAndDestroyChild(_imposterNode->getName());
  //root->removeAndDestroyChild(_imposterNode.getName());
  _scnMgr->destroyEntity(_IMPOSTER_ENTITY.c_str());
}

void ImposterGen::setInput(const string meshName)
{
  _texId = 0;
  _meshName = meshName;
  loadMesh();
  setupCam();
}

void ImposterGen::build()
{

}

void ImposterGen::loadMesh()
{
  using namespace Ogre;
  Ogre::MeshPtr m = Ogre::MeshManager::getSingleton().getByName(_meshName.c_str());
  if(m.isNull())
    {
      m=Ogre::MeshManager::getSingleton().load(_meshName.c_str(),Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);
    }
  Ogre::SceneNode* root = _scnMgr->getRootSceneNode();
  Ogre::Entity* ent = _scnMgr->createEntity(_IMPOSTER_ENTITY.c_str(),_meshName.c_str());
  _imposterNode = root->createChildSceneNode(Ogre::Vector3(0.0,0.0,-50.0),Quaternion::IDENTITY);
  _imposterNode->attachObject(ent);
  Ogre::AxisAlignedBox aabox = ent->getBoundingBox();
  Vector3 max = aabox.getMaximum();
  Vector3 min = aabox.getMinimum();
  cout << "imposter max: " << max << endl;
  cout << "imposter min: " << min << endl;

  Plane plane(Vector3::UNIT_Y,0);
  MeshManager::getSingleton().createPlane("ground",
      ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
      plane,1500,1500,20,20,true,1,5,5,Vector3::UNIT_Z);
  ent = _scnMgr->createEntity("GroundPlaneEnt","ground");
  root->attachObject(ent);

}

void ImposterGen::setupCam()
{
  using namespace Ogre;
  _cam = ZGame::EngineView::getSingleton().getCurrentCamera();
  _cam->setPosition(0.0,10.0,500.0);
  _cam->setNearClipDistance(0.1);
  _cam->setFarClipDistance(5000.0);
  Ogre::Quaternion orient;
  orient.FromAxes(Ogre::Vector3::NEGATIVE_UNIT_X,Ogre::Vector3::UNIT_Y,Ogre::Vector3::NEGATIVE_UNIT_Z);
  //_cam->setOrientation(orient);
  _cam->lookAt(0.0,0.0,0.0);
  _scnMgr->setAmbientLight(ColourValue(0.25,0.25,0.25));
  Ogre::Light* l = _scnMgr->createLight("ImposterLight");
  l->setType(Ogre::Light::LT_DIRECTIONAL);
  Vector3 dir(0.5,0.5,-1.0);
  dir.normalise();
  l->setDirection(dir);
  l->setDiffuseColour(1.0f, 1.0f, 0.8f);
  l->setSpecularColour(1.0f, 1.0f, 1.0f);
  /*
  _cam = _scnMgr->getCamera(_CAMERA_NAME.c_str());
  if(!_cam)
    {
      _cam = _scnMgr->createCamera(_CAMERA_NAME.c_str());
      _cam->setNearClipDistance(_NEAR_CLIP);
      _cam->setFarClipDistance(_FAR_CLIP);
      _cam->setAspectRatio(_ASPECT_RATIO);
      _cam->setPosition(0.0f,0.0f,0.0f);
    }
    */
}


