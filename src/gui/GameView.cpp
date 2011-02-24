#include "gui/GameView.h"
#include "world/GameController.h"
#include "ZWorkspaceController.h"

using namespace ZGame::Gui;
using namespace ZGame;
GameView::GameView(World::GameController* gameCtrl)
    :_key("GameView"), _CTRL_STR("GameViewController"), _gameCtrl(gameCtrl)
{
}

GameView::~GameView()
{
}

Rocket::Core::EventListener*
    GameView::InstanceEventListener(const Rocket::Core::String& value)
{
    if(_CTRL_STR == value)
        return this;
    return 0;
}

void
    GameView::ProcessEvent(Rocket::Core::Event& event)
{
    using Rocket::Core::Element;
    Element* el = event.GetCurrentElement();
    //Get action string
    Rocket::Core::String actionStr = el->GetAttribute<Rocket::Core::String>("action", "");
    if(actionStr.Empty())
        return;

    if(actionStr == "createEnts")
    {
        //raise this event with game controller and also workspace controller, so we can get the proper tools.
        //I know this is completely convoluted and stupid. It will do for now for prototyping.
        _gameCtrl->onBuildComponentEntity();
    }
}

void
    GameView::updatePanel(Rocket::Core::Element* panel)
{
    if(!panel)
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, 
        "Null pointer for panel", "GameView::upatePanel");
    _viewPanel = panel;

}

