#include "toolset/ToolInfo.h"

using namespace ZGame::Toolset;

ToolInfo::ToolInfo(Ogre::uint16 id, Ogre::SceneNode* toolNode) 
    : _id(id), _toolNode(toolNode)
{
}

ToolInfo::~ToolInfo()
{
}


