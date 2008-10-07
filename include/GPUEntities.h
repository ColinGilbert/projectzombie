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
    GPUEntities(const string entsName, const string entsData,
        const string dirData, const string imposterTex, auto_ptr<
            GPUEntsGenProps> props);
    virtual
    ~GPUEntities();
    const string
    getName();
    void
    setEntsData(const string texName);
    const string
    getEntsData()
    {
      return _gpuEntsData;
    }
    void
    setImposterTex(const string texName);
    const string
    getImposterTex()
    {
      return _imposterTex;
    }
    GPUEntsGenProps*
    getProperties()
    {
      return _props.get();
    }
    string
    getGpuEntsDirData()
    {
      return _dirData;
    }
  protected:
    string _gpuEntsData;
    string _imposterTex;
    string _entsName;
    auto_ptr<GPUEntsGenProps> _props;
    string _dirData;
  private:
  };
}

#endif /* GPUENTITIES_H_ */
