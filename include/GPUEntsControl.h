/*
 * GPUEntsControl.h
 * This class is responsible for controlling the GPU entities's simulation.
 *  Created on: Oct 4, 2008
 *      Author: bey0nd
 */

#ifndef GPUENTSCONTROL_H_
#define GPUENTSCONTROL_H_
#include <string>
#include <iostream>
#include <memory>
using namespace std;
#include <Ogre.h>
using namespace Ogre;
#include "ObsInjectors.h"
namespace ZGame
{
  class GPUEntities;
  class PingPongShaders;
  namespace LifeCycle
  {
    struct LifeCycleObserver;
  }
  class GPUEntsControl : public LFCObsInjector
  {
  public:
    GPUEntsControl();
    virtual
    ~GPUEntsControl();
    //This method attaches the GPUEntities that this controller is going to control.
    void
    attachGPUEnts(GPUEntities* ents);
    //This method allows Life Cycle observers functors to be pushed to us.
    void
    fillLfcObservers(LifeCycle::LifeCycleObserver &obs);
    //Life Cycle methods
    //This method called during initialization phase of the life cycle.
    bool
    onInit();
    //This method called during the update phase of the life cycle
    bool
    onUpdate(const Ogre::FrameEvent &evt);
  protected:
    TexturePtr _stateTex; //gpu entities state texture
    TexturePtr _dirTex; //gpu entities direction texture.
    GPUEntities* _gpuEnts;
    Ogre::Timer _timer;
    auto_ptr<PingPongShaders> _posPingPong; //the ping pong to update positions.
    auto_ptr<PingPongShaders> _dirPingPong; //the ping pong to update directions.
    static const Real _DIRUPDATEPERIOD = 2.0; //two seconds per update of direction.
    Real _elapsedT;
    void
    init();

  };

}

#endif /* GPUENTSCONTROL_H_ */
