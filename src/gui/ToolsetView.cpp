#include "gui/ToolsetView.h"
#include "toolset/ToolsetController.h"
#include "gui/GuiUtils.h"
using namespace ZGame;
using namespace ZGame::Gui;

ToolsetView::ToolsetView(Toolset::ToolsetController* toolCtrl)
    : _toolCtrl(toolCtrl), _ctrlStr("ToolsetViewController"),
    _rootElement(0), _key("ToolsetView"), _TOOL_SELECT_ID("tool_select_id")
{
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
    }
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

    optPairsVec.push_back(std::make_pair<String, String>("box", "0"));
    optPairsVec.push_back(std::make_pair<String, String>("sphere", "1"));
    optPairsVec.push_back(std::make_pair<String, String>("cylinder", "2"));

    GuiUtils::ConstructSelectOptions(cameraSelect, optPairsVec);

}


