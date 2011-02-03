#include "toolset/ToolsetController.h"
#include "toolset/ToolsetManager.h"
#include "ZInitPacket.h"
#include "gui/TemplateCloner.h"
using namespace ZGame;
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

int
    ToolsetController::getToolType()
{
    return _curToolType;
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

bool
    ToolsetController::isCursorMode()
{
    return _curToolType == CURSOR;
}

bool
    ToolsetController::onCursorPosition3d(Ogre::Vector3 pos)
{
    if(_curToolType == CURSOR)
    {
        _toolMgr->getTool(_cursorId)->getNode()->setPosition(pos);
        _toolMgr->refreshTool(_cursorId); //hackish
        
        std::for_each(_listenerMap.begin(), _listenerMap.end(), [this](std::pair<ToolsetControllerListener*, ToolsetControllerListener*> iter) {
            iter.second->onChange(this);
        });

        return true;
    }
   
    return false;
}

void
    ToolsetController::onSetCursor3dPosition()
{
    if(_curToolType == CURSOR)
    {
        _switchTool(SELECT);
        std::for_each(_listenerMap.begin(), _listenerMap.end(), [this](std::pair<ToolsetControllerListener*, ToolsetControllerListener*> iter) {
            iter.second->onSetCursor3dPosition(this);
            iter.second->onChange(this);
        });

    }
}

Ogre::Vector3
    ToolsetController::getCursor3dPosition()
{
    return _toolMgr->getTool(_cursorId)->getNode()->getPosition();
}

void
    ToolsetController::addListener(ToolsetControllerListener* listener)
{
    //no checking done whatsoever.
    _listenerMap[listener] = listener;
}

void
    ToolsetController::removeListener(ToolsetControllerListener* listener)
{
    ListenerMap::iterator findMe = _listenerMap.find(listener);
    if(findMe != _listenerMap.end())
    {
        _listenerMap.erase(listener);
    }
    return;
}


/**
* This method will refresh a tool view given a tool id. This method should be called each time you want to update 
*the view on a tool. DO NOT use a ToolView without calling this each time. This mean DO NOT assume anything regarding
*how the ToolInfo are stored, that's what you want to refresh everytime you use it. If you do that we can at leas guaraeentee
*that it will work for the most recent refresh. 
*
* \note WTF? This should be redesigned to not assume this. It's likely you won't have problem because the underlying storage
*will not change. Just to be safe though.
**/
Gui::ToolInfoView*
    ToolsetController::refreshToolView(Gui::ToolInfoView* toolView, int toolId)
{
    if(toolId == -1 && _toolMgr->getSelectionId() != -1)
    {
        toolView->setToolInfo(_toolMgr->getTool(_toolMgr->getSelectionId()));
    }
    else
    {
        toolView->setToolInfo(_toolMgr->getTool(toolId));
    }
    return toolView;
}