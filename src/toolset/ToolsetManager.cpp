#include "toolset/ToolsetManager.h"
#include "geometry/GeometryManager.h"
using namespace ZGame::Toolset;
using namespace ZGame;
ToolsetManager::ToolsetManager(Geometry::GeometryManager* geoMgr)
    : Rocket::Controls::DataSource("tool_source"), _geoMgr(geoMgr), _selectionId(-1)
{
}

ToolsetManager::~ToolsetManager()
{
}


ToolInfo*
    ToolsetManager::getTool(Ogre::uint16 id)
{
    ToolInfo* ret = &_tools.at(id); //bound checking should be done here.
    _selectionId = id;
    return ret;
}

void
    ToolsetManager::refreshTool(Ogre::uint16 id)
{
    NotifyRowChange("tools", id, 1);
}
/**
* \note We should use the component system to add a Render compoent. However, component system is not finished so we do 
*this. 
**/
const Ogre::uint16
    ToolsetManager::createCursor()
{
    Ogre::uint16 id = static_cast<Ogre::uint16>(_tools.size());
    Ogre::SceneNode* node = _geoMgr->createCube(Ogre::Vector3(0.2f, 0.2f, 0.2f), "cursor");
    _tools.push_back(ToolInfo(id, node));
    NotifyRowAdd("tools", _tools.size(), 1);
    return id;
}

/**
* This method implements Rocket::Core::Data source virtual method. It will Get the row based on internal data store of
*ToolInfo.
**/
void
    ToolsetManager::GetRow(Rocket::Core::StringList& row, const Rocket::Core::String& table, int row_index,
    const Rocket::Core::StringList& columns)
{
    if(table == "tools")
    {
        for(size_t i = 0; i < columns.size(); ++i)
        {
            ToolInfo* info = getTool(row_index);
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
    ToolsetManager::GetNumRows(const Rocket::Core::String& table)
{
    return _tools.size();
}