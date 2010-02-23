/*
* GPUEntities.h
*
* This class defines a container for entities that run on the GPU.
*  Created on: Sep 18, 2008
*      Author: bey0nd
*/

#ifndef GPUENTITIES_H_
#define GPUENTITIES_H_
//#include <string>
#include <memory>

#include <Ogre.h>

namespace ZGame
{
    class Imposter;
    class GPUEntsGenProps;
    class GPUEntities
    {
    public:
        GPUEntities(const Ogre::String entsName, const Ogre::String entsData,
            const Ogre::String dirData, const Ogre::String imposterTex, std::auto_ptr<
            GPUEntsGenProps> props);
        virtual
            ~GPUEntities();
        const Ogre::String
            getName();
        void
            setEntsData(const Ogre::String texName);
        const Ogre::String
            getEntsData();
        void
            setImposterTex(const Ogre::String texName);
        const Ogre::String
            getImposterTex();
        GPUEntsGenProps*
            getProperties();
        const Ogre::String
            getGpuEntsDirData();
    protected:
        Ogre::String _gpuEntsData;
        Ogre::String _imposterTex;
        Ogre::String _entsName;
        std::auto_ptr<GPUEntsGenProps> _props;
        Ogre::String _dirData;
    private:
    };
}

#endif /* GPUENTITIES_H_ */
