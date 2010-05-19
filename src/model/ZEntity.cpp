/*
* ZEntity.cpp
*
*  Created on: Sep 18, 2008
*      Author: bey0nd
*/

#include <iostream>

#include "entities/ZEntity.h"

#include "CommandController.h"
#include "command/CommandList.h"

//using namespace ZGame;
using namespace ZGame::Entities;

ZEntity::ZEntity(const Ogre::String &entName, const Ogre::String &meshName) : _entName(entName), _resource(meshName)
{
}

ZEntity::ZEntity(const Ogre::String &entName, const ZEntityResource &res) : _entName(entName), _resource(res)
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

const Ogre::String 
ZEntity::getEntityName() 
{
    return _entName;
}

void
ZEntity::createEntity()
{
    
}
