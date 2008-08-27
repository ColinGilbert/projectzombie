/*
 * EngineView.cpp
 *
 *  Created on: Aug 26, 2008
 *      Author: bey0nd
 */

#include "EngineView.h"

using namespace ZGame;

EngineView::EngineView(Ogre::RenderWindow* window,Ogre::Camera* cam) : renderWindow(window),_curCam(cam)
{
  // TODO Auto-generated constructor stub

}

EngineView::~EngineView()
{
  // TODO Auto-generated destructor stub
}

void EngineView::setCurrentCamera(Ogre::Camera* cam)
{
  _curCam = cam;
}
