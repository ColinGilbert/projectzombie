/*
* ZEntity.cpp
*
*  Created on: Sep 18, 2008
*      Author: bey0nd
*/

#include "ZEntity.h"

//using namespace ZGame;
using ZGame::Entities::ZEntity;

ZEntity::ZEntity(const Ogre::String &entName,const Ogre::String &meshName) : _entName(entName), _meshName(meshName)
{
}

ZEntity::~ZEntity()
{

}

void 
ZEntity::setEntityName(const Ogre::String entName) 
{ 
    _entName = entName;
}
void 
ZEntity::setMeshName(const Ogre::String meshName)
{ 
    _meshName = meshName;
}
const Ogre::String 
ZEntity::getEntityName() 
{
    return _entName;
}
const Ogre::String 
ZEntity::getMeshName() 
{
    return _meshName;
}


