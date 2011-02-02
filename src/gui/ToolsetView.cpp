#include "gui/ToolsetView.h"
#include "toolset/ToolsetController.h"
#include "gui/GuiUtils.h"
using namespace ZGame;
using namespace ZGame::Gui;

ToolsetView::ToolsetView(Toolset::ToolsetController* toolCtrl)
    : _toolCtrl(toolCtrl), _ctrlStr("ToolsetViewController"),
    _rootElement(0), _key("ToolsetView"), _TOOL_SELECT_ID("tool_select_id")
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

void
    ToolsetView::ProcessEvent(Rocket::Core::Event& event)
{
    using Rocket::Core::Element;
    using std::cout; using std::endl;
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
         return;
    }
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

void
    ToolsetView::onChange(Toolset::ToolsetController* controller)
{
    _refreshRightPanel(-1);
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

    _toolCtrl->refreshToolView(&_toolInfoView, toolId);

    if(rightPanel->GetNumChildren() > 0)
    {
        Element* el = rightPanel->GetChild(0);
        rightPanel->RemoveChild(el);
        Gui::GuiUtils::RemoveAllChildReferences(el);
        el->RemoveReference();
    }
   _toolInfoView.appendViewToElement(rightPanel);
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
    using Rocket::Core::Factory;
    using Rocket::Core::Element;
    using Rocket::Core::String;
   
    _rootElement = Rocket::Core::Factory::InstanceElement(0, 
        "div", "div", Rocket::Core::XMLAttributes());

    //Create the tool select drop down box.
    Rocket::Core::XMLAttributes attri;
    attri.Set("id", "toolsetform");
    Element* inputForm = Factory::InstanceElement(0, "form", "form", attri);
    _rootElement->AppendChild(inputForm);
    inputForm->RemoveReference();
    attri.Clear();

    attri.Set("id", _TOOL_SELECT_ID);
    attri.Set("name", "Tool Select");
    attri.Set("onchange", _ctrlStr);
    attri.Set("action", "cameraSelect");

    Element* cameraSelect = GuiUtils::ConstructSelectInput(attri, inputForm);
    attri.Clear();

    std::vector<Gui::SELECT_OPT> optPairsVec;

    auto cIters = _toolCtrl->getToolDescriptions();
    for(Toolset::ToolsetController::ToolDesc::const_iterator iter = cIters.first; 
        iter != cIters.second; ++iter)
    {
        Rocket::Core::String idStr;
        Rocket::Core::TypeConverter<unsigned int, String>::Convert(static_cast<unsigned int>(iter->second), idStr);
        optPairsVec.push_back(std::make_pair<String, String>(iter->first.c_str(), idStr));
    }
    GuiUtils::ConstructSelectOptions(cameraSelect, optPairsVec);

}

void
    ToolsetView::updatePanel(Rocket::Core::Element* panel)
{
    //Setup the views.
    //Storing a pointer to viewPanel here is kind of hackish. May need to rethink this.
    _viewPanel = panel;
    _toolInfoView.setTempalteCloner(getTemplateCloner());

    Rocket::Controls::ElementFormControlSelect* select = static_cast<Rocket::Controls::ElementFormControlSelect*>(panel->GetElementById(_TOOL_SELECT_ID));
    if(!select)
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Null pointer for tool select control", "ToolsetView::updatePanel");
    auto cIters = _toolCtrl->getToolDescriptions();
    for(Toolset::ToolsetController::ToolDesc::const_iterator iter = cIters.first; 
        iter != cIters.second; ++iter)
    {
        Rocket::Core::String idStr;
        Rocket::Core::TypeConverter<unsigned int, Rocket::Core::String>::Convert(static_cast<unsigned int>(iter->second), idStr);
        select->Add(iter->first.c_str(), idStr); //WTF, it should be a Rocket::String here and not Ogre::String.
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