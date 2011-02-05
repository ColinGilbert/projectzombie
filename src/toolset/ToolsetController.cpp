#include "toolset/ToolsetController.h"
#include "toolset/ToolsetManager.h"
#include "ZInitPacket.h"
#include "gui/TemplateCloner.h"
using namespace ZGame;
using namespace ZGame::Toolset;

ToolsetController::ToolsetController(std::auto_ptr<ToolsetManager> toolMgr)
    : _toolMgr(toolMgr), Rocket::Controls::DataSource("tool_source")
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
    NotifyRowAdd("tools", -1, 1);
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
/**
* This method is called for onCreate events. For this event we will create the current tool type. For selection and cursor, there can be only one in existence
*at a time, so we ignore it for those tools. We will create the tools for other types. 
*
*/
void
    ToolsetController::onCreate()
{
    ToolInfo* cubeTool = 0;
    switch(_curToolType)
    {
    case CUBE:
        cubeTool = _toolMgr->createCube();
        cubeTool->getNode()->setPosition(_toolMgr->getTool(_cursorId)->getNode()->getPosition());
        _toolMgr->setSelectionid(cubeTool->getId());
        NotifyRowAdd("tools", -1, 1);   
        break;
    default:
        break;
    }
    std::for_each(_listenerMap.begin(), _listenerMap.end(), [this](std::pair<ToolsetControllerListener*, ToolsetControllerListener*> iter) {
        iter.second->onChange(this);
    });
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
        NotifyRowChange("tools", _cursorId, 1);
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
        //_switchTool(SELECT);
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
* This method will refresh a tool view given a tool id. 
*
**/
bool
    ToolsetController::refreshToolView(Gui::ToolInfoView* toolView, int toolId)
{
    bool refreshed = false;
    if(toolId == -1 && _toolMgr->getSelectionId() != -1)
    {
        toolView->setToolInfo(_toolMgr->getTool(_toolMgr->getSelectionId()));
        refreshed = true;
    }
    else if(toolId != -1)
    {
        toolView->setToolInfo(_toolMgr->getTool(toolId));
        _toolMgr->setSelectionid(toolId);
        refreshed = true;
    }
    return refreshed;
}

/**
* This method implements Rocket::Core::Data source virtual method. It will Get the row based on internal data store of
*ToolInfo.
**/
void
    ToolsetController::GetRow(Rocket::Core::StringList& row, const Rocket::Core::String& table, int row_index,
    const Rocket::Core::StringList& columns)
{
    if(table == "tools")
    {
        for(size_t i = 0; i < columns.size(); ++i)
        {
            ToolInfo* info = _toolMgr->getTool(row_index);
            if(columns[i] == "id")
            {
                Rocket::Core::String idStr;
                Rocket::Core::TypeConverter<int, Rocket::Core::String>::Convert(info->getId(), idStr);
                row.push_back(idStr);
            }
            else if(columns[i] == "name")
            {
                row.push_back(info->getName());
            }
            else if(columns[i] == "pos")
            {
                row.push_back(Ogre::StringConverter::toString(info->getNode()->getPosition()).c_str());
            }
        }
    }
}

int
    ToolsetController::GetNumRows(const Rocket::Core::String& table)
{
    return _toolMgr->getSize();
}