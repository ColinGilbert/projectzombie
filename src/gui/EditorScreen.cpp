#include "gui/EditorScreen.h"
#include <iostream>
using std::cout; using std::endl;
#include "gui/CineView.h"
#include "gui/ToolsetView.h"
#include "gui/GameView.h"
using namespace ZGame::Gui;

EditorScreen::EditorScreen(GuiController* guiCtrl) : Screens(guiCtrl, "EditorScreen"),
    _ctrlStr("EditorScreenController")
{
    p_setKey("EditorScreen");
    _docPath.push_back("editormenu/editormenu.rml");
    _docPath.push_back("editormenu/editortemplates.rml");
}

EditorScreen::~EditorScreen()
{
}

void
    EditorScreen::setCineView(std::auto_ptr<CineView> cineView)
{
    _cineView = cineView;
    p_registerEventInstancer(_cineView.get());
}

void
    EditorScreen::setToolsetView(std::auto_ptr<ToolsetView> toolsetView)
{
    _toolsetView = toolsetView;
    p_registerEventInstancer(_toolsetView.get());
}

void
    EditorScreen::setGameView(std::auto_ptr<GameView> gameView)
{
    _gameView = gameView;
    p_registerEventInstancer(_gameView.get());
}

void
    EditorScreen::_elementIsValid(Rocket::Core::Element* element)
{
    if(!element)
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,"Null element",
        "EditorScreen::_elementIsValid");
}

void
    EditorScreen::_buildEditorTab(Rocket::Controls::ElementTabSet* tab)
{
    using Rocket::Core::Element;

    Element* panel = tab->GetElementById("project_panel");
    _elementIsValid(panel);
    panel = tab->GetElementById("world_panel");
    _elementIsValid(panel);
    panel = tab->GetElementById("view_panel");
    _elementIsValid(panel);
    if(!_cineView.get())
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Invalid pointer for CineView", "EditorScreen::_buildEditorTab");
    panel->AppendChild(_cineView->getViewElement());
    panel = tab->GetElementById("physics_panel");
    _elementIsValid(panel);
    panel = tab->GetElementById("materials_panel");
    _elementIsValid(panel);
    panel = tab->GetElementById("game_panel");
    _elementIsValid(panel);
    panel = tab->GetElementById("toolset_panel");
    _elementIsValid(panel);
    if(!_toolsetView.get())
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Invalid pointer for ToolsetView", "EditorScreen::_buildEditorTab");
    _toolsetView->updatePanel(panel);
    //panel->AppendChild(_toolsetView->getViewElement());
}

void
    EditorScreen::_afterDocLoadedOnLoad()
{
    _docManager->defineRoot(_docPath[0].c_str());
    p_setName(_docManager->getRootDocument()->GetTitle());
    _docManager->get(_docPath[1].c_str())->Hide();
    _docManager->get(_docPath[1].c_str())->Blur();
    //Setup the views
    //We will need to setup a global view handling in Screens
    if(!_cineView.get())
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Null cineView pointer", "EditorScreen::_afterDocLoadedOnLoad");

    _templateCloner.setTemplateDoc(_docManager->get(_docPath[1].c_str()));

    _toolsetView->setTempalteCloner(&_templateCloner);

    //We will implement global view control in Screens super-class. For now just test it.
    Rocket::Controls::ElementTabSet* tab = static_cast<Rocket::Controls::ElementTabSet*>(_docManager->getRootDocument()
        ->GetElementById("main_tabs"));
    _buildEditorTab(tab);

    hide();

}

Rocket::Core::EventListener*
    EditorScreen::InstanceEventListener(const Rocket::Core::String& value)
{
    if(_ctrlStr == value)
    {
        return this;
    }
    return Screens::InstanceEventListener(value);
}

void
    EditorScreen::ProcessEvent(Rocket::Core::Event& evt)
{
    Rocket::Core::Element* el = evt.GetCurrentElement();
    const Rocket::Core::String actionStr(el->GetAttribute<Rocket::Core::String>("action",
        ""));

}