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

#include "ZPrerequisites.h"
#include "world/DummyPageProvider.h"

/**
*This class defines the world controller.
*
**/

namespace ZGame
{
    namespace World
    {
        
        class WorldController : Ogre::Viewport::Listener
        {
        public:
            WorldController();
            ~WorldController();

            //implements the LifeCycleEvents
            bool onInit(ZGame::ZInitPacket *initPacket);
            bool onUpdate(const Ogre::FrameEvent &evt);
            bool onDestroy();
            // Temp method for testing
            void addBlock(Ogre::Real cursorX, Ogre::Real cursorY);
            void removeBlock(Ogre::Real cursorX, Ogre::Real cursorY);
            void getCursor3dPosition(Ogre::Ray rayTo, Ogre::Vector3& position,
                Ogre::Real searchDistance);
            void
                getCursor3dPosition(Ogre::Ray rayTo, Ogre::Vector3& position,
                const Ogre::Plane &constraintPlane);

            /** \brief This method will be called with a selection region specified by the extents.**/
            void
                onSelectionRegion(const Ogre::AxisAlignedBox &selectBox, size_t materialId);

            void
                setWorldConfiguration(std::auto_ptr<WorldConfig> worldConfig);

            //Viewport listener
            virtual void
                viewportCameraChanged(Ogre::Viewport* viewport);
            virtual void
                viewportDimensionsChanged(Ogre::Viewport* viewport){}
            virtual void
                viewportDestroyed(Ogre::Viewport* viewport){}

            /**
            *This method will add positions from the volume with respect to the given region
            *as defined by the AxisAlignedBox. The positions are gathered in such a way it represents
            *a 2D height field of said region. That is, it returns the positions with the highest Y value
            *per 2D integer points in the Volume as defined by the box--a height map.
            *
            **/
            void
                getVolumeRegionPositionsHeightField(const Ogre::AxisAlignedBox &box,
                std::vector<Ogre::Vector3> &positions);


        protected:
            
        private:
            void _init(ZGame::ZInitPacket *packet);
            void _loadWorldMap(WorldMapConfig &config);
        private:
            std::auto_ptr<VolumeMap > _volumeMap;
            VolumeMapPaging* _volumePaging;
            Ogre::SceneManager* _scnMgr;
            DummyPageProvider _pageProvider;
            Ogre::PageManager _pageManager;
            std::auto_ptr<PhysicsManager> _physicsMgr;
            Ogre::Camera* _cam;
            Ogre::Viewport* _viewport;
            std::auto_ptr<WorldConfig> _worldConfig;

        };
    }
}


