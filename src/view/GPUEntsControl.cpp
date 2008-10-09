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

  GPUEntsControl::GPUEntsControl() :
    _elapsedT(0.0f),_gpuEnts(0)
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
            "ZGame/GPUEntsPosUpdatePingPong")), Real(50000.0f)));
    _dirPingPong.reset(new PingPongShaders(_dirTex, MaterialPtr(
        MaterialManager::getSingleton().getByName(
            "ZGame/GPUEntsDirUpdatePingPong")), Real(50000.0f)));
    //attach direction texture to pos update shader
    MaterialPtr posMat = MaterialManager::getSingleton().getByName(
        "ZGame/GPUEntsPosUpdatePingPong");
    assert(!posMat.isNull() && "posMat in null GPUEntsControl::init");
    posMat->getTechnique(0)->getPass(0)->getTextureUnitState(1)->setTextureName(
        _dirTex->getName());

    lm->logMessage(LML_TRIVIAL, "Out GPUEntsControl::init");
  }

  void
  GPUEntsControl::fillLfcObservers(LifeCycle::LifeCycleObserver &obs)
  {
    obs.onUpdate.bind(&GPUEntsControl::onUpdate, this);
  }

  bool
  GPUEntsControl::onInit()
  {
    return true;
  }

  bool
  GPUEntsControl::onUpdate(const Ogre::FrameEvent &evt)
  {
    if (_elapsedT > _DIRUPDATEPERIOD) //we only do directional updates per _DIRUPDATEPERIOD
      {
        srand(_timer.getMillisecondsCPU());
        int randNum = rand();
        MaterialPtr mat = MaterialManager::getSingleton().getByName(
            "ZGame/GPUEntsDirUpdatePingPong");
        mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant(
            "key", randNum);
        _dirPingPong->pingPong();
        _elapsedT = 0.0f-evt.timeSinceLastFrame;
      }

    //update position
    MaterialPtr mat = MaterialManager::getSingleton().getByName(
        "ZGame/GPUEntsPosUpdatePingPong");
    mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant("dt",
        evt.timeSinceLastFrame);
    _posPingPong->pingPong();
    _elapsedT += evt.timeSinceLastFrame;
    return true;
  }

}
