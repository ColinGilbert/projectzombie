#pragma once
#include "world/MapGenerator.h"
namespace ZGame
{
    namespace World
    {
        class TestMapGenerator : public MapGenerator
        {
        public:
            ~TestMapGenerator(){}
            void
                generate(PolyVox::Volume<uint8_t>* data,
                Ogre::int32 pageX, Ogre::int32 pageY);
        };
    }
}

using namespace ZGame::World;
using namespace PolyVox;
inline void 
    TestMapGenerator::generate(Volume<uint8_t>* data,
    Ogre::int32 pageX, Ogre::int32 pageY)
{
    const int width = data->getWidth();
    const int height = data->getHeight() - 128;
    const int depth = data->getDepth();
    const float halfHeight = (float)(height) / 2.0f;
    const float oceanFloor = halfHeight;
    for(size_t z=0; z < width; z++)
    {
        for(size_t y = 0; y < height; y++)
        {
            for(size_t x = 0; x < depth; x++)
            {
                if(y < oceanFloor)
                {
                    data->setVoxelAt(x, y, z, 2);
                }
                else if(y == oceanFloor)
                {
                    data->setVoxelAt(x, y, z, 206);
                }
                else
                {
                    if(x > 10 && x < 20 && y < 80)
                    {
                        data->setVoxelAt(x, y, z, 2);
                    }
                    else
                    {
                    data->setVoxelAt(x, y, z, 0);
                    }
                }
            }
        }
    }
    return;
}