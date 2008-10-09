/*
 * GPUEntsView.h
 * This is the view class for GPUEnttities. This class provides only the view, and takes no responsible in
 * state storage logic (i.e we are going to use smart pointers here since we don't care about the STATE, this is the view
 * class).
 *  Created on: Sep 22, 2008
 *      Author: bey0nd
 */

#ifndef GPUENTSVIEW_H_
#define GPUENTSVIEW_H_
#include <string>
using namespace std;
#include <Ogre.h>
#include <OgreGpuProgram.h>
using namespace Ogre;
#include "ObsInjectors.h"

namespace ZGame
{
  class GPUEntities;
  namespace LifeCycle
  {
    struct LifeCycleObserver;
  }
  class GPUEntsView : public LFCObsInjector
  {
  public:
    GPUEntsView();
    virtual
    ~GPUEntsView();
    void
    attachGPUEnts(GPUEntities* ents);
    void
    alphaBlend();
    bool
    onUpdate(const Ogre::FrameEvent &evt);

    void
    fillLfcObservers(LifeCycle::LifeCycleObserver &obs);


  protected:
    GPUEntities* _ents;
    string _meshName;
    string _entsOgrEntName;
    string _entsOgrEntMatName;
    Ogre::Entity* _ogrEnt;
    bool _sceneAlphaBld;
    GpuProgramParametersSharedPtr _vertParam;
    Ogre::Camera* _cam;

    void
    init();
    void
    initGPUEntsMesh();
    void
    initOgrEnt();
    void
    initCamera();
    void
    createGPUEntsMesh();

  };

}
#endif /* GPUENTSVIEW_H_ */
