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
using PolyVox::SurfaceVertex;
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
VolumeMapView::createRegion(bool regionEmpty, PolyVox::SurfaceMesh* mesh)
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
  _root->detachObject(_manual);
  _scnMgr->destroyManualObject(_manual);
  _root->setVisible(false, true);
  _manual = 0;

}

void
VolumeMapView::updateRegion(bool regionEmpty, PolyVox::SurfaceMesh* mesh)
{
  if (!regionEmpty)
    _manualFromMesh(true, mesh, _manual);
}

void
VolumeMapView::_manualFromMesh(bool isUpdate, PolyVox::SurfaceMesh* mesh, Ogre::ManualObject* manual)
{
  const float ATLAS_WIDTH = 4096.0f;
  const float TEX_WIDTH = 256.0f;
  const float BlockOffset = (TEX_WIDTH) / ATLAS_WIDTH;
  const float pixelOffset = 1.0 / ATLAS_WIDTH;
  const float blockEOffset = 1.0 / TEX_WIDTH;
  using std::vector;
  const vector<uint32_t>& indices = mesh->getIndices();
  const vector<SurfaceVertex>& vertices = mesh->getVertices();

  if(vertices.size() < 1)
    return;

  //Build the Ogre Manual Object. We first iterate through the vertices add position to that.
  //
  const Ogre::Vector2 texCoords[4] =
    { Ogre::Vector2(0.0f, 0.0f), Ogre::Vector2(1.0f, 0.0f), Ogre::Vector2(0.0f, 1.0f), Ogre::Vector2(1.0f, 1.0f) };
  size_t texIdx = 0;

  _manual->estimateVertexCount(indices.size());
  _manual->estimateVertexCount(vertices.size());

  if (isUpdate)
    {
      manual->beginUpdate(0);
   } 
  else
    _manual->begin("PRJZ/Minecraft", Ogre::RenderOperation::OT_TRIANGLE_LIST, "PROJECT_ZOMBIE");

  for (vector<SurfaceVertex>::const_iterator itVertex = vertices.begin(); itVertex != vertices.end(); ++itVertex)
    {
      const SurfaceVertex& vertex = *itVertex;
      const Vector3DFloat& vertPos = vertex.getPosition();
      //const Vector3DFloat& finalPos = vertPos + static_cast<Vector3DFloat> (mesh.m_Region.getLowerCorner());
      Vector3DFloat origin(_origin.x, _origin.y, _origin.z);
      const Vector3DFloat& finalPos = vertPos + origin;
      manual->position(finalPos.getX(), finalPos.getY(), finalPos.getZ());
      manual->normal(vertex.getNormal().getX(), vertex.getNormal().getY(), vertex.getNormal().getZ());
      //Assume that the passed in position are counter clockwise starting from top left corner.
      manual->textureCoord(texCoords[texIdx % 4]);
      texIdx++;

      Ogre::ColourValue val;
      size_t material = vertex.getMaterial() + 0.5f; 
      
      manual->textureCoord(material / 256.0f, 0.0, 0.0, 0.0);
      //val.r = (float)(material) / 256.0f;//(float) (material - 1) * BlockOffset;// - pixelOffset;
      //val.g = 0.0f;
      //val.b = 0.0f;
      //val.a = 1.0f;
      //Ogre::Colo
      //manual->colour(val);
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
