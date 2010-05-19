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

#include "entities/ZEntityResource.h"


namespace ZGame
{
    namespace Entities
    {
        using OpenSteer::SimpleVehicle;
        class ZEntity : public SimpleVehicle
        {
        public:
            ZEntity(const Ogre::String &entName, const Ogre::String &meshName);
            ZEntity(const Ogre::String &entName, const ZEntityResource &res);
            virtual ~ZEntity();

            void setEntityName(const Ogre::String entName);
            const Ogre::String getEntityName();

            const ZEntityResource* const getResource() const
            {
                return &_resource;
            }



        protected:
        private:
            Ogre::String _entName;
            ZEntityResource _resource;

            void createEntity();

        };
    }
}

#endif /* ZENTITY_H_ */
