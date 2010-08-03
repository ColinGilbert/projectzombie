#ifndef _WORLDCONTROLLER_H
#define _WORLDCONTROLLER_H

#include <Ogre.h>
#include "Controller.h"

/**
*This class defines the world controller.
*
**/

namespace ZGame
{
    namespace World
    {
        class WorldMap;
        class WorldController
        {
        public:
            WorldController();
            ~WorldController();

            void loadWorld(); //temp.

            //implements the LifeCycleEvents
            bool onInit();
            bool onUpdate(const Ogre::FrameEvent &evt);
            bool onDestroy();

            /** \brief This method will return a world map. \note Do not call while WorldController has not be initialized.**/
            WorldMap* getWorldMap()
            {
                assert(_worldMap && "Class invariance failed. Trying to get a null WorldMap from WorldController.");
                return _worldMap;
            }

        protected:
            WorldMap* _worldMap;
        private:
            void init();
        private:
            Ogre::Entity* _bobEnt;
            Ogre::AnimationState* _animState;
        };
    }
}


#endif