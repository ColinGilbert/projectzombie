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

  MeshPtr
  GPUEntsMeshBuilder::build(const string name, GPUEntsGenProps* props)
  {
    //LogManager* lm = LogManager::getSingleton().logMessage(LML_TRIVIAL,"In GPUEntsMeshBuilder::build()");
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
    man->setDynamic(false);
    

    //unsigned int pointIndex = 0;
    uint32 pointIndex = 0;
    //uint16 pointIndex = 0;
    Real u, v; //texture coordinate correspond to entity ID.
    u = 0.0f;
    v = 0.0f;
    size_t texW = (size_t) props->getTexWidth();

    size_t texH = (size_t) props->getTexHeight();
    Real tOffset = 1.0f / texW; //texture offset.

    Vector2 uv(u, v);
    Vector2 du(tOffset, 0.0f);
    Vector2 dv(0.0f, tOffset);

    man->begin("BaseWhiteNoLighting", RenderOperation::OT_TRIANGLE_LIST);
    Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL,"Began man mesh looping build");
    //Now loop N entities and create a quad for each entity, with each quad having an ID corresponding to the entity.
    for (size_t j = 0; j < texH; j++)
      {
        /*
        if (pointIndex >= 65536)
          {
            man->end();
            man->begin("BaseWhiteNoLighting", RenderOperation::OT_TRIANGLE_LIST);
            pointIndex = 0;
          }*/
        for (size_t i = 0; i < texW; i++)
          {
            //random offset the height to get rid of homogenous heights

            man->position(corners[UPL]); //upper left
            man->textureCoord(texCoords[UPL]);
            man->textureCoord(uv); //we store the entity ID in the 2nd set of texture coords.
            //man->textureCoord(texCoords[UPL]);
            man->position(corners[LRL]); //lower left
            man->textureCoord(texCoords[LRL]);
            man->textureCoord(uv);
            //man->textureCoord(texCoords[LRL]);
            man->position(corners[LRR]); //lower right
            man->textureCoord(texCoords[LRR]);
            man->textureCoord(uv);
            //man->textureCoord(texCoords[LRR]);
            man->position(corners[UPR]); //upper right
            man->textureCoord(texCoords[UPR]);
            man->textureCoord(uv);
            //man->textureCoord(texCoords[UPR]);
            man->quad(pointIndex, pointIndex + 1, pointIndex + 2, pointIndex
                + 3);
            pointIndex += 4;
            //uv += dv;
            uv.x += tOffset;
          }
        //uv.y = 0.0f;
        //uv += du;
        uv.x = 0.0f;
        uv.y += tOffset;
        //uv += dv;
      }
    Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL,"Finished looping creation of manual mesh.");
    man->end();
    Ogre::AxisAlignedBox infAAB;
    //infAAB.setInfinite();
    infAAB.setExtents(-5500.0, -5500.0, -5500.0, 5500.0, 5500.0, 5500.0);
    man->setBoundingBox(infAAB);
    Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL,"InfABB set");
    MeshPtr mesh = man->convertToMesh(name);
    Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL,"Mesh converted.");
    scnMgr->destroyManualObject(man);
    Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL,"Manual Mesh build!");
    return mesh;
  }

}
