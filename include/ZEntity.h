/*
* ZEntity.h
*
*  Created on: Sep 18, 2008
*      Author: bey0nd
*/

#ifndef ZENTITY_H_
#define ZENTITY_H_

//#include <string>

//using namespace std;
#include <Ogre.h>

#include <OpenSteer/SimpleVehicle.h>

namespace ZGame
{
    namespace Entities
    {
        class ZEntity //: public SimpleVehicle
        {
        public:
            ZEntity(const Ogre::String &entName,const Ogre::String &meshName);
            virtual ~ZEntity();

            void setEntityName(const Ogre::String entName);
            void setMeshName(const Ogre::String meshName);
            const Ogre::String getEntityName();
            const Ogre::String getMeshName();


        protected:
        private:
            Ogre::String _entName;
            Ogre::String _meshName;

        };
    }
}

#endif /* ZENTITY_H_ */
