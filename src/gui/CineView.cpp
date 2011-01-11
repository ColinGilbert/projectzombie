#include "gui/CineView.h"
#include "world/CinematicController.h"

using namespace ZGame;
using namespace ZGame::Gui;

CineView::CineView(World::CinematicController* cineCtrl) : _cineCtrl(cineCtrl), _ctrlStr("CineViewController"),
    _rootElement(0), _key("CineView")
{
}

CineView::~CineView()
{
}

Rocket::Core::EventListener*
    CineView::InstanceEventListener(const Rocket::Core::String& value)
{
    if(_ctrlStr == value)
    {
        return this;
    }

    return 0;
}

void
    CineView::ProcessEvent(Rocket::Core::Event& event)
{

}

Rocket::Core::Element*
    CineView::getViewElement()
{
    if(_rootElement)
        return _rootElement;
    _generateRootElement();
    return _rootElement;
}

void
    CineView::_generateRootElement()
{
    using Rocket::Core::Factory;
}