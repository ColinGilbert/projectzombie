#include "toolset/ToolsetManager.h"
#include "geometry/GeometryManager.h"
using namespace ZGame::Toolset;
using namespace ZGame;
ToolsetManager::ToolsetManager(Geometry::GeometryManager* geoMgr)
    : _geoMgr(geoMgr)
{
}

ToolsetManager::~ToolsetManager()
{
}


ToolInfo*
    ToolsetManager::getTool(Ogre::uint16 id)
{
    return &_tools[id]; 
}

const Ogre::uint16
    ToolsetManager::createCursor()
{
    Ogre::uint16 id = static_cast<Ogre::uint16>(_tools.size());
    Ogre::SceneNode* node = _geoMgr->createCube(Ogre::Vector3(0.2f, 0.2f, 0.2f));
    _tools.push_back(ToolInfo(id, node));
    return id;
}