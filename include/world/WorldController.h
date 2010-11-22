#ifndef _WORLDCONTROLLER_H
#define _WORLDCONTROLLER_H

#include <memory>
#include <Ogre.h>
#include <OgreTerrain.h>
#include <OgreTerrainGroup.h>
#include "Controller.h"
#include "world/WorldMap.h"
#include "world/DummyPageProvider.h"

/**
*This class defines the world controller.
*
**/

namespace ZGame
{
    namespace World
    {
        class VolumeMap;
        class VolumeMapPaging;
        struct WorldMapConfig;
        class PhysicsManager;
        class WorldController
        {
        public:
            WorldController();
            ~WorldController();

            //implements the LifeCycleEvents
            bool onInit();
            bool onUpdate(const Ogre::FrameEvent &evt);
            bool onDestroy();
            void addCube();

            /** \brief This method will return a world map. \note Do not call while WorldController has not be initialized.**/
            WorldMap* getWorldMap()
            {
                assert(_worldMap.get() && "Class invariance failed. Trying to get a null WorldMap from WorldController.");
                return _worldMap.get();
            }         

        protected:
            
        private:
            void _init();
            void _loadWorldMap(WorldMapConfig &config);
        private:
            std::auto_ptr<World::WorldMap > _worldMap;
            std::auto_ptr<VolumeMap > _volumeMap;
            VolumeMapPaging* _volumePaging;
            Ogre::SceneManager* _scnMgr;
            DummyPageProvider _pageProvider;
            Ogre::PageManager _pageManager;
            std::auto_ptr<PhysicsManager> _physicsMgr;
            Ogre::Camera* cam;
        
        };
    }
}


#endif