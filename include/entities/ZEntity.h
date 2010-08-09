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

#include "EntitiesDefs.h"
#include "entities/ZEntityResource.h"


namespace ZGame
{
    namespace Entities
    {
        using OpenSteer::SimpleVehicle;
        class ZEntity //: public SimpleVehicle
        {
        public:
            ZEntity(const ZENT_KEY &key, const Ogre::String &meshName);
            ZEntity(const ZEntityResource &res);
            virtual ~ZEntity();

            //void setEntityName(const Ogre::String &entName);
            const ZENT_KEY getEntityName() const;

            ZEntityResource const* const getResource() const
            {
                return &_resource;
            }
            /** \brief This function is called on read events. */
            bool
                onRead(const Ogre::Vector3 &pos, const Ogre::Quaternion &orient);
            /** \brief This function is called on Write events. */
            bool
                onWrite(Ogre::Vector3 &pos, Ogre::Quaternion &orient) const; 
            const Ogre::Vector3& getPosition() const
            {
                return _worldPos;
            }
            const Ogre::Quaternion& getOrientation() const
            {
                return _worldOrient;
            }

        protected:
        private:
            ZENT_KEY _entKey;
            ZEntityResource _resource;
            Ogre::Vector3 _worldPos;
            Ogre::Quaternion _worldOrient;
        };
    }
}

#endif /* ZENTITY_H_ */
