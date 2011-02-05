#pragma once

#include "ZPrerequisites.h"
namespace ZGame
{
    namespace World
    {
        class MapGenerator
        {
        public:
            virtual ~MapGenerator(){}
            virtual void generate(PVolume* data, PolyVox::Region region,
                Ogre::Real pageX, Ogre::Real pageY) = 0;
            
        protected:
            MapGenerator(){}
        };
    }
}