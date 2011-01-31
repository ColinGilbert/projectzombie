#include "toolset/ToolsetController.h"

using namespace ZGame::Toolset;

ToolsetController::ToolsetController(std::auto_ptr<ToolsetManager> toolMgr)
    : _toolMgr(toolMgr)
{
}

ToolsetController::~ToolsetController()
{
}

