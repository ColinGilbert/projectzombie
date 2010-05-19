#ifndef ZENTITY_BUILDER_H
#define ZENTITY_BUILDER_H
/** \file This files defines the classes for a builder which will build ZEntities for the system.*/

#include <memory>
#include <Ogre.h>

#include "delegates/EntityDelegates.h"

namespace ZGame
{
    namespace Entities
    {
        class ZEntity;

        class EntitiesBuilder
        {
        public:
            /** \brief This method will build the ZEntity. 
            *
            * \note The read event here will eventually turn into delegate which will read from the lock-free message queue.
            */
            static bool build(ZEntity* zEnt, const EntityUpdateEvent &readEvent);
            /** \brief This method will un-build the ZEntity. */
            static bool unbuild(ZEntity* zEnt);
        protected:
            EntitiesBuilder(){}
            virtual ~EntitiesBuilder(){}
        };
    }
}


#endif