/*
 * GPUEntsControl.h
 * This class is responsible for controlling the GPU entities's simulation.
 *  Created on: Oct 4, 2008
 *      Author: bey0nd
 */

#ifndef GPUENTSCONTROL_H_
#define GPUENTSCONTROL_H_
//#include <string>
#include <iostream>
#include <memory>
#include <Ogre.h>
namespace ZGame
{
  class GPUEntities;
  class PingPongShaders;
  namespace LifeCycle
  {
    struct LifeCycleObserver;
  }
  class GPUEntsControl
  {
  public:
    GPUEntsControl();
    virtual
    ~GPUEntsControl();
    //This method attaches the GPUEntities that this controller is going to control.
    void
    attachGPUEnts(GPUEntities* ents);
    //This method allows Life Cycle observers functors to be pushed to us.
    //Life Cycle methods
    //This method called during initialization phase of the life cycle.
    bool
    onInit();
    //This method called during the update phase of the life cycle
    bool
    onUpdate(const Ogre::FrameEvent &evt);

    bool
    onDestroy()
    {

    }

  protected:
	  Ogre::TexturePtr _stateTex; //gpu entities state texture
	  Ogre::TexturePtr _dirTex; //gpu entities direction texture.
    GPUEntities* _gpuEnts;
    Ogre::Timer _timer;
	  std::auto_ptr<PingPongShaders> _posPingPong; //the ping pong to update positions.
	  std::auto_ptr<PingPongShaders> _dirPingPong; //the ping pong to update directions.
	  static const Ogre::Real _DIRUPDATEPERIOD; //= 2.0; //two seconds per update of direction.
	  Ogre::Real _elapsedT;
    void
    init();
  };

}

#endif /* GPUENTSCONTROL_H_ */
