/*
 * GPUEntsLoader.h
 * This is a controller class which loads a bunch of GPU entities into a GPU entities container.
 *  Created on: Sep 18, 2008
 *      Author: bey0nd
 */

#ifndef GPUENTSLOADER_H_
#define GPUENTSLOADER_H_


#include <Ogre.h>
#include "GPUEntities.h"

namespace ZGame
{
  class GPUEntsGen
  {
  public:
    GPUEntsGen();
    virtual ~GPUEntsGen();

    void setInput(GPUEntities* input) {_input = input;}

    void build();

  protected:

    GPUEntities* _input;

  private:
  };
}


#endif /* GPUENTSLOADER_H_ */
