#include "toolset/ToolInfo.h"
#include "toolset/ToolXForm.h"
using namespace ZGame::Toolset;

ToolInfo::ToolInfo(Ogre::uint16 id, Ogre::SceneNode* toolNode) 
    : _id(id), _toolNode(toolNode)
{
}

ToolInfo::~ToolInfo()
{
}

void
    ToolInfo::setToolXForm(ToolXForm* toolXForm)
{
    _toolXForm = toolXForm;
}

ToolXForm*
    ToolInfo::getToolXForm()
{
    if(!_toolXForm)
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
        "_tooLXForm null pointer", "ToolInfo::getToolXForm");
    return _toolXForm;
}