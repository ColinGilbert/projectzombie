/*
* VolumeMapView.cpp
*
*  Created on: Sep 21, 2010
*      Author: beyzend
*/

#include "world/VolumeMapView.h"
#include "EngineView.h"
#include <iostream>
#include <Shapes/OgreBulletCollisionsTrimeshShape.h>
#include <Utils/OgreBulletCollisionsMeshToShapeConverter.h>
#include "world/PhysicsManager.h"
#include <OgreException.h>
using std::cout;
using std::endl;

using namespace ZGame::World;
using PolyVox::PositionMaterial;
using PolyVox::SurfaceMesh;
using PolyVox::Vector3DFloat;
using namespace Ogre;

VolumeMapView::VolumeMapView() : _manual(0), _phyBody(0)
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
   

}

void
    VolumeMapView::createRegion(const Ogre::Vector3 &origin, PolyVox::SurfaceMesh<PositionMaterial>* mesh)
{
    _origin = origin;
    assert(_manual == 0 && "Manual should be null invarience failed.");
    _manual = _scnMgr->createManualObject();
    _manual->setCastShadows(false);
    _manual->setDynamic(false);
    _manualFromMesh(false, mesh, _manual);
}

void
    VolumeMapView::unloadRegion(PhysicsManager* phyMgr)
{
    _manual->detachFromParent();
    _scnMgr->destroyManualObject(_manual);
    _root->setVisible(false, true);
    phyMgr->destroyBody(_phyBody);
    _phyBody = 0;
}

void
    VolumeMapView::updateRegion(PolyVox::SurfaceMesh<PositionMaterial>* mesh)
{
    //CHECK manual is null.
    assert(_manual == 0 && "Manual should be null invariance failed.");
    _manual = _scnMgr->createManualObject();
    _manual->setCastShadows(false);
    _manual->setDynamic(false);
    _manualFromMesh(false, mesh, _manual);
}

void
    VolumeMapView::_manualFromMesh(bool isUpdate, PolyVox::SurfaceMesh<PositionMaterial>* mesh, Ogre::ManualObject* manual)
{
    using std::vector;
    const vector<uint32_t>& indices = mesh->getIndices();
    const vector<PositionMaterial>& vertices = mesh->getVertices();

    if(vertices.size() < 1)
        return;
    _manual->estimateIndexCount(indices.size());
    _manual->estimateVertexCount(vertices.size());
    _manual->begin("PRJZ/Minecraft", Ogre::RenderOperation::OT_TRIANGLE_LIST, "PROJECT_ZOMBIE");

    for (vector<PositionMaterial>::const_iterator itVertex = vertices.begin(); itVertex != vertices.end(); ++itVertex)
    {
        const PositionMaterial& vertex = *itVertex;
        const Vector3DFloat& vertPos = vertex.getPosition();
       
        manual->position(vertPos.getX(), vertPos.getY(), vertPos.getZ());
        size_t material = vertex.getMaterial() + 0.5f; 

        manual->textureCoord(material / 256.0f, 0.0, 0.0, 0.0);
    }
    //Then iterate through the indices add create the indices list.
    for (vector<uint32_t>::const_iterator itIdx = indices.begin(); itIdx != indices.end(); ++itIdx)
    {
        manual->index(*itIdx);
        uint32_t ix = *itIdx;
    }
    manual->end();
}

void
    VolumeMapView::createPhysicsRegion(PhysicsManager* mgr)
{
    _phyBody = mgr->staticObjectFromManual(_manual, _phyBody, 0.1f, 0.6f, _root->_getFullTransform());
}


void
    VolumeMapView::finalizeRegion()
{
    _root->attachObject(_manual);
    _root->setPosition(_origin);
    _root->setVisible(true);
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
