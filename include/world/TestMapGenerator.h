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
                generate(PolyVox::Volume<uint8_t>* data, PolyVox::Region,
                Ogre::int32 pageX, Ogre::int32 pageY);
        private:
            void
                createSphereInVolume(Volume<uint8_t>& volData,  PolyVox::Region &region, float fRadius, uint8_t uValue);
            void
                createCubeInVolume(Volume<uint8_t>& volData, Vector3DUint16 lowerCorner, Vector3DUint16 upperCorner, uint8_t uValue);
        };
    }
}

using namespace ZGame::World;
using namespace PolyVox;
inline void 
    TestMapGenerator::generate(Volume<uint8_t>* data, PolyVox::Region region,
    Ogre::int32 pageX, Ogre::int32 pageY)
{
    
    const int width = region.width();
    const int height = region.height() - 10;
    const int depth = region.depth();
    const float halfHeight = (float)(height) / 2.0f;
    const float oceanFloor = halfHeight;
    createSphereInVolume(*data, region, width - 15, 
        //Ogre::Math::RangeRandom(width-15, width), 
        Ogre::Math::RangeRandom(1.0, 256.0));
    return;
}

inline void 
    TestMapGenerator::createSphereInVolume(Volume<uint8_t>& volData, PolyVox::Region &region,
    float fRadius, uint8_t uValue)
{
	//This vector hold the position of the center of the volume
    Vector3DFloat v3dVolCenter(region.getLowerCorner().getX() + region.depth() / 2, 
        region.getLowerCorner().getY() + region.height() / 2, region.getLowerCorner().getZ() + region.width() / 2);

	//This three-level for loop iterates over every voxel in the volume
    for (int z = region.getLowerCorner().getZ(); z < region.getUpperCorner().getZ(); z++)
	{
		for (int y = region.getLowerCorner().getY(); y < region.getUpperCorner().getY(); y++)
		{
			for (int x = region.getLowerCorner().getX(); x < region.getUpperCorner().getX(); x++)
			{
				//Store our current position as a vector...
				Vector3DFloat v3dCurrentPos(x,y,z);	
				//And compute how far the current position is from the center of the volume
				float fDistToCenter = (v3dCurrentPos - v3dVolCenter).length();

				//If the current voxel is less than 'radius' units from the center
				//then we make it solid, otherwise we make it empty space.
				if(fDistToCenter <= fRadius)
				{
					volData.setVoxelAt(x,y,z, uValue);
				}
			}
		}
	}
}

inline void 
    TestMapGenerator::createCubeInVolume(Volume<uint8_t>& volData, Vector3DUint16 lowerCorner, Vector3DUint16 upperCorner, uint8_t uValue)
{
	//This three-level for loop iterates over every voxel between the specified corners
	for (int z = lowerCorner.getZ(); z <= upperCorner.getZ(); z++)
	{
		for (int y = lowerCorner.getY(); y <= upperCorner.getY(); y++)
		{
			for (int x = lowerCorner.getX() ; x <= upperCorner.getX(); x++)
			{
				volData.setVoxelAt(x,y,z, uValue);
			}
		}
	}
}
