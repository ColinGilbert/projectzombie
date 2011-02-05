#include "toolset/ToolsetManager.h"
#include "geometry/GeometryManager.h"
using namespace ZGame::Toolset;
using namespace ZGame;
ToolsetManager::ToolsetManager(Geometry::GeometryManager* geoMgr)
    : _geoMgr(geoMgr), _selectionId(-1)
{
}

ToolsetManager::~ToolsetManager()
{
   for(auto iter = _tools.begin(); iter != _tools.end(); ++iter)
   {
       delete *iter;
   }
}


ToolInfo*
    ToolsetManager::getTool(Ogre::uint16 id)
{
    ToolInfo* ret = _tools.at(id); //bound checking should be done here.
    return ret;
}

/**
* \note We should use the component system to add a Render compoent. However, component system is not finished so we do 
*this. 
**/
const Ogre::uint16
    ToolsetManager::createCursor()
{
    Ogre::uint16 id = static_cast<Ogre::uint16>(_tools.size());
    Ogre::SceneNode* node = _geoMgr->createCube(Ogre::Vector3(1.05f, 1.05f, 1.05f), "cursor");
    _tools.push_back(new ToolInfo(id, node));
    return id;
}

ToolInfo*
    ToolsetManager::createCube()
{
    Ogre::uint16 id = static_cast<Ogre::uint16>(_tools.size());
    Ogre::SceneNode* node = _geoMgr->createCube(Ogre::Vector3(10.0f, 10.0f, 1.0f), "toolcube");
    ToolInfo* tool = new ToolInfo(id, node);
    _tools.push_back(tool);
    return tool;
}