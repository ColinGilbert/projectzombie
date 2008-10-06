/*
 * ImposterGen.cpp
 *
 *  Created on: Sep 4, 2008
 *      Author: bey0nd
 */

#include <cmath>
#include <stringstream>
#include <ostream>
using namespace std;
#include "Imposter.h"
#include "ImposterGen.h"
#include "EngineView.h"
using namespace ZGame;

int ImposterGen::_id = 0;

ImposterGen::ImposterGen() :
  _rotVal(0), _imposter(0)
{
  std::ostringstream ss;
  _curId = _id;
  _id++;
  _scnMgr = ZGame::EngineView::getSingleton().getSceneManager();
  ss << "IMPOSTER_CAMERA" << _curId;
  _imposterKeys["camera"] = ss.str();
  ss.str("");

  ss << "IMPOSTER_SCN_NODE" << _curId;
  _imposterKeys["scnnode"] = ss.str();
  ss.str("");
  ss << "IMPOSTER_ENTITY" << _curId;
  _imposterKeys["entity"] = ss.str();
  ss.str("");
  ss << "IMPOSTER_LIGHT" << _curId;
  _imposterKeys["light"] = ss.str();
  ss.str("");
  map<string, string>::iterator it;
  ss << "In ImposterGen::ImposterGen()" << endl;
  ss << "ImposterKeys: " << endl;
  for (it = _imposterKeys.begin(); it != _imposterKeys.end(); ++it)
    {
      ss << "<key,value>: " << it->first << "," << it->second << endl;
    }
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL, ss.str());
}

ImposterGen::~ImposterGen()
{
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL,
      "in ~ImposterGen");
  Ogre::SceneNode* root = _scnMgr->getRootSceneNode();
  root->removeAndDestroyChild(_imposterNode->getName());
  _scnMgr->destroyEntity(_imposterKeys["entity"]);
  _scnMgr->destroyCamera(_imposterKeys["camera"]);
  _scnMgr->destroyLight(_imposterKeys["light"]);
}

void
ImposterGen::setInput(Imposter* input)
{
  _imposter = input;
  _rotVal = 2 * Ogre::Math::PI / _imposter->SEGTHETA;
  setupCam();
  loadMesh();
  setupRTT();

}

void
ImposterGen::build()
{
  if (_imposter)
    {
      renderToTextures();
    }
  else
    Ogre::LogManager::getSingleton().logMessage(Ogre::LML_CRITICAL,
        "ImposterGen::build(), null imposter.");
}

void
ImposterGen::renderToTextures()
{
  using namespace Ogre;
  Radian rotPhi(-Math::HALF_PI / _imposter->SEGPHI);
  Radian rotTheta(-Math::TWO_PI / _imposter->SEGTHETA);
  //for each phi value,
  TexturePtr tex = _imposter->getTextures();
  RenderTarget* rtt = tex->getBuffer()->getRenderTarget();
  Viewport* vp = _cam->getViewport();
  //render theta # of shots to texture
  _imposterNode->setVisible(true, true); //set the damn imposter visible.
  int width = _imposter->getWidth();
  int height = _imposter->getHeight();
  Real unitWidth = (Real) _imposter->getDim() / width;
  Real unitHeight = (Real) _imposter->getDim() / height;
  for (size_t i = 0; i <= _imposter->SEGPHI; i++)
    {
      for (size_t j = 0; j < _imposter->SEGTHETA; j++)
        {
          vp->setDimensions(j * unitWidth, i * unitHeight, unitWidth,
              unitHeight);
          rtt->update(); //take the shot
          camRotateTheta(rotTheta); //reposition the camera according to theta
        }
      camRotatePhi(rotPhi); //reposition the camera according to phi
    }
  rtt->setActive(false);
  rtt->removeAllViewports();
  rtt->removeAllListeners();
  _imposterNode->setVisible(false, true); //hide imposter
}

void
ImposterGen::camRotatePhi(Ogre::Radian& rot)
{
  //Note: we're using a crude way to do rotation. What we are doing is moving the camera to the center
  //of the imposter node. From there we rotate the camera on it's local axes, and translate
  //by camera offset, which we computed earlier based on the model's extent. This is equivelent to
  //rotation about an abitrary center of mass. I.E moving to origin, then rotate, then move back.
  using namespace Ogre;
  Vector3 pos = _imposterNode->getPosition();
  pos.y = pos.y + _cen.y;
  pos.z = pos.z - _cen.z;
  _cam->setPosition(pos);
  _cam->pitch(rot);
  _cam->moveRelative(Vector3(0.0f, 0.0f, -(_camOffset + _cen.z)));
}
void
ImposterGen::camRotateTheta(Ogre::Radian& rot)
{
  using namespace Ogre;
  Vector3 pos = _imposterNode->getPosition();
  pos.y = pos.y + _cen.y;
  pos.z = pos.z - _cen.z;
  _cam->setPosition(pos);
  _cam->yaw(rot);
  _cam->moveRelative(Vector3(0.0f, 0.0f, -(_camOffset + _cen.z)));
}

void
ImposterGen::rotateRight()
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

void
ImposterGen::rotateUp()
{
  using namespace Ogre;
  Vector3 pos = _imposterNode->getPosition();
  Radian rot(-_rotVal);
  //move the camera using a stupid method. Should attach it to some sort of scene graph node and move it that way.
  pos.y = pos.y + _cen.y;
  pos.z = pos.z - _cen.z;
  _cam->setPosition(pos);
  _cam->pitch(rot);
  _cam->moveRelative(Vector3(0.0, 0.0, -(_camOffset + _cen.z)));

  //_imposterNode->roll(rot,Ogre::Node::TS_LOCAL);
  //_imposterNode->translate(-_cen.x,-_cen.y,0.0);
}

