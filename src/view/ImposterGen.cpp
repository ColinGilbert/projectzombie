/*
 * ImposterGen.cpp
 *
 *  Created on: Sep 4, 2008
 *      Author: bey0nd
 */

#include "ImposterGen.h"
#include "EngineView.h"
using namespace ZGame;

ImposterGen::ImposterGen() : _texId(0),_CAMERA_NAME("IMPOSTER_CAMERA")
{
  _scnMgr = ZGame::EngineView::getSingleton().getSceneManager();
}

ImposterGen::~ImposterGen()
{

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
  Ogre::MeshPtr m = Ogre::MeshManager::getSingleton().getByName(_meshName.c_str());
  if(m.isNull())
    {
      m=Ogre::MeshManager::getSingleton().load(_meshName.c_str(),Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);
    }
}

void ImposterGen::setupCam()
{
  _cam = _scnMgr->getCamera(_CAMERA_NAME.c_str());
  if(!_cam)
    {
      _cam = _scnMgr->createCamera(_CAMERA_NAME.c_str());
      _cam->setNearClipDistance(_NEAR_CLIP);
      _cam->setFarClipDistance(_FAR_CLIP);
      _cam->setAspectRatio(_ASPECT_RATIO);
      _cam->setPosition(0.0f,0.0f,0.0f);
    }
}


