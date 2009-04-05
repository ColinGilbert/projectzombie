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

#include <Ogre.h>

namespace ZGame
{
  class Imposter;
  class GPUEntsGenProps;
  class GPUEntities
  {
  public:
	  GPUEntities(const std::string entsName, const std::string entsData,
		  const std::string dirData, const std::string imposterTex, std::auto_ptr<
            GPUEntsGenProps> props);
    virtual
    ~GPUEntities();
	const std::string
    getName();
    void
	setEntsData(const std::string texName);
	const std::string
    getEntsData();
    void
	setImposterTex(const std::string texName);
	const std::string
    getImposterTex();
    GPUEntsGenProps*
    getProperties();
	const std::string
    getGpuEntsDirData();
  protected:
	std::string _gpuEntsData;
	std::string _imposterTex;
    std::string _entsName;
	std::auto_ptr<GPUEntsGenProps> _props;
    std::string _dirData;
  private:
  };
}

#endif /* GPUENTITIES_H_ */
