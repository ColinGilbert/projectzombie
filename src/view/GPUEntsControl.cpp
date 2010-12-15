/*
 * GPUEntsControl.cpp
 *
 *  Created on: Oct 4, 2008
 *      Author: bey0nd
 */
#include <stdexcept>
#include <cstdlib>
using namespace std;
#include "GPUEntsControl.h"
#include "GPUEntities.h"
#include "LifeCycleDelegates.h"
#include "EventDelegates.h"
#include "PingPongShaders.h"

namespace ZGame
{
  using namespace Ogre;
  const Real GPUEntsControl::_DIRUPDATEPERIOD = 2.0; //two seconds per update of direction.

  GPUEntsControl::GPUEntsControl() :
    _elapsedT(0.0f), _gpuEnts(0)
  {
    // TODO Auto-generated constructor stub

  }

  GPUEntsControl::~GPUEntsControl()
  {
    // TODO Auto-generated destructor stub
  }

  void
  GPUEntsControl::attachGPUEnts(GPUEntities* ents)
  {

    if (!ents)
      throw std::invalid_argument(
          "Attempting to attach a null GPUEntities in GPUEntsControl.");

    _gpuEnts = ents;
    init();
  }

  void
  GPUEntsControl::init()
  {
    LogManager* lm = LogManager::getSingletonPtr();
    lm->logMessage(LML_TRIVIAL, "In GPUEntsControl::init");
    _stateTex = TextureManager::getSingleton().getByName(
        _gpuEnts->getEntsData());
    assert(!_stateTex.isNull() && "_stateTex is null in GPUEntsControl::init()");
    _dirTex = TextureManager::getSingleton().getByName(
        _gpuEnts->getGpuEntsDirData());
    assert(!_stateTex.isNull() && "_dirTex is null in GPUEntsControl::init()");
    //Setup the ping pongs
    _posPingPong.reset(new PingPongShaders(_stateTex, MaterialPtr(
        MaterialManager::getSingleton().getByName(
            "ZGame/GPUEntsPosUpdatePingPong")), Real(5000000.0f)));
    _dirPingPong.reset(new PingPongShaders(_dirTex, MaterialPtr(
        MaterialManager::getSingleton().getByName(
            "ZGame/GPUEntsDirUpdatePingPong")), Real(5000000.0f)));
    //attach direction texture to pos update shader
    MaterialPtr posMat = MaterialManager::getSingleton().getByName(
        "ZGame/GPUEntsPosUpdatePingPong");
    assert(!posMat.isNull() && "posMat in null GPUEntsControl::init");
    posMat->getTechnique(0)->getPass(0)->getTextureUnitState(1)->setTextureName(
        _dirTex->getName());
    MaterialPtr dirMat = MaterialManager::getSingleton().getByName(
        "ZGame/GPUEntsDirUpdatePingPong");
    dirMat->getTechnique(0)->getPass(0)->getTextureUnitState(1)->setTextureName(
        _stateTex->getName());

    lm->logMessage(LML_TRIVIAL, "Out GPUEntsControl::init");
  }

  

  bool
  GPUEntsControl::onInit()
  {
    return true;
  }

  bool
  GPUEntsControl::onUpdate(const Ogre::FrameEvent &evt)
  {
    //Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL,"In GPUEntsControl onUpdate");
    Vector3 camPos = EngineView::getSingleton().getCurrentCamera()->getPosition();

    srand(_timer.getMillisecondsCPU());
    int randNum = rand();
    MaterialPtr mat = MaterialManager::getSingleton().getByName(
        "ZGame/GPUEntsDirUpdatePingPong");
    Ogre::GpuProgramParametersSharedPtr gpu =
        mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
    //Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL,"Finished getting GPU parameters");
    //update direction
    gpu->setNamedConstant("key", randNum);
    gpu->setNamedConstant("dt", evt.timeSinceLastFrame);
    gpu->setNamedConstant("camPos",camPos);
    //Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL,"FInished setting GPU parameters");

    //Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL,"About to ping-pong direction update");
    _dirPingPong->pingPong();
    //Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL,"FInish ping-ponging direction update");

    //update position
    mat = MaterialManager::getSingleton().getByName(
        "ZGame/GPUEntsPosUpdatePingPong");
    mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant(
        "dt", evt.timeSinceLastFrame);
    gpu = mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
    gpu->setNamedConstant("key", randNum);
    gpu->setNamedConstant("camPos",camPos);
    
    _posPingPong->pingPong();
    //Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL,"Finished ping-ponging position update");

    //Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL,"Out of GPUEntsControl onUpdate");
    return true;
  }

}
