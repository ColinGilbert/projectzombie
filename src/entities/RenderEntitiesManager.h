#ifndef _RENDER_ENTITIES_MANAGER_H
#define _RENDER_ENTITIES_MANAGER_H
/**
* \file This file will define class for functions which manages Render Entities.
*/

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

            void createRenderEntity();

        protected:
        private:

        };
    }
}


#endif