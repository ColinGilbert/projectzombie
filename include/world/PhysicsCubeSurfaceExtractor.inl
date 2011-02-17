#include <Array.h>
#include <MaterialDensityPair.h>
#include <SurfaceMesh.h>
#include <PolyVoxImpl/MarchingCubesTables.h>
#include <VertexTypes.h>
#include "world/PhysicsManager.h"
#include <Shapes/OgreBulletCollisionsCompoundShape.h>

//namespace World
//{
template <typename VoxelType>
ZGame::World::PhysicsCubeSurfaceExtractor<VoxelType>::PhysicsCubeSurfaceExtractor(Volume<VoxelType>* volData, Region region,
    ZGame::World::PhysicsManager* phyMgr) : _volData(volData), _regSizeInVoxels(region), _phyMgr(phyMgr)
{
}

template <typename VoxelType>
void ZGame::World::PhysicsCubeSurfaceExtractor<VoxelType>::execute(OgreBulletCollisions::CompoundCollisionShape* compoundShape,
    const Ogre::Vector3& worldOrigin)
{
    using namespace OgreBulletCollisions;
    using namespace OgreBulletDynamics;

    for(uint16_t z = _regSizeInVoxels.getLowerCorner().getZ(); z < _regSizeInVoxels.getUpperCorner().getZ(); ++z)
    {
        for(uint16_t y = _regSizeInVoxels.getLowerCorner().getY(); y < _regSizeInVoxels.getUpperCorner().getY(); ++y)
        {
            for(uint16_t x = _regSizeInVoxels.getLowerCorner().getX(); x < _regSizeInVoxels.getUpperCorner().getX(); ++x)
            {
                VoxelType currentMaterial = _volData->getVoxelAt(x, y, z);
                VoxelType plusMaterial = _volData->getVoxelAt(x + 1, y, z);
                if(currentMaterial > plusMaterial)
                {
                    //output cube
                    _phyMgr->addCubeToCompoundShape(worldOrigin + Ogre::Vector3(x, y, z), compoundShape);
                }

            }
        }
    }
}

//}
