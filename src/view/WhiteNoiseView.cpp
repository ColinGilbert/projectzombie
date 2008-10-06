/*
 * WhiteNoiseView.cpp
 *
 *  Created on: Oct 4, 2008
 *      Author: bey0nd
 */
#include <string>
using namespace std;
#include "WhiteNoiseView.h"
#include "EngineView.h"
#include "LifeCycleDelegates.h"
namespace ZGame
{

  WhiteNoiseView::WhiteNoiseView() : _fullScreenQuad(0)
  {
    // TODO Auto-generated constructor stub

  }

  WhiteNoiseView::~WhiteNoiseView()
  {
    // TODO Auto-generated destructor stub
    if (_fullScreenQuad)
      delete _fullScreenQuad;
  }

  bool
  WhiteNoiseView::onUpdate(const Ogre::FrameEvent &evt)
  {
  }

  void
  WhiteNoiseView::fillLfcObservers(ZGame::LifeCycle::LifeCycleObserver &obs)
  {
    obs.onUpdate.bind(&WhiteNoiseView::onUpdate, this);

  }

  void
  WhiteNoiseView::init()
  {
    SceneManager* scnMgr = EngineView::getSingleton().getSceneManager();
    /**
    Rectangle2D* rect = new Rectangle2D(true);
    rect->setCorners(-1.0,1.0,1.0,-1.0);
    rect->setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY);
    rect->setMaterial("ZGame/WhiteNoiseTest");
    SceneManager* scnMgr = EngineView::getSingleton().getSceneManager();
    SceneNode* node = scnMgr->getRootSceneNode()->createChildSceneNode();
    node->attachObject(rect);
    **/

    Plane plane(Vector3::UNIT_Z,0);
    MeshManager::getSingleton().createPlane("TempWhiteNoisePlane",
        ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,plane,50,50);
    Entity* noiseEnt = scnMgr->createEntity("WhiteNoiseEnt","TempWhiteNoisePlane");
    noiseEnt->setVisible(true);
    SceneNode* node2 = scnMgr->getRootSceneNode()->createChildSceneNode();
    node2->setPosition(0.0,10.0,0.0);
    node2->attachObject(noiseEnt);
    noiseEnt->setMaterialName("ZGame/WhiteNoiseTest");
  }

}
