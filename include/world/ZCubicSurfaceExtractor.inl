
#include <iostream>
using std::cout;
using std::endl;
#include <Array.h>
#include <MaterialDensityPair.h>
#include <SurfaceMesh.h>
#include <PolyVoxImpl/MarchingCubesTables.h>
#include <VertexTypes.h>
#include "world/WorldDefs.h"

namespace PolyVox
{
    template <typename VoxelType>
    ZCubicSurfaceExtractor<VoxelType>::ZCubicSurfaceExtractor(Volume<VoxelType>* volData, Region region,
        SurfaceMesh<PositionMaterial>* result) : _volData(volData),
        //_sampVolume(volData),
        _regSizeInVoxels(region),
        _meshCurrent(result)
    {
        //_frontRLE.reserve(16);
        //_backRLE.reserve(16);
        _regSizeInVoxels.cropTo(_volData->getEnclosingRegion());
        _regSizeInCells = _regSizeInVoxels;
        _regSizeInCells.setUpperCorner(_regSizeInCells.getUpperCorner() - Vector3DInt16(1, 1, 1));
        _meshCurrent->clear();
    }

    template <typename VoxelType>
    inline void ZCubicSurfaceExtractor<VoxelType>::_resetParams(uint16_t x, uint16_t y, uint16_t z,
        FACE &whichFace, VoxelType &faceMaterial, AXIS xyz)
    {
        faceMaterial = _volData->getVoxelAt(x, y, z);
        VoxelType plusMaterial;

        switch(xyz)
        {
        case Z:
            plusMaterial = _volData->getVoxelAt(x, y, z+1);
            break;
        case Y:
            plusMaterial = _volData->getVoxelAt(x, y+1, z);
            break;
        case X:
            plusMaterial = _volData->getVoxelAt(x+1, y, z);
            break;    
        }
        bool A = faceMaterial > AIR_BLOCK;
        bool B = plusMaterial > AIR_BLOCK;
        if(A == B)
        {
            whichFace = NOFACE;
            faceMaterial = 0;
            return;
        }
        if(A > B)
        {
            whichFace = MYFACE;
            return;
        }
        if(A < B)
        {
            whichFace = NOTMYFACE;
            faceMaterial = plusMaterial;
            return;
        }     
    }

    /**
    * This method will extractor a surface from the voxel data. It does this by operating on the slices
    *over the three axis. The method will merge faces that have the same material. It uses a front/back face
    *buffer to mark a voxel as been merged. When processing an merged voxel the voxel will be skipped.
    * \note Caching behavior may be bad.
    **/
    template <typename VoxelType>
    void ZCubicSurfaceExtractor<VoxelType>::execute()
    {
        using namespace PolyVox;

        for(uint16_t z = _regSizeInVoxels.getLowerCorner().getZ(); z <= _regSizeInVoxels.getUpperCorner().getZ() + 2; z++)
        {
            uint16_t startX = _regSizeInVoxels.getLowerCorner().getX();
            //Initialize X faces
            for(size_t i = 0; i < WORLD_BLOCK_WIDTH; ++i)
            {
                rleXsInfo[i].faceCount = 0;
                _resetParams(i, _regSizeInVoxels.getLowerCorner().getY(), z, 
                    rleXsInfo[i].whichFace, rleXsInfo[i].faceMaterial, X);
            }

            uint16_t regZ = z - _regSizeInVoxels.getLowerCorner().getZ();

            for(uint16_t y = _regSizeInVoxels.getLowerCorner().getY(); y <= _regSizeInVoxels.getUpperCorner().getY() + 2; y++)
            {

                //Initialize Z face.
                uint16_t zFaceCount = 0;

                FACE zWhichFace; //whether face is on voxel or face is on next voxel.
                VoxelType zFaceMaterial;
                _resetParams(startX, y, z, zWhichFace, zFaceMaterial, Z); //0 is Z axis.
                RLE_VEC rleZ;

                //Initialize Y face
                uint16_t yFaceCount = 0;
                FACE yWhichFace;
                VoxelType yFaceMaterial;
                _resetParams(startX, y, z, yWhichFace, yFaceMaterial, Y);
                RLE_VEC rleY;

                uint16_t regY = y - _regSizeInVoxels.getLowerCorner().getY();

                for(uint16_t x = _regSizeInVoxels.getLowerCorner().getX(); x <= _regSizeInVoxels.getUpperCorner().getX() + 2; x++)
                {
                    /*
                    if(x >= 10 && x < 20 && y == 79)
                        cout << "STOP HERE" << endl;
                    if(x == 10)
                        cout << "STOP HERE" << endl;
                    if(x == 19)
                        cout << "STOP HERE" << endl;
                        */
                    //Start at the lower corner x.
                    uint16_t regX = x - _regSizeInVoxels.getLowerCorner().getX();
                    //int currentVoxel = _volData->getVoxelAt(x, y, z).getDensity() >= VoxelType::getThreshold();
                    //int plusZVoxel = _volData->getVoxelAt(x, y, z+1).getDensity() >= VoxelType::getThreshold();
                    //int plusYVoxel = _volData->getVoxelAt(x, y+1, z).getDensity() >= VoxelType::getThreshold();
                    //int plusXVoxel = _volData->getVoxelAt(x+1, y, z).getDensity() >= VoxelType::getThreshold();
                    
                    VoxelType currentMaterial = _volData->getVoxelAt(x, y, z);
                    VoxelType currentMaterialPlusZ = _volData->getVoxelAt(x, y, z+1);
                    VoxelType currentMaterialPlusY = _volData->getVoxelAt(x, y+1, z);
                    VoxelType currentMaterialPlusX = _volData->getVoxelAt(x + 1, y, z);

                    _markRLE(zFaceMaterial, currentMaterial, 
                        currentMaterialPlusZ, zFaceCount, zWhichFace,
                        rleZ);

                    _markRLE(yFaceMaterial, currentMaterial, 
                        currentMaterialPlusY, yFaceCount, yWhichFace,
                        rleY);

                    _markRLE(rleXsInfo[regX].faceMaterial, currentMaterial,
                        currentMaterialPlusX, rleXsInfo[regX].faceCount, rleXsInfo[regX].whichFace,
                        rleXs[regX]);
                }
                _finalizeRLE(zFaceMaterial, zFaceCount, zWhichFace, rleZ);
                _finalizeRLE(yFaceMaterial, yFaceCount, yWhichFace, rleY);
                _mergeFace(rleZ, 0, regY, regZ, Z); //merge Z faces.
                _mergeFace(rleY, 0, regY, regZ, Y); 
            }

            for(size_t i = 0; i < WORLD_BLOCK_WIDTH; ++i)
            {

                _finalizeRLE(rleXsInfo[i].faceMaterial, rleXsInfo[i].faceCount, rleXsInfo[i].whichFace,
                    rleXs[i]);
                _mergeFace(rleXs[i], 
                    (uint16_t)(i), 0, regZ, X);
                rleXs[i].clear();
            } 
        }
        _meshCurrent->m_Region = _regSizeInVoxels;
        _meshCurrent->m_vecLodRecords.clear();
        LodRecord lodRecord;
        lodRecord.beginIndex = 0;
        lodRecord.endIndex = _meshCurrent->getNoOfIndices();
        _meshCurrent->m_vecLodRecords.push_back(lodRecord);

    }



