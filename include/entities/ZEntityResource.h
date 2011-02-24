#ifndef _ZENTITY_RESOURCE_H
#define _ZENTITY_RESOURCE_H

#include <Ogre.h>
#include "entities/EntitiesDefs.h"

namespace ZGame
{
    namespace Entities
    {
        typedef Ogre::String ZENT_KEY;
        /** This class represent the resource oriented aspect of ZEntity.
        *
        * \note Notice we defined a key. This obviously raises the questions such as what is an EMPTY KEY? This is an issue as you see we allow for a default constructor. So this means
        * we will need default values for key. Here, the default value will simply be an empty Ogre::String as that is what we defined the key to be of as of now.
        */
        class ZEntityResource
        {
        public:
            static const ZENT_KEY NULL_KEY;
            static bool IsNullKey(const ZENT_KEY &key)
            {
                return NULL_KEY == key;
            }
            ZEntityResource(){}
            ZEntityResource(const ZENT_KEY &key, const Ogre::String &resName);

            virtual ~ZEntityResource();

            const Ogre::String& getResourceName() const
            {
                return _resourceName;
            }

            const ZENT_KEY& getKey() const
            {
                return _key;
            }

        protected:
        private:
            ZENT_KEY _key;
            Ogre::String _resourceName;         
        };
    }
}


#endif