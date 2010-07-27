#include "entities/ZEntityResource.h"

using namespace ZGame::Entities;

const ZENT_KEY ZEntityResource::NULL_KEY(""); //Empty string is NULL_KEY as of now.

ZEntityResource::ZEntityResource(const ZENT_KEY &key, const Ogre::String &resName) 
: _key(key), _resourceName(resName)
{
}

ZEntityResource::~ZEntityResource()
{
}