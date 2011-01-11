#include "gui/EditorScreen.h"
#include <iostream>
using std::cout; using std::endl;
#include "gui/CineView.h"
using namespace ZGame::Gui;

EditorScreen::EditorScreen(GuiController* guiCtrl) : Screens(guiCtrl, "EditorScreen"),
    _ctrlStr("EditorScreenController")
{
    p_setKey("EditorScreen");
    _docPath.push_back("editormenu/editormenu.rml");
}

EditorScreen::~EditorScreen()
{
}

void
    EditorScreen::setCineView(std::auto_ptr<CineView> cineView)
{
    _cineView = cineView;
}

void
    EditorScreen::_afterDocLoadedOnLoad()
{
    _docManager->defineRoot(_docPath[0].c_str());
    p_setName(_docManager->getRootDocument()->GetTitle());
    hide();
}

Rocket::Core::EventListener*
    EditorScreen::InstanceEventListener(const Rocket::Core::String& value)
{
    if(_ctrlStr == value)
    {
        return this;
    }
    return 0;
}

void
    EditorScreen::ProcessEvent(Rocket::Core::Event& evt)
{
    Rocket::Core::Element* el = evt.GetCurrentElement();
    const Rocket::Core::String actionStr(el->GetAttribute<Rocket::Core::String>("action",
        ""));

}