    /** This method will mark the a slice based on RLE.**/
    template <typename VoxelType>
    inline void ZCubicSurfaceExtractor<VoxelType>::_markRLE(VoxelType &faceMaterial, VoxelType currentMaterial,
        VoxelType currentMaterialPlus, uint16_t &faceCount, FACE &previousWhichFace, 
        RLE_VEC &rleVec)
    {
        bool A = currentMaterial > AIR_BLOCK;
        bool B = currentMaterialPlus > AIR_BLOCK;

        if(A == B) //The current voxel and plusVoxel must have the same material (AIR or Any Material)
        {
            if(previousWhichFace == NOFACE)
            {
                faceCount++;
            }
            else
            {
                rleVec.push_back(std::make_pair(faceMaterial, std::make_pair(faceCount, previousWhichFace))); //write facematerial, count, and which face face is facing.
                previousWhichFace = NOFACE;
                faceMaterial = AIR_BLOCK;
                faceCount = 1;
            }
            return;
        }
        if(A > B) //If current voxel is greater than plusVoxel
        {
            if(previousWhichFace == MYFACE)
            {
                if(faceMaterial == currentMaterial)
                    faceCount++;
                else
                {
                    rleVec.push_back(std::make_pair(faceMaterial, std::make_pair(faceCount, previousWhichFace)));
                    //previousWhichFace = MYFACE;
                    faceMaterial = currentMaterial;
                    faceCount = 1;
                }
            } 
            else //The previous face is NOTMYFACE or NOFACE, meaning the face has flipped to MyFace.
            {
                rleVec.push_back(std::make_pair(faceMaterial, std::make_pair(faceCount, previousWhichFace)));
                previousWhichFace = MYFACE;
                faceMaterial = currentMaterial;
                faceCount = 1;
            }
            return;
        }
        if(A < B) //CurrentVoxel is less than plusVoxel.
        {
            if(previousWhichFace == MYFACE)
            {
                rleVec.push_back(std::make_pair(faceMaterial, std::make_pair(faceCount, previousWhichFace)));
                previousWhichFace = NOTMYFACE;
                faceMaterial = currentMaterialPlus;
                faceCount = 1;
            }
            else
            {
                if(faceMaterial == currentMaterialPlus)
                {
                    faceCount++;
                }
                else
                {
                    rleVec.push_back(std::make_pair(faceMaterial, std::make_pair(faceCount, previousWhichFace)));
                    previousWhichFace = NOTMYFACE;
                    faceMaterial = currentMaterialPlus;
                    faceCount = 1;
                }
            }
            return;
        }
    }

