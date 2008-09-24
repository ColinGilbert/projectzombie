/*
 * GPUEntsView.h
 *
 *  Created on: Sep 22, 2008
 *      Author: bey0nd
 */

#ifndef GPUENTSVIEW_H_
#define GPUENTSVIEW_H_
namespace ZGame
{
  class GPUEntities;
  class GPUEntsView
  {
    public:
      GPUEntsView();
      virtual ~GPUEntsView();
      void init();
      void attachGPUEnts(GPUEntities* ents);
      GPUEntities* _ents;
    protected:
      void createGPUEntsMesh();
  };

}
#endif /* GPUENTSVIEW_H_ */
