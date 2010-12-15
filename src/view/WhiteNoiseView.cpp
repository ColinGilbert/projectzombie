/*
 * WhiteNoiseView.cpp
 *
 *  Created on: Oct 4, 2008
 *      Author: bey0nd
 */
//#include <string>
#include <cstdlib>
using namespace std;
#include "WhiteNoiseView.h"
#include "LifeCycleDelegates.h"
#include "PingPongShaders.h"
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

    srand(_timer.getMillisecondsCPU());
    int randNum = rand();
    //cout << "randNUm: " << randNum << endl;

    MaterialPtr mat = MaterialManager::getSingleton().getByName(
        "ZGame/PingPongShadersTest");
    Ogre::GpuProgramParametersSharedPtr gpu =
        mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
    gpu->setNamedConstant("key", randNum);

    _pingPong->pingPong();

    return true;
  }

  bool
  WhiteNoiseView::onInit()
  {
    SceneManager* scnMgr = EngineView::getSingleton().getSceneManager();

    _whiteNoiseTex = TextureManager::getSingleton().createManual(
        "WhiteNoiseTestRTT",
        ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME, TEX_TYPE_2D,
        512, 512, 0, Ogre::PF_R8G8B8, Ogre::TU_RENDERTARGET);


    //initialize ping pong
    Ogre::LogManager::getSingleton().logMessage(LML_TRIVIAL,
        "initializing pingpong in WhiteNoiseView.");
    _pingPong.reset(new PingPongShaders(_whiteNoiseTex, MaterialPtr(
        MaterialManager::getSingleton().getByName("ZGame/PingPongShadersTest")),
        Real(50000.0f)));


    //the plane to put texture on
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

    return true;
  }

}
