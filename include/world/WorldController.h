/**
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
**/
/**
*author: beyzend 
*email: llwijk@gmail.com
**/

#pragma once

#include <memory>
#include <Ogre.h>
#include <OgreTerrain.h>
#include <OgreTerrainGroup.h>
#include "Controller.h"
#include "world/WorldMap.h"
#include "world/DummyPageProvider.h"
#include "ZInitPacket.h"
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
            bool onInit(ZGame::ZInitPacket initPacket);
            bool onUpdate(const Ogre::FrameEvent &evt);
            bool onDestroy();
            // Temp method for testing
            void addBlock(Ogre::Real cursorX, Ogre::Real cursorY);
            void removeBlock(Ogre::Real cursorX, Ogre::Real cursorY);

            /** \brief This method will return a world map. \note Do not call while WorldController has not be initialized.**/
            WorldMap* getWorldMap()
            {
                assert(_worldMap.get() && "Class invariance failed. Trying to get a null WorldMap from WorldController.");
                return _worldMap.get();
            }         

        protected:
            
        private:
            void _init(ZGame::ZInitPacket packet);
            void _loadWorldMap(WorldMapConfig &config);
        private:
            std::auto_ptr<World::WorldMap > _worldMap;
            std::auto_ptr<VolumeMap > _volumeMap;
            VolumeMapPaging* _volumePaging;
            Ogre::SceneManager* _scnMgr;
            DummyPageProvider _pageProvider;
            Ogre::PageManager _pageManager;
            std::auto_ptr<PhysicsManager> _physicsMgr;
            Ogre::Camera* _cam;


          

        };
    }
}