    template <typename VoxelType>
    inline void ZCubicSurfaceExtractor<VoxelType>::_finalizeRLE(
        VoxelType faceMaterial, uint16_t faceCount, FACE previousWhichFace,
        RLE_VEC& rleVec)
    {
        rleVec.push_back(std::make_pair(faceMaterial, 
            std::make_pair(faceCount, previousWhichFace)));
    }

    /** This method will generate faces given an Run Length Encoding vector representing compressed faces.**/
    template <typename VoxelType>
    inline void ZCubicSurfaceExtractor<VoxelType>::_mergeFace(RLE_VEC &rleVec,
        uint16_t regX, uint16_t regY, uint16_t regZ, AXIS xyz)
    {
        uint16_t count = 0;
        for(size_t i = 0; i < rleVec.size(); ++i)
        {
            count = rleVec[i].second.first;

            if(rleVec[i].first > 0)
            {
                uint32_t v0, v1, v2, v3;
                //Lower left corner
                VoxelType faceMaterial = rleVec[i].first;

                switch(xyz)
                {
                case Z: //z axis
                    v0 = _meshCurrent->addVertex(PositionMaterial(Vector3DFloat(regX - 0.5f, regY - 0.5f, regZ + 0.5f), 
                        faceMaterial));
                    //upper left corner
                    v1 = _meshCurrent->addVertex(PositionMaterial(Vector3DFloat(regX - 0.5f, regY + 0.5f, regZ + 0.5f), 
                        faceMaterial));
                    //lower right corner
                    v2 = _meshCurrent->addVertex(PositionMaterial(Vector3DFloat(regX + count - 0.5f, regY - 0.5f, regZ + 0.5f), 
                        faceMaterial));
                    //upper right corner
                    v3 = _meshCurrent->addVertex(PositionMaterial(Vector3DFloat(regX + count - 0.5f, regY + 0.5f, regZ + 0.5f), 
                        faceMaterial));
                    if(rleVec[i].second.second == MYFACE)
                    {

                        _meshCurrent->addTriangleCubic(v0, v2, v1);
                        _meshCurrent->addTriangleCubic(v1, v2, v3);
                    }
                    else
                    {
                        _meshCurrent->addTriangleCubic(v0, v1, v2);
                        _meshCurrent->addTriangleCubic(v1, v3, v2);
                    }
                    break;
                case Y: //y axis
                    v0 = _meshCurrent->addVertex(PositionMaterial(Vector3DFloat(regX - 0.5f, regY + 0.5f, regZ - 0.5f), 
                        faceMaterial));
                    //upper left corner
                    v1 = _meshCurrent->addVertex(PositionMaterial(Vector3DFloat(regX - 0.5f, regY + 0.5f, regZ + 0.5f), 
                        faceMaterial));
                    //lower right corner
                    v2 = _meshCurrent->addVertex(PositionMaterial(Vector3DFloat(regX + count - 0.5f, regY + 0.5f, regZ - 0.5f), 
                        faceMaterial));
                    //upper right corner
                    v3 = _meshCurrent->addVertex(PositionMaterial(Vector3DFloat(regX + count - 0.5f, regY + 0.5f, regZ + 0.5f), 
                        faceMaterial));
                   
                    if(rleVec[i].second.second == MYFACE)
                    {
                         _meshCurrent->addTriangleCubic(v0, v1, v2);
                         _meshCurrent->addTriangleCubic(v1, v3, v2);
                    }
                    else
                    {

                        _meshCurrent->addTriangleCubic(v0, v2, v1);
                        _meshCurrent->addTriangleCubic(v1, v2, v3);
                    }
                    break;
                case X:
                    v0 = _meshCurrent->addVertex(PositionMaterial(Vector3DFloat(regX + 0.5f, regY - 0.5f, regZ - 0.5f), 
                        faceMaterial));
                    //upper left corner
                    v1 = _meshCurrent->addVertex(PositionMaterial(Vector3DFloat(regX + 0.5f, regY - 0.5f, regZ + 0.5f), 
                        faceMaterial));
                    //lower right corner
                    v2 = _meshCurrent->addVertex(PositionMaterial(Vector3DFloat(regX + 0.5f, regY + count - 0.5f, regZ - 0.5f), 
                        faceMaterial));
                    //upper right corner
                    v3 = _meshCurrent->addVertex(PositionMaterial(Vector3DFloat(regX + 0.5f, regY + count - 0.5f, regZ + 0.5f),
                        faceMaterial));
                    if(rleVec[i].second.second == MYFACE)
                    {

                        _meshCurrent->addTriangleCubic(v0, v2, v1);
                        _meshCurrent->addTriangleCubic(v1, v2, v3);
                    }
                    else
                    {
                        _meshCurrent->addTriangleCubic(v0, v1, v2);
                        _meshCurrent->addTriangleCubic(v1, v3, v2);
                    }
                    break;
                default:
                    break;
                }

            }
            switch(xyz)
            {
            case Z:
            case Y:
                regX += count;
                break;
            case X:
                regY += count;
                break;
            }
        }


    }
}
