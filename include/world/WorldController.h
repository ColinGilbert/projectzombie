#ifndef _WORLDCONTROLLER_H
#define _WORLDCONTROLLER_H

#include <memory>
#include <Ogre.h>
#include <OgreTerrain.h>
#include <OgreTerrainGroup.h>
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

            

            //implements the LifeCycleEvents
            bool onInit();
            bool onUpdate(const Ogre::FrameEvent &evt);
            bool onDestroy();

            /** \brief This method will return a world map. \note Do not call while WorldController has not be initialized.**/
            WorldMap* getWorldMap()
            {
                assert(_worldMap.get() && "Class invariance failed. Trying to get a null WorldMap from WorldController.");
                return _worldMap.get();
            }

        protected:
            
        private:
            void init();
            void loadWorldMap();
        private:
            std::auto_ptr<WorldMap> _worldMap;
            Ogre::SceneManager* _scnMgr;
        
        };
    }
}


#endif