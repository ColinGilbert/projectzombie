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
#include "Ogre.h"
namespace ZGame
{
  class GPUEntities;
  class GPUEntsView
  {
    public:
      GPUEntsView();
      virtual ~GPUEntsView();
      void attachGPUEnts(GPUEntities* ents);
      void alphaBlend();
      GPUEntities* _ents;
    protected:
      string _meshName;
      string _entsOgrEntName;
      string _entsOgrEntMatName;
      Ogre::Entity* _ogrEnt;
      bool _sceneAlphaBld;


      void init();
      void initOgrEnt();
      void createGPUEntsMesh();
  };

}
#endif /* GPUENTSVIEW_H_ */
