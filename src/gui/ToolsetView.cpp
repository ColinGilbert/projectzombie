#include "gui/ToolsetView.h"
#include "toolset/ToolsetController.h"
#include "gui/GuiUtils.h"
using namespace ZGame;
using namespace ZGame::Gui;

ToolsetView::ToolsetView(Toolset::ToolsetController* toolCtrl)
    : _toolCtrl(toolCtrl), _ctrlStr("ToolsetViewController"),
    _rootElement(0), _key("ToolsetView"), _TOOL_SELECT_ID("tool_select_id"),
    _CREATE_SHAPE_ID("create_id"), _SCALE_MODE_ID("scaleMode_id"), _toolInfoView(_ctrlStr)
{
    _toolCtrl->addListener(this);
}

ToolsetView::~ToolsetView()
{
}

Rocket::Core::EventListener*
    ToolsetView::InstanceEventListener(const Rocket::Core::String& value)
{
    if(_ctrlStr == value)
        return this;
    return 0;
}

static Ogre::Real texOffset = 0.0f;

void
    ToolsetView::ProcessEvent(Rocket::Core::Event& event)
{
    using Rocket::Core::Element;
    using std::cout; using std::endl;
    using Rocket::Core::String;
    Element* el = event.GetCurrentElement();
    if(_TOOL_SELECT_ID == el->GetId())
    {
         Rocket::Controls::SelectOption* option = GuiUtils::getSelectOption(el);
         if(option)
         {
             unsigned int toolType;
             Rocket::Core::TypeConverter<Rocket::Core::String, unsigned int>::Convert(option->GetValue(), toolType);
             _toolCtrl->setToolType(static_cast<Toolset::ToolsetController::ToolType>(toolType));
             //refresh this view.
         }
    }
    else if(_CREATE_SHAPE_ID == el->GetId())
    {
        //String actionStr = GuiUtils::GetActionString(el); //No need to get action string actually.
        _toolCtrl->onCreate();
    }
    else if(_SCALE_MODE_ID == el->GetId())
    {
        
    }
    else
    {
        //Get action string.
        Rocket::Core::String actionStr = el->GetAttribute<Rocket::Core::String>("action", "");
        if(actionStr.Empty())
            return;
        Rocket::Core::StringList cmdList;
        //parse out command.
        Rocket::Core::StringUtilities::ExpandString(cmdList, actionStr, ' ');
        if(cmdList[0] == "get_tool")
        {
            if(cmdList.size() != 2)
                return; //throw exception
            int id;
            Rocket::Core::TypeConverter<Rocket::Core::String, int>::Convert(cmdList[1], id);
            std::cout << "get tool id is: " << id << std::endl;
            _refreshRightPanel(id);
        }
    }

}

void
    ToolsetView::onChange(Toolset::ToolsetController* controller)
{
    _refreshRightPanel(-1);
    _toolSelect->SetSelection(controller->getToolType());
}

void
    ToolsetView::_refreshRightPanel(int toolId)
{
    using Rocket::Core::Element;
    using Rocket::Core::ElementUtilities;
    if(!_viewPanel)
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Null pointer for view panel", "ToolsetView;:_refreshRightPanel");
    Element* rightPanel = _viewPanel->GetElementById("panel_right");
    
    if(!rightPanel)
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Null pointer for right panel", "ToolsetView::_refreshRightPanel");

    //Hack////
    if(toolId > -1)
    {
        if(rightPanel->GetNumChildren() > 0) //clean up the current elements that is in the panel because we're setting a new tool.
        {
            Element* el = rightPanel->GetChild(0);
            rightPanel->RemoveChild(el);
            Gui::GuiUtils::RemoveAllChildReferences(el);
            el->RemoveReference();
        }
    }
    //END HACK///Maybe this entire program is one big hack. 

    _toolCtrl->refreshToolView(&_toolInfoView, toolId, rightPanel);

   
}

Rocket::Core::Element*
    ToolsetView::getViewElement()
{
    if(_rootElement)
        return _rootElement;
    _generateRootElement();
    return _rootElement;
}

void
    ToolsetView::_generateRootElement()
{
}

void
    ToolsetView::updatePanel(Rocket::Core::Element* panel)
{
    //Setup the views.
    //Storing a pointer to viewPanel here is kind of hackish. May need to rethink this.
    _viewPanel = panel;
    _toolInfoView.setTempalteCloner(getTemplateCloner());

    _toolSelect = static_cast<Rocket::Controls::ElementFormControlSelect*>(panel->GetElementById(_TOOL_SELECT_ID));
    if(!select)
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Null pointer for tool select control", "ToolsetView::updatePanel");
    auto cIters = _toolCtrl->getToolDescriptions();
    for(Toolset::ToolsetController::ToolDesc::const_iterator iter = cIters.first; 
        iter != cIters.second; ++iter)
    {
        Rocket::Core::String idStr;
        Rocket::Core::TypeConverter<unsigned int, Rocket::Core::String>::Convert(static_cast<unsigned int>(iter->second), idStr);
        _toolSelect->Add(iter->first.c_str(), idStr); //WTF, it should be a Rocket::String here and not Ogre::String.
    }
}

void
    ToolsetView::ToolsetIdFormatter::FormatData(Rocket::Core::String& formatted_data, const Rocket::Core::StringList& raw_data)
{
    std::cout << "raw data size: " << raw_data.size() << std::endl;
    const Rocket::Core::String _ctrlStr("ToolsetViewController");
    //format id
    formatted_data.Append("<div onclick=\"");
    formatted_data.Append(_ctrlStr + "\" ");
    formatted_data.Append("action=\"get_tool " + raw_data[0] + "\""); //chage_tool id
    formatted_data.Append(">" + raw_data[0] + "</div>");
}