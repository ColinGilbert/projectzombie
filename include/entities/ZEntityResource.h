#ifndef _ZENTITY_RESOURCE_H
#define _ZENTITY_RESOURCE_H

#include <Ogre.h>


namespace ZGame
{
    namespace Entities
    {
        /** This class represent the resource oriented aspect of ZEntity.*/
        class ZEntityResource
        {
        public:
            ZEntityResource(){}
            ZEntityResource(const Ogre::String &resName);
            virtual ~ZEntityResource();
           
            const Ogre::String& getResourceName() const
            {
                return _resourceName;
            }

        protected:
        private:
            Ogre::String _resourceName;
            
        };
    }
}


#endif