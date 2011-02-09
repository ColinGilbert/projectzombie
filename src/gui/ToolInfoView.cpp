#include "gui/ToolInfoView.h"
#include "toolset/ToolInfo.h"
#include "gui/TemplateCloner.h"

using namespace ZGame::Gui;
using ZGame::Toolset::ToolInfo;

ToolInfoView::ToolInfoView(Rocket::Core::String ctrlStr)
    : _rootElement(0), _key("ToolInfoView"), _toolInfo(0), _ctrlStr(ctrlStr)
{
}


ToolInfoView::~ToolInfoView()
{
}

Rocket::Core::Element*
    ToolInfoView::getViewElement()
{
    if(!_rootElement)
    {
        _rootElement = Rocket::Core::Factory::InstanceElement(0, "div", "div", Rocket::Core::XMLAttributes());
        _generateBasicView(_rootElement);
    }
    else
    {
        _rootElement->RemoveReference();
        _rootElement = Rocket::Core::Factory::InstanceElement(0, "div", "div", Rocket::Core::XMLAttributes());
        _generateBasicView(_rootElement);
    }
    return _rootElement;
}

void
    ToolInfoView::appendViewToElement(Rocket::Core::Element* root)
{
    _generateBasicView(root);
}

void
    ToolInfoView::refreshViewElement(Rocket::Core::Element* el)
{
    if(!el)
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Null pointer for element",
        "ToolInfoView::refreshViewElement");
    Rocket::Core::Element* transformEl = el->GetElementById("transformation");
    if(!transformEl)
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Null pointer for transformation element",
        "ToolInfoView::refreshViewElement");
    Rocket::Core::Element* worldPosEl;
    worldPosEl = transformEl->GetElementById("world_pos");
    Rocket::Core::Element* scaleEl = transformEl->GetElementById("scale");
    
    if(!el || !scaleEl)
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Null pointer for worldPosEl", 
        "ToolInfoView::refreshViewElement");
    //world pos
    worldPosEl->SetInnerRML(Ogre::StringConverter::toString(_toolInfo->getNode()->getPosition()).c_str());
    //scale
    scaleEl->SetInnerRML("N/A");
}


/** 
* This method will generate the basic view for ToolInfo. Such that it will create input / ouput view for transformation attributes of this ToolInfo.
*
**/
void
    ToolInfoView::_generateBasicView(Rocket::Core::Element* rootEl)
{
    //Generate this position attributes.
    //Clone from editortemplates.
    if(!_toolInfo)
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, "Null pointer for _toolInfo", "ToolInfoView::_generateBasicView");
    Rocket::Core::Element* transformationEl = getTemplateCloner()->getCloneById("transformation"); //Exception should be thrown if invalid.
    if(!transformationEl)
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, "Null pointer for transformationEl", "ToolInfoView::_generateBasicView");
    transformationEl->SetId("transformation");
    Rocket::Core::Element* worldPosEl = transformationEl->GetElementById("world_pos");
    Rocket::Core::Element* scaleEl = transformationEl->GetElementById("scale");
    //Rocket::Core::Element* scaleButton = transformationEl->GetElementById("scalemode_id");

    if(!worldPosEl)
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, "Null pointer for worldPosEl", "ToolInfoView::_generateBasicView");
    if(!scaleEl)
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, "Null pointer for scaleEl", "ToolInfoView::_generateBasicView");
    //if(!scaleButton)
      //  OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, "Null pointer for scale mode button", "ToolInfoView::_generateBasicView");
    //World position attribute.
    worldPosEl->SetInnerRML(Ogre::StringConverter::toString(_toolInfo->getNode()->getPosition()).c_str());
    //Set scale attribute.
    scaleEl->SetInnerRML("N/A");
    //Set scale button
    //scaleButton->SetAttribute<Rocket::Core::String>("onclick", _ctrlStr);
    
    rootEl->AppendChild(transformationEl);
}
