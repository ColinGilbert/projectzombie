/*
 * GPUEntsView.h
 *
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

namespace ZGame
{
  class GPUEntities;
  namespace LifeCycle
  {
    struct LifeCycleObserver;
  }
  class GPUEntsView
  {
    public:
      GPUEntsView();
      virtual ~GPUEntsView();
      void attachGPUEnts(GPUEntities* ents);
      void alphaBlend();
      bool onUpdate(const Ogre::FrameEvent &evt);

      void fillLfcObservers(LifeCycle::LifeCycleObserver &obs);

      GPUEntities* _ents;
    protected:
      string _meshName;
      string _entsOgrEntName;
      string _entsOgrEntMatName;
      Ogre::Entity* _ogrEnt;
      bool _sceneAlphaBld;
      GpuProgramParametersSharedPtr _vertParam;

      void init();
      void initOgrEnt();
      void createGPUEntsMesh();

  };

}
#endif /* GPUENTSVIEW_H_ */
