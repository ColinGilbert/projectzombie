#include "gui/HDRSettingsView.h"
#include "graphics/HDRCompositor.h"

using namespace ZGame::Gui;

HDRSettingsView::HDRSettingsView(HDRCompositor* compositor) : _theCompositor(compositor),
    _DIV_CLASS(""), _rootElement(0), _TONE_MAPPER_SELECT_ID("tone_mapper_select_id"),
    _GLARE_TYPE_SELECT_ID("glare_type_select_id")
{
}

HDRSettingsView::~HDRSettingsView()
{
}

Rocket::Core::Element*
    HDRSettingsView::getViewElement()
{
    if(_rootElement)
        return _rootElement;
    else
    {
        _generateElement();
        return _rootElement;
    }
}

/**
* \note We decided to generate the entire form in code because we didn't want to mess with dataselect for this simple select.
**/
void
    HDRSettingsView::_generateElement()
{
    using Rocket::Core::Factory;
    using Rocket::Core::Element;


    //Generate the parent.
    _rootElement = Rocket::Core::Factory::InstanceElement(0, //root
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
    attri.Set("id", _TONE_MAPPER_SELECT_ID);
    attri.Set("name", "Tone Mapper Select");
    attri.Set("onchange", "DebugController");
    attri.Set("action", "hdrHighSettingsSelect");
    Element* toneMapperSelect = Factory::InstanceElement(inputForm,
        "select", "select", attri);
    inputForm->AppendChild(toneMapperSelect);
    toneMapperSelect->RemoveReference();
    attri.Clear();

    std::vector<SELECT_OPT> optPairsVec;

    optPairsVec.push_back(std::make_pair<Rocket::Core::String, Rocket::Core::String>("TM_NONE", "0"));
    optPairsVec.push_back(std::make_pair<Rocket::Core::String, Rocket::Core::String>("TM_LINEAR", "1"));
    optPairsVec.push_back(std::make_pair<Rocket::Core::String, Rocket::Core::String>("TM_REINHARDS", "2"));
    optPairsVec.push_back(std::make_pair<Rocket::Core::String, Rocket::Core::String>("TM_REINHARDSMOD", "3"));
    optPairsVec.push_back(std::make_pair<Rocket::Core::String, Rocket::Core::String>("TM_LOG", "4"));
    optPairsVec.push_back(std::make_pair<Rocket::Core::String, Rocket::Core::String>("TM_ADAPTLOG", "5"));
    optPairsVec.push_back(std::make_pair<Rocket::Core::String, Rocket::Core::String>("TM_REINHARDLOCAL", "6"));
    optPairsVec.push_back(std::make_pair<Rocket::Core::String, Rocket::Core::String>("TM_COUNT", "7"));

    _constructSelectInput(toneMapperSelect, optPairsVec);

    //Generate GLARETYPE drop down
    attri.Set("id", _GLARE_TYPE_SELECT_ID);
    attri.Set("name", "Glare Type Select");
    attri.Set("onchange", "DebugController");
    attri.Set("action", "hdrHighSettingsSelect");
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
    HDRSettingsView::actionElementUpdate(Rocket::Core::Element* actionElement)
{
   
    cout << "actionElement id: " << actionElement->GetId().CString() << endl;
    if(_TONE_MAPPER_SELECT_ID == actionElement->GetId())
    {

        Rocket::Controls::ElementFormControlSelect *select = static_cast<Rocket::Controls::ElementFormControlSelect*>(actionElement);
        Rocket::Controls::SelectOption* option = select->GetOption(select->GetSelection());
        if(option)
        {
            cout << "Option selected: " << option->GetValue().CString() << endl;
            //Assuming if within bounds the option is correct.
            int optionNum;
            std::istringstream iss(option->GetValue().CString());
            iss >> optionNum;
            cout << "OptionNum: " << optionNum << endl;
            _theCompositor->SetToneMapper((HDRCompositor::TONEMAPPER)optionNum);
        }
        else
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Option index out of bound", "HdrSettingsView::actionElementUpdate");
        }
    }
    else if(_GLARE_TYPE_SELECT_ID == actionElement->GetId())
    {
        Rocket::Controls::ElementFormControlSelect *select = static_cast<Rocket::Controls::ElementFormControlSelect*>(actionElement);
        Rocket::Controls::SelectOption* option = select->GetOption(select->GetSelection());
        if(option)
        {
            cout << "Option selected: " << option->GetValue().CString() << endl;
            //Assuming if within bounds the option is correct.
            int optionNum;
            std::istringstream iss(option->GetValue().CString());
            iss >> optionNum;
            _theCompositor->SetGlareType((HDRCompositor::GLARETYPE)optionNum);
        }
        else
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Option index out of bound", "HdrSettingsView::actionElementUpdate");
        }
    }
    else
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Invalid action element passed in",
            "HDRSettingsView::actionElementUpdate");
    }

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



