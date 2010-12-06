
#include <iostream>
using std::cout;
using std::endl;
#include <Array.h>
#include <MaterialDensityPair.h>
#include <SurfaceMesh.h>
#include <PolyVoxImpl/MarchingCubesTables.h>
#include <VertexTypes.h>
namespace PolyVox
{
    template <typename VoxelType>
    ZCubicSurfaceExtractor<VoxelType>::ZCubicSurfaceExtractor(Volume<VoxelType>* volData, Region region,
        SurfaceMesh<PositionMaterial>* result) : _volData(volData),
        //_sampVolume(volData),
        _regSizeInVoxels(region),
        _meshCurrent(result)
    {
        
        //_regSizeInVoxels.cropTo(_volData->getEnclosingRegion());
        _regSizeInCells = _regSizeInVoxels;
        _regSizeInCells.setUpperCorner(_regSizeInCells.getUpperCorner() - Vector3DInt16(1, 1, 1));
        
        _meshCurrent->clear();
    }

    template <typename VoxelType>
    inline void ZCubicSurfaceExtractor<VoxelType>::_resetParams(int16_t x, int16_t y, int16_t z,
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
    * This method will extract a surface from the voxel data. The surface that is extracted will be decimated through
    * RLE; such that, for a slice the number of faces will be less than or equal to the number of voxels for that slice. 
    * \precondition the volume data is valid in that it specifies a valid region of data (take notice of boundary conditions else you will get holes).
    * \postcondition a surface is extracted from the given Volume and that surfaces has been merged through RLE.
    **/
    template <typename VoxelType>
    void ZCubicSurfaceExtractor<VoxelType>::execute()
    {
        using namespace PolyVox;

        const size_t WORLD_BLOCK_WIDTH = _regSizeInVoxels.depth();
        std::vector<RLE_VEC> rleXs(WORLD_BLOCK_WIDTH); 
        std::vector<RLE_INFO> rleXsInfo(WORLD_BLOCK_WIDTH);
        int16_t startX = _regSizeInVoxels.getLowerCorner().getX();
            
        for(int16_t z = _regSizeInVoxels.getLowerCorner().getZ(); z < _regSizeInVoxels.getUpperCorner().getZ(); z++)
        {
            //Initialize X faces
            for(size_t i = 0; i < WORLD_BLOCK_WIDTH; ++i)
            {
                rleXsInfo[i].faceCount = 0;
                _resetParams(startX + i, 0, z, 
                    rleXsInfo[i].whichFace, rleXsInfo[i].faceMaterial, X);
            }

            int16_t regZ = z - _regSizeInVoxels.getLowerCorner().getZ();

            for(int16_t y = _regSizeInVoxels.getLowerCorner().getY(); y < _regSizeInVoxels.getUpperCorner().getY(); y++)
            {

                //Initialize Z face.
                uint16_t zFaceCount = 0;

                FACE zWhichFace; //whether face is on voxel or face is on next voxel.
                VoxelType zFaceMaterial;
                _resetParams(startX, y, z, zWhichFace, zFaceMaterial, Z); 
                RLE_VEC rleZ;

                //Initialize Y face
                uint16_t yFaceCount = 0;
                FACE yWhichFace;
                VoxelType yFaceMaterial;
                _resetParams(startX, y, z, yWhichFace, yFaceMaterial, Y);
                RLE_VEC rleY;

                int16_t regY = y - _regSizeInVoxels.getLowerCorner().getY();

                for(int16_t x = _regSizeInVoxels.getLowerCorner().getX(); x < _regSizeInVoxels.getUpperCorner().getX(); x++)
                {
                    //Start at the lower corner x.
                    int16_t regX = x - _regSizeInVoxels.getLowerCorner().getX();
                    if(x == 63 && y == 127 && z == 79)
                        cout << "stop here" << endl;
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
                _mergeFace(rleZ, startX, y, z, Z); 
                _mergeFace(rleY, startX, y, z, Y); 
            }

            for(size_t i = 0; i < WORLD_BLOCK_WIDTH; ++i)
            {

                _finalizeRLE(rleXsInfo[i].faceMaterial, rleXsInfo[i].faceCount, rleXsInfo[i].whichFace,
                    rleXs[i]);
                _mergeFace(rleXs[i], 
                    startX + i, 0, z, X);
                rleXs[i].clear();
            } 
        }
    }



    /** This method will mark a slice based on RLE given book keeping data for RLE.**/
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
                rleVec.push_back(RLE_VOXEL(faceMaterial, faceCount, previousWhichFace)); //write facematerial, count, and which face face is facing.
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
                    rleVec.push_back(RLE_VOXEL(faceMaterial, faceCount, previousWhichFace));
                    //previousWhichFace = MYFACE;
                    faceMaterial = currentMaterial;
                    faceCount = 1;
                }
            } 
            else //The previous face is NOTMYFACE or NOFACE, meaning the face has flipped to MyFace.
            {
                rleVec.push_back(RLE_VOXEL(faceMaterial, faceCount, previousWhichFace));
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
                rleVec.push_back(RLE_VOXEL(faceMaterial, faceCount, previousWhichFace));
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
                    rleVec.push_back(RLE_VOXEL(faceMaterial, faceCount, previousWhichFace));
                    previousWhichFace = NOTMYFACE;
                    faceMaterial = currentMaterialPlus;
                    faceCount = 1;
                }
            }
            return;
        }
    }

    /** This method does the final step in marking a slice with RLE. Basically, the last run length data needs to be written out (i.e the state machine
    has reached a final state, in the final state we need to write out the last bit of Run Length).**/
    template <typename VoxelType>
    inline void ZCubicSurfaceExtractor<VoxelType>::_finalizeRLE(
        VoxelType faceMaterial, uint16_t faceCount, FACE previousWhichFace,
        RLE_VEC& rleVec)
    {
        rleVec.push_back(RLE_VOXEL(faceMaterial, faceCount, previousWhichFace));
    }

    /** This method will generate faces given a Run Length Encoding vector representing compressed faces.**/
    template <typename VoxelType>
    inline void ZCubicSurfaceExtractor<VoxelType>::_mergeFace(RLE_VEC &rleVec,
        int16_t regX, int16_t regY, int16_t regZ, AXIS xyz)
    {
        uint16_t count = 0;
        for(size_t i = 0; i < rleVec.size(); ++i)
        {
            count = rleVec[i].length;
            
            if(rleVec[i].material > AIR_BLOCK)
            {
                uint32_t v0, v1, v2, v3;
                //Lower left corner
                VoxelType faceMaterial = rleVec[i].material;

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
                    if(rleVec[i].whichFace== MYFACE)
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
                   
                    if(rleVec[i].whichFace == MYFACE)
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
                    if(rleVec[i].whichFace == MYFACE)
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
