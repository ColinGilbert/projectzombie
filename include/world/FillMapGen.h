#pragma once

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

#include "ZPrerequisites.h"

namespace ZGame
{
    namespace World
    {
        /**
        * This class is designed as a generator that will fill a PolyVox::Region with a val.
        **/
        class FillMapGen
        {
        public:
            FillMapGen(){}
            ~FillMapGen(){}

            void
                generate(PVolume* data, PolyVox::Region &region, uint8_t val);
        };

        class FillMapPositionGen
        {
        public:
            FillMapPositionGen(){}
            ~FillMapPositionGen(){}

            void
                generate(PVolume* data, PolyVox::Region &region, std::vector<Ogre::Vector3>* positions,
                const Ogre::Vector3 &worldOrigin);
        };

    }
}

using namespace ZGame::World;
using namespace PolyVox;

inline void 
    FillMapGen::generate(PVolume* data, PolyVox::Region &region, uint8_t val)
{
    for(uint16_t z = region.getLowerCorner().getZ(); z < region.getUpperCorner().getZ()+1; ++z)
    {
        for(uint16_t y = region.getLowerCorner().getY(); y < region.getUpperCorner().getY()+1; ++y)
        {
            for(uint16_t x = region.getLowerCorner().getX(); x < region.getUpperCorner().getX()+1; ++x)
            {
                data->setVoxelAt(x, y, z, val);
            }
        }
    }
}

void
    FillMapPositionGen::generate(PVolume* data, PolyVox::Region &region, std::vector<Ogre::Vector3>* positions,
    const Ogre::Vector3 &worldOrigin)
{
    for(uint16_t z = region.getLowerCorner().getZ(); z < region.getUpperCorner().getZ() + 1; ++z)
    {
        for(uint16_t x = region.getLowerCorner().getX(); x < region.getUpperCorner().getX() + 1; ++x)
        {
            bool found = false;
            uint16_t minHeight;
            //find the max Y per colum.
            for(size_t y = region.getLowerCorner().getY(); y < region.getUpperCorner().getY() + 1; ++y)
            {
                PolyVox::Material8 mat = data->getVoxelAt(x, y, z);
                if(mat.getDensity() == mat.getMinDensity())
                {
                    found = true;
                    minHeight = y;
                    break;
                }
            }
            if(found)
            {
                Ogre::Vector3 pos(x, minHeight, z);
                positions->push_back(worldOrigin + pos);
            }
        }
    }
}

