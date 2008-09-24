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
#include <memory>
using namespace std;
#include <Ogre.h>

namespace ZGame
{
  class Imposter;
  class GPUEntsGenProps;
  class GPUEntities
  {
  public:
    GPUEntities(const string entsName,const string entsData,const string imposterTex,
        auto_ptr<GPUEntsGenProps> props);
    virtual ~GPUEntities();
    const string getName();
    void setEntsData(const string texName);
    void setImposterTex(const string texName);
    GPUEntsGenProps* getProperties() {return _props.get();}
  protected:
    string _gpuEntsData;
    string _imposterTex;
    string _entsName;
    auto_ptr<GPUEntsGenProps> _props;
  private:
  };
}

#endif /* GPUENTITIES_H_ */
