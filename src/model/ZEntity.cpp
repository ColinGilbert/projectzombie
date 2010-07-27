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

ZEntity::ZEntity(const ZENT_KEY &key, const Ogre::String &meshName) 
: _entKey(key), _resource(key, meshName), _worldPos(Ogre::Vector3(0, 0, 0)), _worldOrient(Ogre::Quaternion())
{
}

ZEntity::ZEntity(const ZEntityResource &res) 
: _entKey(res.getKey()), _resource(res)
{
}

ZEntity::~ZEntity()
{

}
/*
void 
ZEntity::setEntityName(const Ogre::String entName) 
{ 
    _entName = entName;
}*/

const ZENT_KEY 
ZEntity::getEntityName() const
{
    return _entKey;
}

/**
*This function is called for on read events--meaning, that 
*
*/
bool
ZEntity::onRead(const Ogre::Vector3 &pos, const Ogre::Quaternion &orient)
{
    _worldPos = pos;   
    _worldOrient = orient;
    return true;
}

/**
*This function is called for on write events--meaning,
*
*/
bool
ZEntity::onWrite(Ogre::Vector3 &pos, Ogre::Quaternion &orient) 
{
    pos = _worldPos;
    orient = _worldOrient;
    return true;
}
