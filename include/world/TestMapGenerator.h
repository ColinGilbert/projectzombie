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
        private:
            void
                createSphereInVolume(Volume<uint8_t>& volData, float fRadius, uint8_t uValue);
            void
                createCubeInVolume(Volume<uint8_t>& volData, Vector3DUint16 lowerCorner, Vector3DUint16 upperCorner, uint8_t uValue);
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
    createSphereInVolume(*data, Ogre::Math::RangeRandom(width-2, width+16), Ogre::Math::RangeRandom(1.0, 256.0));
    return;
}

inline void 
    TestMapGenerator::createSphereInVolume(Volume<uint8_t>& volData, float fRadius, uint8_t uValue)
{
	//This vector hold the position of the center of the volume
	Vector3DFloat v3dVolCenter(volData.getWidth() / 2, volData.getHeight() / 2, volData.getDepth() / 2);

	//This three-level for loop iterates over every voxel in the volume
	for (int z = 0; z < volData.getWidth(); z++)
	{
		for (int y = 0; y < volData.getHeight(); y++)
		{
			for (int x = 0; x < volData.getDepth(); x++)
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
