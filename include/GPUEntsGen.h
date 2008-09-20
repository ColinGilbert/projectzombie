/*
 * GPUEntsLoader.h
 * This is a controller class which loads a bunch of GPU entities into a GPU entities container.
 *  Created on: Sep 18, 2008
 *      Author: bey0nd
 */

#ifndef GPUENTSLOADER_H_
#define GPUENTSLOADER_H_


#include <Ogre.h>

namespace ZGame
{
  class GPUEntities;
  class ZEntity;
  class GPUEntsGenProps;
  class GPUEntsGen
  {
  public:
    GPUEntsGen(ZEntity* ent,GPUEntities* input,GPUEntsGenProps* props);
    virtual ~GPUEntsGen();

    void build();

  protected:

    GPUEntities* _input;
    ZEntity* _ent;
    GPUEntsGenProps* _props;
    void genImposters();
    void genPosition();


  private:
  };
}


#endif /* GPUENTSLOADER_H_ */