void
ImposterGen::rotateDown()
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

void
ImposterGen::rotateLeft()
{
  using namespace Ogre;
  Radian rot(_rotVal);
  Vector3 pos = _imposterNode->getPosition();
  pos.y = pos.y + _cen.y;
  pos.z = pos.z - _cen.z; //move to center
  _cam->setPosition(pos);
  _cam->yaw(rot);
  _cam->moveRelative(Vector3(0.0, 0.0, -(_camOffset + _cen.z)));

}

void
ImposterGen::loadMesh()
{
  using namespace Ogre;
  Ogre::MeshPtr m = Ogre::MeshManager::getSingleton().getByName(
      _imposter->getMeshName().c_str());
  if (m.isNull())
    {
      m = Ogre::MeshManager::getSingleton().load(
          _imposter->getMeshName().c_str(),
          Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);
    }
  Real defaultZ = -100.0;
  Ogre::SceneNode* root = _scnMgr->getRootSceneNode();
  Ogre::Entity* ent = _scnMgr->createEntity(_imposterKeys["entity"].c_str(),
      _imposter->getMeshName().c_str());
  _imposterNode = root->createChildSceneNode(_imposterKeys["scnnode"].c_str(),
      Ogre::Vector3(0.0, 0.0, defaultZ), Quaternion::IDENTITY);
  _imposterNode->attachObject(ent);
  _imposterNode->setVisible(false, true); //hide imposter
  Ogre::Quaternion orient;
  orient.FromAxes(Ogre::Vector3::UNIT_Z, Ogre::Vector3::UNIT_Y,
      Ogre::Vector3::NEGATIVE_UNIT_X);
  _imposterNode->setOrientation(orient);
  fitImposterExtent(ent, _imposterNode, defaultZ);
}

void
ImposterGen::setupRTT()
{
  using namespace Ogre;
  ostringstream ss;
  ss << "In ImposterGen::stupCam()" << endl;
  Ogre::TexturePtr tex = _imposter->getTextures();
  Viewport* vp;
  RenderTarget* rttTex = tex->getBuffer()->getRenderTarget();
  vp = rttTex->addViewport(_cam);
  vp->setBackgroundColour(ColourValue::ZERO);
  rttTex->setAutoUpdated(false);
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL, ss.str());
}

void
ImposterGen::setupCam()
{
  using namespace Ogre;

  _cam = _scnMgr->createCamera(_imposterKeys["camera"].c_str());
  _cam->setNearClipDistance(_NEAR_CLIP);
  _cam->setFarClipDistance(_FAR_CLIP);
  _cam->setAspectRatio(_ASPECT_RATIO);
  _cam->lookAt(0.0, 0.0, -1.0);

  _scnMgr->setAmbientLight(ColourValue(0.25, 0.25, 0.25));

  Ogre::Light* l = _scnMgr->createLight(_imposterKeys["light"].c_str());
  l->setType(Ogre::Light::LT_DIRECTIONAL);
  Vector3 dir(0.5, 0.5, -1.0);
  dir.normalise();
  l->setDirection(dir);
  l->setDiffuseColour(1.0f, 1.0f, 0.8f);
  l->setSpecularColour(1.0f, 1.0f, 1.0f);
}

/**
 * This method fits the imposter extent to screen by using linear algebra by solving for intersection pt T. Assuming 2D case
 */
void
ImposterGen::fitImposterExtent(Ogre::Entity* ent, Ogre::SceneNode* node,
    Ogre::Real defaultZ)
{
  using namespace Ogre;
  AxisAlignedBox aabox = ent->getWorldBoundingBox(true);
  Vector3 max = aabox.getMaximum();
  Vector3 min = aabox.getMinimum();
  Vector3 cen = aabox.getCenter();
  Real fovy = _cam->getFOVy().valueRadians() / 2; //remember it's Beta/2
  node->setPosition(-cen.x, -cen.y, defaultZ); //center the bounding box
  //now we want to move the entity such that it's bound box is bounded by the screen exactly.
  //recal world bound
  aabox = ent->getWorldBoundingBox(true);
  max = aabox.getMaximum();
  min = aabox.getMinimum();
  //equation is given by: We're looking for the point at which distance between two rays is minimum. For our CASE it is assumed
  //this t is the intersection point due to the nature of our problem.
  Real offset = 3.5;
  Vector2 s1(max.z, max.y + offset);
  Vector2 p1(0, max.y + offset);
  Vector2 p0(0, 0.0);
  Vector2 s0(cos(fovy), sin(fovy));
  s1 = (s1 - p1).normalisedCopy();
  Real det = s0.dotProduct(s1);
  det = det * det - 1;
  //if det == 0 throw exception here, i.e det = 0
  det = 1 / det;
  Real i = (p1 - p0).dotProduct(s0); //manually expand matrix
  Real j = (p1 - p0).dotProduct(s1);
  det = det * (-s0.dotProduct(s1) * i + j);

  _imposterNode->setPosition(-cen.x, -cen.y, det);
  aabox = ent->getBoundingBox();
  _cen = aabox.getCenter();
  _camOffset = det;
}
