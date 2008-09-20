/*
 * GPUEntities.h
 *
 * This class defines a container for entities that run on the GPU.
 *  Created on: Sep 18, 2008
 *      Author: bey0nd
 */

#ifndef GPUENTITIES_H_
#define GPUENTITIES_H_
#include <Ogre.h>

namespace ZGame
{
  class GPUEntities
  {
  public:
    GPUEntities(Ogre::TexturePtr entsData,Ogre::TexturePtr imposterTex);
    virtual ~GPUEntities();
  protected:
    Ogre::TexturePtr _gpuEntsData;
    Ogre::TexturePtr _imposterTex;

  private:
  };
}

#endif /* GPUENTITIES_H_ */
