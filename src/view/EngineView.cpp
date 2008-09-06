/*
 * EngineView.cpp
 *
 *  Created on: Aug 26, 2008
 *      Author: bey0nd
 */

#include "EngineView.h"

using namespace ZGame;

template<> EngineView* Ogre::Singleton<EngineView>::ms_Singleton = 0;

EngineView::EngineView(Ogre::RenderWindow* window,Ogre::Camera* cam) : renderWindow(window),_curCam(cam)
{
  // TODO Auto-generated constructor stub
  _scnMgr = _curCam->getSceneManager();
}

EngineView::~EngineView()
{
  // TODO Auto-generated destructor stub
}

void EngineView::setCurrentCamera(Ogre::Camera* cam)
{
  _curCam = cam;
}

EngineView& EngineView::getSingleton()
{
  return *ms_Singleton;
}

EngineView* EngineView::getSingletonPtr()
{
  return ms_Singleton;
}
