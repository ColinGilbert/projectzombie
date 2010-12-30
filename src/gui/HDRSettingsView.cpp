#include "gui/HDRSettingsView.h"
#include "graphics/HDRCompositor.h"

using namespace ZGame::Gui;

HDRSettingsView::HDRSettingsView(HDRCompositor* compositor) : _theCompositor(compositor),
    _DIV_CLASS(""), _rootElement(0)
{
}

HDRSettingsView::~HDRSettingsView()
{
}

Rocket::Core::Element*
    HDRSettingsView::getViewElement(Rocket::Controls::ElementTabSet* parent)
{
    if(_rootElement)
        return _rootElement;
    else
    {
        _generateElement(parent);
        return _rootElement;
    }
}

void
    HDRSettingsView::actionElementUpdate(Rocket::Core::Element* actionElement)
{
}

void
    HDRSettingsView::_generateElement(Rocket::Controls::ElementTabSet* parent)
{
    using Rocket::Core::Factory;
    using Rocket::Core::Element;


    //Generate the parent.
    _rootElement = Rocket::Core::Factory::InstanceElement(parent, //root
        "div", "div", Rocket::Core::XMLAttributes()); //plain ol' div tag.
    
    //generate form control
    Rocket::Core::XMLAttributes attri;
    attri.Set("id", "hdrhighform");
    Element* inputForm = Factory::InstanceElement(_rootElement,
        "form", "form", attri);
    _rootElement->AppendChild(inputForm);
    inputForm->RemoveReference();
    attri.Clear();
    //generate TONEMAPPER drop down
    attri.Set("id", "tone_mapper_select_id");
    attri.Set("name", "tonemapper select");
    Element* toneMapperSelect = Factory::InstanceElement(inputForm,
        "select", "select", attri);
    inputForm->AppendChild(toneMapperSelect);
    toneMapperSelect->RemoveReference();
    attri.Clear();

    std::vector<SELECT_OPT> optPairsVec;

    optPairsVec.push_back(std::make_pair<Rocket::Core::String, Rocket::Core::String>("TM_NONE", "0"));
    optPairsVec.push_back(std::make_pair<Rocket::Core::String, Rocket::Core::String>("TM_LINEAR", "1"));
    optPairsVec.push_back(std::make_pair<Rocket::Core::String, Rocket::Core::String>("TM_REINHARDS", "2"));
    optPairsVec.push_back(std::make_pair<Rocket::Core::String, Rocket::Core::String>("TM_LOG", "3"));
    optPairsVec.push_back(std::make_pair<Rocket::Core::String, Rocket::Core::String>("TM_ADAPTLOG", "4"));
    optPairsVec.push_back(std::make_pair<Rocket::Core::String, Rocket::Core::String>("TM_REINHARDLOCAL", "5"));
    optPairsVec.push_back(std::make_pair<Rocket::Core::String, Rocket::Core::String>("TM_COUNT", "6"));

    _constructSelectInput(toneMapperSelect, optPairsVec);

    //Generate GLARETYPE drop down
    attri.Set("id", "glare_type_select_id");
    attri.Set("name", "glare_type_select");
    Element* glareType = Factory::InstanceElement(inputForm, 
        "select", "select", attri);
    inputForm->AppendChild(glareType);
    glareType->RemoveReference();
    optPairsVec.clear();
    optPairsVec.push_back(std::make_pair<Rocket::Core::String, Rocket::Core::String>("GT_NONE", "0"));
    optPairsVec.push_back(std::make_pair<Rocket::Core::String, Rocket::Core::String>("GT_BLUR", "1"));

    _constructSelectInput(glareType, optPairsVec);
    
}

void
    HDRSettingsView::_constructSelectInput(Rocket::Core::Element* select, 
    std::vector<SELECT_OPT>& optionPairsVec)
{
    for(size_t i = 0; i < optionPairsVec.size(); ++i)
    {
        //Here we are going to assume first is name, and second is value.
        Rocket::Core::XMLAttributes attri;
        attri.Set("name", optionPairsVec[i].first);
        attri.Set("value", optionPairsVec[i].second);
        Rocket::Core::Element* el = Rocket::Core::Factory::InstanceElement(select, "option", "option", attri);
        el->SetInnerRML(optionPairsVec[i].first);
        select->AppendChild(el);
        el->RemoveReference();
    }
}



