/*
* VolumeMapView.cpp
*
*  Created on: Sep 21, 2010
*      Author: beyzend
*/

#include "world/VolumeMapView.h"
#include "EngineView.h"
#include <iostream>
using std::cout;
using std::endl;

using namespace ZGame::World;
using PolyVox::PositionMaterial;
using PolyVox::SurfaceMesh;
using PolyVox::Vector3DFloat;
using namespace Ogre;

VolumeMapView::VolumeMapView()
{
    _initManualObject();
}

VolumeMapView::~VolumeMapView()
{
}

void
    VolumeMapView::_initManualObject()
{
    _scnMgr = EngineView::getSingleton().getSceneManager();
    _root = _scnMgr->getRootSceneNode()->createChildSceneNode();
    //_manual = _scnMgr->createManualObject();
    //_manual->setCastShadows(false);
    //_manual->setDynamic(false);

}

void
    VolumeMapView::createRegion(bool regionEmpty, PolyVox::SurfaceMesh<PositionMaterial>* mesh)
{
    //if (!regionEmpty)
    _manual = _scnMgr->createManualObject();
    _manual->setCastShadows(false);
    _manual->setDynamic(false);
    _manualFromMesh(false, mesh, _manual);
}

void
    VolumeMapView::unloadRegion(bool regionEmpty)
{
    //if (!regionEmpty)
    //_root->detachObject(_manual);
    _manual->detachFromParent();
    _scnMgr->destroyManualObject(_manual);
    _root->setVisible(false, true);
    _manual = 0;

}

void
    VolumeMapView::updateRegion(bool regionEmpty, PolyVox::SurfaceMesh<PositionMaterial>* mesh)
{
    if (!regionEmpty)
        _manualFromMesh(true, mesh, _manual);
}

void
    VolumeMapView::_manualFromMesh(bool isUpdate, PolyVox::SurfaceMesh<PositionMaterial>* mesh, Ogre::ManualObject* manual)
{
    using std::vector;
    const vector<uint32_t>& indices = mesh->getIndices();
    const vector<PositionMaterial>& vertices = mesh->getVertices();

    if(vertices.size() < 1)
        return;

    //Build the Ogre Manual Object. We first iterate through the vertices add position to that.
    //
    _manual->estimateIndexCount(indices.size());
    _manual->estimateVertexCount(vertices.size());
    _manual->begin("PRJZ/Minecraft", Ogre::RenderOperation::OT_TRIANGLE_LIST, "PROJECT_ZOMBIE");

    for (vector<PositionMaterial>::const_iterator itVertex = vertices.begin(); itVertex != vertices.end(); ++itVertex)
    {
        const PositionMaterial& vertex = *itVertex;
        const Vector3DFloat& vertPos = vertex.getPosition();
        //const Vector3DFloat& finalPos = vertPos + static_cast<Vector3DFloat> (mesh.m_Region.getLowerCorner());
        Vector3DFloat origin(_origin.x, _origin.y, _origin.z);
        const Vector3DFloat& finalPos = vertPos + origin;
        manual->position(finalPos.getX(), finalPos.getY(), finalPos.getZ());
        //manual->normal(vertex.getNormal().getX(), vertex.getNormal().getY(), vertex.getNormal().getZ()); 
        //Assume that the passed in position are counter clockwise starting from top left corner.

        Ogre::ColourValue val;
        size_t material = vertex.getMaterial() + 0.5f; 

        manual->textureCoord(material / 256.0f, 0.0, 0.0, 0.0);
    }
    //Then iterate through the indices add create the indices list.
    for (vector<uint32_t>::const_iterator itIdx = indices.begin(); itIdx != indices.end(); ++itIdx)
    {
        manual->index(*itIdx);
        uint32_t ix = *itIdx;
        //cout << "INDEX!: " << ix << endl;
    }
    manual->end();

    //add it to root scene.
    _root->attachObject(manual);
    _root->setVisible(true);
    //scnMgr->getRootSceneNode()->createChildSceneNode()->attachObject(manual);
}

void
    VolumeMapView::_convertColorFromMaterialID(Ogre::ColourValue &val, uint8_t materialID)
{
    switch (materialID)
    {
    case 1:
        val.r = 1.0f;
        val.g = 0.0f;
        val.b = 0.0f;
        break;
    case 2:
        val.r = 0.0f;
        val.g = 1.0f;
        val.b = 0.0f;
        break;
    case 3:
        val.r = 0.0f;
        val.g = 0.0f;
        val.b = 1.0f;
        break;
    case 4:
        val.r = 1.0f;
        val.g = 1.0f;
        val.b = 0.0f;
        break;
    case 5:
        val.r = 1.0f;
        val.g = 0.0f;
        val.b = 1.0f;
        break;
    default:
        val.r = 1.0f;
        val.g = 1.0f;
        val.b = 1.0f;
    }
}
