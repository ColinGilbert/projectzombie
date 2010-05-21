#ifndef _RENDER_ENTITIES_MANAGER_H
#define _RENDER_ENTITIES_MANAGER_H
/**
* \file This file will define class for functions which manages Render Entities.
*/
#include <Ogre.h>
#include "delegates/EntityDelegates.h"
namespace ZGame
{

    namespace Entities
    {
        /**
        *This class is a Manager for managing Render Entities. Render Entities represent an Entity on the rendering side.
        */
        class RenderEntitiesManager
        {
        public:
            RenderEntitiesManager();
            virtual 
                ~RenderEntitiesManager();
            /**
            *This method will create a RenderEntity in the system. This method is hooked to the Command and Control system. Therefore we will register it to the CommandController.
            *
            *\note The method take in pointers; this is fine because one the thread system is in place, the upstream CommandController will have copied the needed parameters from the
            *message stream. Thus these variables will be defined in the local thread, and will be taken care of upstream by the CommandController.
            */
            bool 
                createRenderEntity(const ZEntityResource* const res, const EntityUpdateEvent* const read);

        protected:
        private:

        };
    }
}


#endif