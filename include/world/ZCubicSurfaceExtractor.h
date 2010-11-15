#pragma warning(disable : 4503)
#pragma once
/**
* \file This file contains an implementation of a custom CubicSurfaceExtractor. 
*
**/
#include <vector>
#include <utility>
#include <Ogre.h>
#include <PolyVoxForwardDeclarations.h>
#include <Volume.h>
#include <VolumeSampler.h>
#include <PolyVoxImpl/TypeDef.h>
#include "world/WorldDefs.h"
namespace PolyVox
{
        template <typename VoxelType>
        class ZCubicSurfaceExtractor
        {
        public:
            ZCubicSurfaceExtractor(Volume<VoxelType>* volData, Region region, SurfaceMesh<PositionMaterial>* result);

            void execute();

        private:
            
            static const VoxelType AIR_BLOCK = 0; 
            
            enum AXIS
            {
                X = 0, Y, Z
            };
            enum FACE
            {
                MYFACE = 1, NOTMYFACE=0, NOFACE=-1
            };

            struct RLE_INFO
            {
                uint16_t faceCount;
                //uint8_t faceCount;
                VoxelType faceMaterial;
                FACE whichFace;
                //int isMyFace;
                //int faceMaterial;
            };

            //material id, count, face orientation (is my face).
            typedef std::pair<VoxelType, std::pair<size_t, FACE> > RLE;
            typedef std::vector<RLE> RLE_VEC;
            Volume<VoxelType>* _volData;
            //VolumeSampler<VoxelType> _sampVolume;

            SurfaceMesh<PositionMaterial>* _meshCurrent;

            Region _regSizeInVoxels;
            Region _regSizeInCells;

            void _resetParams(uint16_t x, uint16_t y, uint16_t z, FACE &whichFace,
                VoxelType &faceMaterial, AXIS xyz);

            void _markRLE(VoxelType &faceMaterial, VoxelType currentMaterial,
                VoxelType currentMaterialPlus, uint16_t &faceCount, FACE &previousWhichFace, 
                RLE_VEC& rleVec);
            void _finalizeRLE(VoxelType faceMaterial, uint16_t faceCount, 
                FACE previousWhichFace, RLE_VEC& rleVec);

            void _mergeFace(RLE_VEC &rleVec,
                uint16_t regX, uint16_t regY, uint16_t regZ,
                AXIS xyz);

            RLE_VEC rleXs[WORLD_BLOCK_WIDTH + 2]; //run length for x facing faces.
            RLE_INFO rleXsInfo[WORLD_BLOCK_WIDTH + 2];
           

            //RLE_VEC _fronRLE;
            //RLE_VEC _backRLE;

        };
}

#include "ZCubicSurfaceExtractor.inl"