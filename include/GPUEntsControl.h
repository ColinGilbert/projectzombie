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
namespace ZGame
{
  class GPUEntities;
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
    //This method attachs the GPUEntities that this controller is going to control.
    void
    attachGPUEnts(GPUEntities* ents);
    //This method allows Life Cycle observers to be pushed
    void
    fillLfcObservers(LifeCycle::LifeCycleObserver &obs);

  };

}

#endif /* GPUENTSCONTROL_H_ */
