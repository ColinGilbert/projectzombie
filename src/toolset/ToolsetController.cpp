#include "toolset/ToolsetController.h"
#include "toolset/ToolsetManager.h"
#include "ZInitPacket.h"
#include "gui/TemplateCloner.h"
#include "toolset/CubeToolXForm.h"
#include "world/WorldController.h"
using namespace ZGame;
using namespace ZGame::Toolset;

ToolsetController::ToolsetController(std::auto_ptr<ToolsetManager> toolMgr)
    : _toolMgr(toolMgr), Rocket::Controls::DataSource("tool_source"), _curToolsetMode(MOVECURSOR),
    _cubeSelectXForm(new CubeToolXForm()), _materialId(1)
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
    _cursorBlueId = _toolMgr->createBlueCursor();
    NotifyRowAdd("tools", -1, 1);
    _cursorYellowId = _toolMgr->createBlueCursor(Ogre::Vector3(1.0f, 1.0f, 1.0f));
    _toolMgr->getTool(_cursorYellowId)->setToolXForm(_cubeSelectXForm);
    //We won't inform row add because we want to hide it from selection.
    return true;
}

ToolsetController::ToolDescCIter
    ToolsetController::getToolDescriptions()
{
    return std::make_pair(_toolsDesc.cbegin(), _toolsDesc.cend());
}

void
    ToolsetController::setToolsetMode(ToolsetMode mode)
{
    _curToolsetMode = mode;
    switch(_curToolsetMode)
    {
    case SCALE:
        setToolType(CURSOR);
        break;
    default:
        break;//do nothing
    }
}

int
    ToolsetController::getToolsetMode()
{
    return _curToolsetMode;
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
    _informListeners(ONCHANGE_EVENT);
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
        cubeTool->getNode()->setPosition(_toolMgr->getTool(_cursorBlueId)->getNode()->getPosition());
        _toolMgr->setSelectionid(cubeTool->getId());
        NotifyRowAdd("tools", -1, 1);   
        break;
    default:
        break;
    }
    _informListeners(ONCHANGE_EVENT);
}

bool
    ToolsetController::isCursorMode()
{
    return _curToolType == CURSOR;
}
void
    ToolsetController::_informListeners(unsigned int event)
{
  for(auto iter = _listenerMap.begin(); iter != _listenerMap.end(); ++iter)
    {
      if(event & ToolsetController::ONCHANGE_EVENT)
            iter->second->onChange(this);
        if(event & ToolsetController::ONCURSOR3DPOSITION_EVENT)
            iter->second->onSetCursor3dPosition(this);
    }
  /* //Let's not use lambda. On one of my boxes g++4.4 is the default. I don't feel like going through the trouble of recompile this project's depdencies using
     //gcc 4.5
    std::for_each(_listenerMap.begin(), _listenerMap.end(), [this, event](std::pair<ToolsetControllerListener*, ToolsetControllerListener*> iter) {
        if(event & ToolsetController::ONCHANGE_EVENT)
            iter.second->onChange(this);
        if(event & ToolsetController::ONCURSOR3DPOSITION_EVENT)
            iter.second->onSetCursor3dPosition(this);
    });
  */
}

bool
    ToolsetController::onCursorPosition3d(Ogre::Vector3 pos)
{
    if(_curToolType == CURSOR && _curToolsetMode == MOVECURSOR)
    {
        _toolMgr->getTool(_cursorId)->getNode()->setPosition(pos);
        _informListeners(ONCHANGE_EVENT);
        NotifyRowChange("tools", _cursorId, 1);
        return true;
    }
    else if(_curToolType == CURSOR && _curToolsetMode == SCALE)
    {
        //In this mode, we want to do scale operation with the cursor. Since we are in SELECT mode,
        //the anchor will be _cursorBlueId's tool. This mean we are scaling the blue cursor in SELECT mode. 
        ToolInfo* cursor = _toolMgr->getTool(_cursorId);
        cursor->getNode()->setPosition(pos);
        ToolInfo* blueCursor = _toolMgr->getTool(_cursorBlueId);
        ToolInfo* yellowCursor = _toolMgr->getTool(_cursorYellowId);
        yellowCursor->getToolXForm()->scaleApply(yellowCursor->getNode(), blueCursor->getNode()->getPosition(), cursor->getNode()->getPosition());

    }
    //Else we are scaling some sort of geometric tool. We should have that ID, and the corresponding tool will be used
    //as the anchor.


    return false;
}

void
    ToolsetController::onSetCursor3dPosition(World::WorldController* worldCtrl)
{
    using std::cout; using std::endl;
    if(_curToolType == CURSOR && _curToolsetMode == MOVECURSOR)
    {
        //Move the blue cursor to the position
        _toolMgr->getTool(_cursorBlueId)->getNode()->setPosition(
            _toolMgr->getTool(_cursorId)->getNode()->getPosition());
        _toolMgr->getTool(_cursorYellowId)->getNode()->setPosition(
            _toolMgr->getTool(_cursorId)->getNode()->getPosition());
        _informListeners(ONCHANGE_EVENT | ONCURSOR3DPOSITION_EVENT);
    }
    else if(_curToolType == CURSOR && _curToolsetMode == SCALE)
    {
        //stop scale mode.
        setToolsetMode(MOVECURSOR);
        setToolType(SELECT);
        //compute AABB from anchor (blue cusor) and cursor. 
        ToolInfo* cursor = _toolMgr->getTool(_cursorId);
        ToolInfo* anchor = _toolMgr->getTool(_cursorBlueId);
        ToolInfo* select = _toolMgr->getTool(_cursorYellowId);

        //output AABB of yellow cursor which is the selection
        Ogre::AxisAlignedBox selectAABB;
        unsigned int whichCorner; //kind of redundant
        
        _computeSelectAABB(selectAABB, cursor->getNode()->getPosition(), anchor->getNode()->getPosition(),
            whichCorner);

        cout << "computed Select AABB: " << selectAABB << endl;
        cout << "min AABB: " << selectAABB.getCorner(Ogre::AxisAlignedBox::NEAR_LEFT_BOTTOM) << endl;
        worldCtrl->onSelectionRegion(selectAABB, _materialId);

    }
}

