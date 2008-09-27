/*
 * GPUEntsMeshBuilder.cpp
 *
 *  Created on: Sep 23, 2008
 *      Author: bey0nd
 */

#include "GPUEntsMeshBuilder.h"
#include "GPUEntsGenProps.h"
#include "EngineView.h"
using namespace Ogre;

namespace ZGame
{

  GPUEntsMeshBuilder::GPUEntsMeshBuilder()
  {
    // TODO Auto-generated constructor stub

  }

  GPUEntsMeshBuilder::~GPUEntsMeshBuilder()
  {
    // TODO Auto-generated destructor stub
  }

  MeshPtr GPUEntsMeshBuilder::build(const string name,GPUEntsGenProps* props)
  {
    Real entHeight = props->getEntHeight();
    Real halfHeight = entHeight * 0.5;
    const string manName = "TEMPMANUAL";
    Vector3 min(0.0f - halfHeight, 0.0f - halfHeight, 0.0f);
    Vector3 changeX(entHeight, 0.0f, 0.0f);
    Vector3 changeY(0.0f, entHeight, 0.0f);
    const uint32 UPL = 0;
    const uint32 LRL = 1;
    const uint32 LRR = 2;
    const uint32 UPR = 3;
    Vector3 corners[4] =
      { min + changeY, //upper left
          min, //lower left
          min + changeX, //lower right
          min + changeX + changeY }; //upper right
    Vector2 texCoords[4] =
      { Vector2(0.0f, 0.0f), //upper left
          Vector2(0.0f, 1.0f), //lower left
          Vector2(1.0f, 1.0f), //lower right
          Vector2(1.0, 0.0f) //uppper right
        };
    SceneManager* scnMgr = EngineView::getSingleton().getSceneManager();
    ManualObject* man = scnMgr->createManualObject(manName.c_str());
    man->begin("BaseWhiteNoLighting", RenderOperation::OT_TRIANGLE_LIST);
    unsigned int pointIndex = 0;
    Real u, v; //texture coordinate correspond to entity ID.
    u = v = 0.0f;

    size_t texW = (size_t) props->getTexHeight();
    size_t texH = (size_t) props->getTexHeight();
    Real tOffset = 1.0f / texW; //texture offset.

    Vector2 uv(0.0f, 0.0f);
    Vector2 du(tOffset, 0.0f);
    Vector2 dv(0.0f, tOffset);

    //Now loop N entities and create a quad for each entity, with each quad having an ID corresponding to the entity.
    for (size_t j = 0; j < texH; j++)
      {
        if (pointIndex == 65536)
          {
            man->end();
            man->begin("BaseWhiteNoLighting", RenderOperation::OT_TRIANGLE_LIST);
            pointIndex = 0;
          }
        for (size_t i = 0; i < texW; i++)
          {
            man->position(corners[UPL]); //upper left
            man->textureCoord(texCoords[UPL]);
            man->textureCoord(uv); //we store the entity ID in the 2nd set of texture coords.
            man->position(corners[LRL]); //lower left
            man->textureCoord(texCoords[LRL]);
            man->textureCoord(uv);
            man->position(corners[LRR]); //lower right
            man->textureCoord(texCoords[LRR]);
            man->textureCoord(uv);
            man->position(corners[UPR]); //upper right
            man->textureCoord(texCoords[UPR]);
            man->textureCoord(uv);

            man->quad(pointIndex, pointIndex + 1, pointIndex + 2, pointIndex
                + 3);
            pointIndex += 4;
            uv += du;
          }
        uv.x = 0.0f;
        uv += dv;
      }
    man->end();
    Ogre::AxisAlignedBox infAAB;
    //infAAB.setInfinite();
    infAAB.setExtents(-2500.0,-2500.0,-2500.0,2500.0,2500.0,2500.0);
    man->setBoundingBox(infAAB);
    MeshPtr mesh = man->convertToMesh(name);
    scnMgr->destroyManualObject(man);
    return mesh;
  }


}
