#ifndef ZENTITY_BUILDER_H
#define ZENTITY_BUILDER_H
/** \file This files defines the classes for a builder which will build ZEntities for the system.*/

#include <memory>
#include <Ogre.h>

#include "entities/EntitiesManager.h"
#include "delegates/EntityDelegates.h"

namespace ZGame
{
    namespace Entities
    {
        class ZEntity;

        class EntitiesBuilder
        {
        public:
            EntitiesBuilder(){}
            virtual ~EntitiesBuilder(){}
            /** \brief This method will build ZEntities through the Command and Control system.
            */
            static bool build(EntitiesManager* mgr, int numOfEnts);
            /** \brief This method will un-build the ZEntity. */
            static bool unbuild(ZEntity* zEnt);
        protected:

        };
    }
}


#endif