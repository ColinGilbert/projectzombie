#ifndef _RENDER_ENTITIES_MANAGER_H
#define _RENDER_ENTITIES_MANAGER_H
/**
* \file This file will define class for functions which manages Render Entities.
*/

#include <vector>

using std::vector;

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
            RenderEntitiesManager * 
                getManager()
            {
                return this;
            }
            /**
            *This method will create a RenderEntity in the system. This method is hooked to the Command and Control system. Therefore we will register it to the CommandController.
            *
            *\note The method take in pointers; this is fine because one the thread system is in place, the upstream CommandController will have copied the needed parameters from the
            *message stream. Thus these variables will be defined in the local thread, and will be taken care of upstream by the CommandController.
            
            bool 
                createRenderEntity(const ZEntityResource* const res, const EntityUpdateEvent* const read);
            */
            bool
                createRenderEntity(ZEntity const* ent);
        protected:
        private:
            //typedef vector<Ogre::SceneNode*> ENT_NODES_VEC;
            //typedef ENT_NODES_VEC::iterator ENT_NODES_ITER;

            /** This vector stores SceneNode pointers. Care must be taken to properly deallocate them such as when restarting,
            this RenderEntitiesMananger. Otherwise we can expect SceneManager to take care of deallocation for us, on shutdown.**/
            //ENT_NODES_VEC _entNodes;
            Ogre::SceneNode* _entNodesRoot;
            Ogre::SceneManager* _scnMgr;


        };
    }
}


#endif