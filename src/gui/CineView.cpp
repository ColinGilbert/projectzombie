#include "gui/CineView.h"
#include "world/CinematicController.h"
#include "ZPrerequisites.h"
#include "gui/GuiUtils.h"
using namespace ZGame;
using namespace ZGame::Gui;

CineView::CineView(World::CinematicController* cineCtrl) : _cineCtrl(cineCtrl), _ctrlStr("CineViewController"),
    _rootElement(0), _key("CineView"), _CAM_SELECT_ID("camera_select_id")
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
    using Rocket::Core::Element;
    using std::cout; using std::endl;
    Element* el = event.GetCurrentElement();
    if(_CAM_SELECT_ID == el->GetId())
    {
        Rocket::Controls::SelectOption* option = GuiUtils::getSelectOption(el);
        if(option)
        {
            World::CAMERA_ID camId;
            cout << "Option Selected: " << option->GetValue().CString() << endl;
            Rocket::Core::TypeConverter<Rocket::Core::String, World::CAMERA_ID>::Convert(option->GetValue(), camId);
            _cineCtrl->onCameraChange(camId);
        }
    }
    else
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, "Valid Element Id was not found", "CineView::ProcessEvent");
   

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
    using Rocket::Core::Element;
    using Rocket::Core::String;
    _rootElement = Rocket::Core::Factory::InstanceElement(0, 
        "div", "div", Rocket::Core::XMLAttributes());
    //generate form control
    Rocket::Core::XMLAttributes attri;
    attri.Set("id", "cineform");
    Element* inputForm = Factory::InstanceElement(0, "form", "form", attri);
    _rootElement->AppendChild(inputForm);
    inputForm->RemoveReference();
    attri.Clear();

    //generate camera select drop down
    attri.Set("id", _CAM_SELECT_ID);
    attri.Set("name", "Camera Select");
    attri.Set("onchange", _ctrlStr);
    attri.Set("action", "cameraSelect");

    Element* cameraSelect = GuiUtils::ConstructSelectInput(attri, inputForm);
    attri.Clear();

    //I'm sorry if this smells. i.first is the key, i.second is ZCameraInfo
    World::CAM_INFO_CITERS iters = _cineCtrl->getCameraInfosIterators();
     std::vector<Gui::SELECT_OPT> optPairsVec;
     for(World::CAM_INFOS::const_iterator i = iters.first; 
        i != iters.second; ++i)
    {
        Rocket::Core::String idStr;
        Rocket::Core::String nameStr;
        nameStr.Append((*i)->getName().c_str());
        //nameStr.Append(" ");
        //nameStr.Append(camInfos[i].type.c_str());
        Rocket::Core::TypeConverter<int, String>::Convert(static_cast<int>((*i)->getId()), idStr);
        optPairsVec.push_back(std::make_pair<String, String>("cam", idStr));
    }
    GuiUtils::ConstructSelectOptions(cameraSelect, optPairsVec);

}