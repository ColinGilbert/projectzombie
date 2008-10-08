/*
 * WhiteNoiseView.cpp
 *
 *  Created on: Oct 4, 2008
 *      Author: bey0nd
 */
#include <string>
#include <cstdlib>
using namespace std;
#include "WhiteNoiseView.h"
#include "EngineView.h"
#include "LifeCycleDelegates.h"
namespace ZGame
{

  WhiteNoiseView::WhiteNoiseView() :
    _fullScreenQuad(0)
  {
    // TODO Auto-generated constructor stub

  }

  WhiteNoiseView::~WhiteNoiseView()
  {
    // TODO Auto-generated destructor stub

  }

  bool
  WhiteNoiseView::onUpdate(const Ogre::FrameEvent &evt)
  {
    //render white noise to texture
    RenderTarget* rtt = _whiteNoiseTex->getBuffer()->getRenderTarget();
    SceneManager* scnMgr = EngineView::getSingleton().getSceneManager();
    SceneNode* rootNode = scnMgr->getRootSceneNode();

    srand(_timer.getMillisecondsCPU());
    int randNum = rand();
    //cout << "randNUm: " << randNum << endl;

    MaterialPtr mat = MaterialManager::getSingleton().getByName("ZGame/WhiteNoiseTest");
    Ogre::GpuProgramParametersSharedPtr gpu = mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
    gpu->setNamedConstant("key",randNum);

    //rootNode->setVisible(false);
    _fullScreenQuad->setVisible(true);
   // rootNode->setVisible(false);
    rtt->update();
    //rootNode->flipVisibility(true);
    _fullScreenQuad->setVisible(false);
    //rootNode->setVisible(true);
    return true;
  }

  void
  WhiteNoiseView::fillLfcObservers(ZGame::LifeCycle::LifeCycleObserver &obs)
  {
    obs.onUpdate.bind(&WhiteNoiseView::onUpdate, this);
    obs.onInit.bind(&WhiteNoiseView::onInit,this);
  }

  bool
  WhiteNoiseView::onInit()
  {
    SceneManager* scnMgr = EngineView::getSingleton().getSceneManager();

    _whiteNoiseTex = TextureManager::getSingleton().createManual(
        "WhiteNoiseTestRTT",
        ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME, TEX_TYPE_2D,
        512.0, 512.0, 0, Ogre::PF_R8G8B8, Ogre::TU_RENDERTARGET);

    Camera* rttCam = scnMgr->createCamera("WhiteNoiseTestRTTCam");
    Camera* cam = EngineView::getSingleton().getCurrentCamera();
    rttCam->setAspectRatio(1.0);
    rttCam->setNearClipDistance(1.0);
    rttCam->setFarClipDistance(1000.0);
    rttCam->setPosition(0.0, 0.0, -49990.0);
    rttCam->lookAt(0.0, 0.0, -50000.0);
    RenderTarget* rttTex = _whiteNoiseTex->getBuffer()->getRenderTarget();
    Viewport* vp = rttTex->addViewport(rttCam);
    vp->setBackgroundColour(ColourValue::Red);
    vp->setClearEveryFrame(true);
    vp->setDimensions(0.0, 0.0, 1.0, 1.0);
    rttTex->setAutoUpdated(false);
    rttTex->setActive(true);

    Plane plane(Vector3::UNIT_Z, 0);
    MeshManager::getSingleton().createPlane("TempWhiteNoisePlane",
        ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, 50, 50);
    Entity* noiseEnt = scnMgr->createEntity("WhiteNoiseEnt",
        "TempWhiteNoisePlane");
    noiseEnt->setVisible(true);
    SceneNode* node2 = scnMgr->getRootSceneNode()->createChildSceneNode();
    node2->setPosition(0.0, 10.0, 0.0);
    node2->attachObject(noiseEnt);
    noiseEnt->setMaterialName("ZGame/WhiteNoiseQuad");
    Ogre::MaterialPtr mat = MaterialManager::getSingleton().getByName(
        "ZGame/WhiteNoiseQuad");
    TextureUnitState* texUnit =
        mat->getTechnique(0)->getPass(0)->getTextureUnitState(0);
    texUnit->setTextureName(_whiteNoiseTex->getName());

    _fullScreenQuad.reset(new Rectangle2D(true));
    _fullScreenQuad->setCorners(-1.0, 1.0, 1.0, -1.0);
    _fullScreenQuad->setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY);
    _fullScreenQuad->setMaterial("ZGame/WhiteNoiseTest");
    _fullScreenQuad->setVisible(false);
    _fullScreenQuad->setBoundingBox(Ogre::AxisAlignedBox(-5000.0, -5000.0,
        -5000.0, 5000.0, 5000.0, 5000.0));
    SceneNode* node = scnMgr->getRootSceneNode()->createChildSceneNode();
    node->attachObject(_fullScreenQuad.get());
    node->setPosition(0.0,0.0,-50000.0);
    //scnMgr->getRenderQueue()->addRenderable(_fullScreenQuad.get(),Ogre::RENDER_QUEUE_OVERLAY);
    //scnMgr->getRootSceneNode()->createChildSceneNode()->attachObject(_fullScreenQuad.get());


  }

}
