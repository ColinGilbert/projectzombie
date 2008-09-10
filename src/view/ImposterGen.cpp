/*
 * ImposterGen.cpp
 *
 *  Created on: Sep 4, 2008
 *      Author: bey0nd
 */

#include <cmath>

using namespace std;

#include "ImposterGen.h"
#include "EngineView.h"
using namespace ZGame;

ImposterGen::ImposterGen() : _texId(0),_CAMERA_NAME("IMPOSTER_CAMERA"),_IMPOSTER_NODE("IMPOSTER_SCN_NODE"),
_IMPOSTER_ENTITY("IMPOSTER_ENTITY"),_rotVal(2*Ogre::Math::PI/_segPhi)
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
  setupCam();
  loadMesh();

}

void ImposterGen::build()
{

}

void ImposterGen::rotateUp()
{
  using namespace Ogre;
  Vector3 pos = _imposterNode->getPosition();
  Radian rot(-_rotVal);
  //move the camera using a stupid method. Should attach it to some sort of scene graph node and move it that way.
  pos.y = pos.y + _cen.y;
  pos.z = pos.z - _cen.z;
  _cam->setPosition(pos);
  _cam->pitch(rot);
  _cam->moveRelative(Vector3(0.0,0.0,-(_camOffset+_cen.z)));

  //_imposterNode->roll(rot,Ogre::Node::TS_LOCAL);
  //_imposterNode->translate(-_cen.x,-_cen.y,0.0);
}

void ImposterGen::rotateDown()
{
  using namespace Ogre;
  Vector3 pos = _imposterNode->getPosition();
  Radian rot(_rotVal);
  //move the camera using a stupid method. Should attach it to some sort of scene graph node and move it that way.
  pos.y = pos.y + _cen.y;
  pos.z = pos.z - _cen.z;
  _cam->setPosition(pos);
  _cam->pitch(rot);
  _cam->moveRelative(Vector3(0.0, 0.0, -(_camOffset + _cen.z)));
}

void ImposterGen::rotateLeft()
{
  using namespace Ogre;
  Radian rot(_rotVal);
  Vector3 pos = _imposterNode->getPosition();
  pos.y = pos.y + _cen.y;
  pos.z = pos.z - _cen.z; //move to center
  _cam->setPosition(pos);
  _cam->yaw(rot);
  _cam->moveRelative(Vector3(0.0,0.0,-(_camOffset+_cen.z)));

}

void ImposterGen::rotateRight()
{
  using namespace Ogre;
  Radian rot(-_rotVal);
  Vector3 pos = _imposterNode->getPosition();
  pos.y = pos.y + _cen.y;
  pos.z = pos.z - _cen.z; //move to center
  _cam->setPosition(pos);
  _cam->yaw(rot);
  _cam->moveRelative(Vector3(0.0, 0.0, -(_camOffset + _cen.z)));
}

void ImposterGen::loadMesh()
{
  using namespace Ogre;
  Ogre::MeshPtr m = Ogre::MeshManager::getSingleton().getByName(_meshName.c_str());
  if(m.isNull())
    {
      m=Ogre::MeshManager::getSingleton().load(_meshName.c_str(),Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);
    }
  Real defaultZ = -100.0;
  Ogre::SceneNode* root = _scnMgr->getRootSceneNode();
  Ogre::Entity* ent = _scnMgr->createEntity(_IMPOSTER_ENTITY.c_str(),_meshName.c_str());
  _imposterNode = root->createChildSceneNode(Ogre::Vector3(0.0,0.0,defaultZ),Quaternion::IDENTITY);
  _imposterNode->attachObject(ent);
  Ogre::Quaternion orient;
  orient.FromAxes(Ogre::Vector3::UNIT_Z,Ogre::Vector3::UNIT_Y,Ogre::Vector3::NEGATIVE_UNIT_X);
  _imposterNode->setOrientation(orient);
  fitImposterExtent(ent,_imposterNode,defaultZ);
  //_camNode = _imposterNode->createChildSceneNode("IMPOSTER_CAM_NODE",Vector3::ZERO,Quaternion::IDENTITY);
  //_camNode->attachObject(_cam);
  //_camNode->translate(_cen.x,_cen.y,-_cen.z,Node::TS_PARENT);
  /*
  Plane plane(Vector3::UNIT_Y,0);
  MeshManager::getSingleton().createPlane("ground",
      ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
      plane,1500,1500,20,20,true,1,5,5,Vector3::UNIT_Z);
  ent = _scnMgr->createEntity("GroundPlaneEnt","ground");
  root->attachObject(ent);
  */

}

void ImposterGen::setupCam()
{
  using namespace Ogre;
  _cam = ZGame::EngineView::getSingleton().getCurrentCamera();
  _cam->setPosition(0.0,0.0,0.0);
  _cam->setNearClipDistance(1.0);
  _cam->setFarClipDistance(5000.0);



  //_cam->setOrientation(orient);
  _cam->lookAt(0.0,0.0,-1.0);

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

/**
 * This method fits the imposter extent to screen by using linear algebra by solving for intersection pt T. Assuming 2D case
 */
void ImposterGen::fitImposterExtent(Ogre::Entity* ent,Ogre::SceneNode* node,Ogre::Real defaultZ)
{
  using namespace Ogre;
  AxisAlignedBox aabox = ent->getWorldBoundingBox(true);
  Vector3 max = aabox.getMaximum();
  Vector3 min = aabox.getMinimum();
  Vector3 cen = aabox.getCenter();
  Real fovy = _cam->getFOVy().valueRadians()/2; //remember it's Beta/2
  node->setPosition(-cen.x,-cen.y,defaultZ); //center the bounding box
  //now we want to move the entity such that it's bound box is bounded by the screen exactly.
  //recal world bound
  aabox = ent->getWorldBoundingBox(true);
  max = aabox.getMaximum();
  min = aabox.getMinimum();
  //equation is given by: We're looking for the point at which distance between two rays is minimum. For our CASE it is assumed
  //this t is the intersection point due to the nature of our problem.
  Real offset = 2.5;
  Vector2 s1(max.z,max.y+offset);
  Vector2 p1(0,max.y+offset);
  Vector2 p0(0,0.0);
  Vector2 s0(cos(fovy),sin(fovy));
  s1 = (s1-p1).normalisedCopy();
  Real det = s0.dotProduct(s1);
  det = det*det-1;
  //if det == 0 throw exception here, i.e det = 0
  det = 1/det;
  Real i = (p1-p0).dotProduct(s0); //manually expand matrix
  Real j = (p1-p0).dotProduct(s1);
  det = det*(-s0.dotProduct(s1)*i+j);

  _imposterNode->setPosition(-cen.x,-cen.y,det);
  aabox = ent->getBoundingBox();
  _cen = aabox.getCenter();
  _camOffset = det;
}
