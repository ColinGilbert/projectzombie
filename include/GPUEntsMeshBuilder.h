/*
 * GPUEntsMeshBuilder.h
 *
 *  Created on: Sep 23, 2008
 *      Author: bey0nd
 */

#ifndef GPUENTSMESHBUILDER_H_
#define GPUENTSMESHBUILDER_H_
//#include <string>
//using namespace std;
#include <Ogre.h>

namespace ZGame
{

  class GPUEntsGenProps;
  class GPUEntsMeshBuilder
  {
  public:
      static Ogre::MeshPtr build(const Ogre::String name, GPUEntsGenProps* props);
  protected:
    GPUEntsMeshBuilder();
    virtual
    ~GPUEntsMeshBuilder();
  };

}

#endif /* GPUENTSMESHBUILDER_H_ */
