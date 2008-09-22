/*
 * GPUEntities.h
 *
 * This class defines a container for entities that run on the GPU.
 *  Created on: Sep 18, 2008
 *      Author: bey0nd
 */

#ifndef GPUENTITIES_H_
#define GPUENTITIES_H_
#include <string>
using namespace std;
#include <Ogre.h>

namespace ZGame
{
  class Imposter;
  class GPUEntities
  {
  public:
    GPUEntities(const string entsName,Ogre::TexturePtr entsData,Ogre::TexturePtr imposterTex);
    virtual ~GPUEntities();
    const string getName();
    void setEntsData(const Ogre::TexturePtr &texptr);
    void setImposterTex(const Ogre::TexturePtr &texptr);
  protected:
    Ogre::TexturePtr _gpuEntsData;
    Ogre::TexturePtr _imposterTex;
    Imposter* _imposter;
    string _entsName;
  private:
  };
}

#endif /* GPUENTITIES_H_ */
