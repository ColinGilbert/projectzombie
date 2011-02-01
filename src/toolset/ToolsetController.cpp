#include "toolset/ToolsetController.h"
#include "toolset/ToolsetManager.h"
#include "ZInitPacket.h"
using namespace ZGame::Toolset;

ToolsetController::ToolsetController(std::auto_ptr<ToolsetManager> toolMgr)
    : _toolMgr(toolMgr)
{
    _toolsDesc.push_back(std::make_pair("SELECT_TOOL", SELECT));
    _toolsDesc.push_back(std::make_pair("CURSOR_TOOL", CURSOR)); 
    _toolsDesc.push_back(std::make_pair("CUBE_TOOL", CUBE));
    setToolType(_toolsDesc[0].second);
}

ToolsetController::~ToolsetController()
{
}

bool
    ToolsetController::onInit(ZGame::ZInitPacket* initPacket)
{
    _cursorId = _toolMgr->createCursor();
    return true;
}

ToolsetController::ToolDescCIter
    ToolsetController::getToolDescriptions()
{
    return std::make_pair(_toolsDesc.cbegin(), _toolsDesc.cend());
}

void
    ToolsetController::setToolType(ToolType type)
{
    _switchTool(type);
}

void
    ToolsetController::_switchTool(ToolType type)
{
    _curToolType = type;
}

void
    ToolsetController::onCreate()
{

}

void
    ToolsetController::onCursorPosition3d(Ogre::Vector3 pos)
{
    if(_curToolType == CURSOR)
        _toolMgr->getTool(_cursorId)->getNode()->setPosition(pos);
}