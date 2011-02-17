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
/**
* \file This file contains an implementation of a custom CubicSurfaceExtractor. 
*
**/
#include "ZPrerequisites.h"
#include <PolyVoxForwardDeclarations.h>
#include <Volume.h>
#include <VolumeSampler.h>
#include <PolyVoxImpl/TypeDef.h>
#include "world/WorldDefs.h"
namespace PolyVox
{
        template <typename VoxelType>
        /**
        *\brief This class is a custom class for extracting a cubic surface from PolyVox Volumes using Run Length Encoding. 
        *
        *This class takes a PolyVox volume to generate a 1D run length encoding over the voxels while iterating over Z slices (constant Z per slice). For each
        *slice update, it uses this RLE information to merge faces; for Z and Y facing faces, it merges per iteration of Y; for X facing faces, it merges per
        *iteration of Z (since X facing faces' run length are computed in the Y direction as per iteration over Z slices).
        **/
        class ZCubicSurfaceExtractor
        {
        public:
            ZCubicSurfaceExtractor(Volume<VoxelType>* volData, Region region, SurfaceMesh<PositionMaterial>* result);

            void execute();

        private:
            
            //static const VoxelType AIR_BLOCK = 0; 
            VoxelType AIR_BLOCK;
            enum AXIS
            {
                X = 0, Y, Z
            };
            enum FACE
            {
                MYFACE = 1, NOTMYFACE=0, NOFACE=-1
            };

            struct RLE_VOXEL
            {
                RLE_VOXEL(VoxelType mat, uint16_t len, FACE which)
                    : material(mat), length(len), whichFace(which) 
                {
                }
                VoxelType material;
                uint16_t length;
                FACE whichFace;
            };

            struct RLE_INFO
            {
                uint16_t faceCount;
                VoxelType faceMaterial;
                FACE whichFace;
            };

            //material id, count, face orientation.
            //typedef std::pair<VoxelType, std::pair<size_t, FACE> > RLE;
            typedef std::vector<RLE_VOXEL> RLE_VEC;
            Volume<VoxelType>* _volData;
            //VolumeSampler<VoxelType> _sampVolume;

            SurfaceMesh<PositionMaterial>* _meshCurrent;

            Region _regSizeInVoxels;
            Region _regSizeInCells;

            void _resetParams(int16_t x, int16_t y, int16_t z, FACE &whichFace,
                VoxelType &faceMaterial, AXIS xyz);

            void _markRLE(VoxelType &faceMaterial, VoxelType currentMaterial,
                VoxelType currentMaterialPlus, uint16_t &faceCount, FACE &previousWhichFace, 
                RLE_VEC& rleVec);
            void _finalizeRLE(VoxelType faceMaterial, uint16_t faceCount, 
                FACE previousWhichFace, RLE_VEC& rleVec);

            void _mergeFace(RLE_VEC &rleVec,
                int16_t regX, int16_t regY, int16_t regZ,
                AXIS xyz);
        };
}

#include "ZCubicSurfaceExtractor.inl"