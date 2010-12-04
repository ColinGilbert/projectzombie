#pragma once
#include <iostream>
#include <Ogre.h>
#include <MaterialDensityPair.h>
#include <Volume.h>

namespace ZGame
{
    namespace World
    {
        class MapGenerator
        {
        public:
            virtual ~MapGenerator(){}
            virtual void generate(PolyVox::UInt8Volume* data, PolyVox::Region region,
                Ogre::Real pageX, Ogre::Real pageY) = 0;
            
        protected:
            MapGenerator(){}
        };
    }
}