void
    ToolsetController::onMaterialChange(size_t matId)
{
    _materialId = matId;
}

bool
    ToolsetController::getConstraintPlane(const Ogre::Ray &ray, Ogre::Plane &plane)
{
    using namespace Ogre;
    ToolInfo* cursor = _toolMgr->getTool(_cursorId);
    ToolInfo* anchor = _toolMgr->getTool(_cursorBlueId);
 
    /*
    *This perhaps is a dumb method. All we're doing, we're determine which plane to constraint to based on view direction of ray.
    */

    Ogre::Vector3 normalZ = Ogre::Vector3::UNIT_Z;
    Ogre::Vector3 normalX = Ogre::Vector3::UNIT_X;
    
    Ogre::Plane zPlane(normalZ, cursor->getNode()->getPosition());
    Ogre::Plane xPlane(normalX, cursor->getNode()->getPosition());


    //project view vector into z,x plane.
    Ogre::Vector3 view = ray.getDirection();
    view.y = 0.0f;
    if(view.squaredLength() <= 0.001f)
        return false;
    
    if(view.z < 0.0f)
        view *= -1.0f;
    Ogre::Radian rad = Ogre::Math::ATan2(view.x, view.z);
    Ogre::Radian pifourth(Ogre::Math::PI / 4.0f);
    if(rad >= -Ogre::Radian(pifourth) && rad <= Ogre::Radian(pifourth))
    {
        plane = zPlane;
    }
    else
        plane = xPlane;

    return true;
}

void
    ToolsetController::_computeSelectAABB(Ogre::AxisAlignedBox &aabb, const Ogre::Vector3 &cursor, 
    const Ogre::Vector3 &anchor, unsigned int &whichCorner)
{
    using namespace Ogre;
    //We are going to assume everything is in CUBE units. By extent we mean the min / max of the AABB, in world
    //coordinates.
    //Depending on quadrant move the extents.
    Ogre::Vector3 leftExtent, rightExtent;
    //Quad. I. 
    if(cursor.x >= anchor.x && cursor.z <= anchor.z)
    {
        leftExtent = anchor; rightExtent = cursor;
        if(cursor.y < anchor.y)
        {
            leftExtent.y = cursor.y;
            rightExtent.y = anchor.y;
        }
        whichCorner = AxisAlignedBox::NEAR_RIGHT_TOP;
    }
    //Quad. II
    else if(cursor.x < anchor.x && cursor.z <= anchor.z)
    {
        leftExtent = cursor; rightExtent = anchor;
        leftExtent.z = anchor.z; rightExtent.z = cursor.z;
        leftExtent.y = anchor.y; rightExtent.y = cursor.y;
        if(cursor.y < anchor.y)
        {
            rightExtent.y = anchor.y;
        }
        whichCorner = AxisAlignedBox::NEAR_LEFT_TOP;
    }
    //Quad. III
    else if(cursor.x < anchor.x && cursor.z > anchor.z)
    {
        leftExtent = cursor; rightExtent = anchor;
        if(cursor.y >= anchor.y)
        {
            leftExtent.y = anchor.y;
            rightExtent.y = cursor.y;
        }
        whichCorner = AxisAlignedBox::FAR_LEFT_TOP;
    }
    //Quad. IV
    else if(cursor.x >= anchor.x && cursor.z > anchor.z)
    {
        leftExtent = anchor; rightExtent = cursor;
        leftExtent.z = cursor.z; rightExtent.z = anchor.z;
        if(cursor.y < anchor.y)
        {
            leftExtent.y = cursor.y;
            rightExtent.y = anchor.y;
        }
        whichCorner = AxisAlignedBox::FAR_RIGHT_TOP;
    }
    aabb.setExtents(leftExtent, rightExtent);
}


Ogre::Vector3
    ToolsetController::getCursor3dPosition()
{
    return _toolMgr->getTool(_cursorBlueId)->getNode()->getPosition();
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
void
    ToolsetController::refreshToolView(Gui::ToolInfoView* toolView, int toolId,
    Rocket::Core::Element* rootElement)
{
    if(toolId == -1 && _toolMgr->getSelectionId() != -1)
    {
        toolView->setToolInfo(_toolMgr->getTool(_toolMgr->getSelectionId()));
        toolView->refreshViewElement(rootElement);
    }
    else if(toolId != -1)
    {
        toolView->setToolInfo(_toolMgr->getTool(toolId));
        toolView->appendViewToElement(rootElement);
        _toolMgr->setSelectionid(toolId);    
    }
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

