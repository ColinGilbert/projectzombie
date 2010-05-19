/*
* Imposter.h
*
*This class defines a state class for imposters.
*  Created on: Sep 11, 2008
*      Author: bey0nd
*/

#ifndef IMPOSTER_H_
#define IMPOSTER_H_

//#include <string>
#include <vector>
using namespace std;

#include <Ogre.h>

namespace ZGame
{
    namespace Entities
    {
        class ZEntityResource;
    }

    using Entities::ZEntityResource;

    class Imposter
    {
    public:
        static const size_t TEXDIM = 512;
        static const size_t SEGTHETA = 16;
        static const size_t SEGPHI = 16;


        Imposter(const ZEntityResource* const resource);
        ~Imposter();

        const Ogre::String getMeshName() const;
        Ogre::TexturePtr getTextures();//{return _texture;}

        void init();

        int getWidth() const;
        int getHeight() const ;
        int getDim() const;


    protected:
        Ogre::String _meshName;
        Ogre::TexturePtr _texture;

        void setupTextures();

    private:
        static int _id; //global unique id for imposter instances
        int _curId; //current id for this imposter

        const Ogre::String TYPE_NAME;


    };
}


#endif /* IMPOSTER_H_ */
