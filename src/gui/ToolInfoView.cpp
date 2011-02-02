#include "gui/ToolInfoView.h"
#include "toolset/ToolInfo.h"
#include "gui/TemplateCloner.h"

using namespace ZGame::Gui;
using ZGame::Toolset::ToolInfo;

ToolInfoView::ToolInfoView() 
    : _rootElement(0), _key("ToolInfoView"), _toolInfo(0)
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
/** 
* This method will generate the basic view for ToolInfo. Such that it will create input / ouput view for transformation attributes of this ToolInfo.
* \note Ideally we should load from a template. We are don't that now; we simply create everything in c++, for now.
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
    Rocket::Core::Element* worldPosEl = transformationEl->GetElementById("world_pos");
    if(!worldPosEl)
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, "Null pointer for worldPosEl", "ToolInfoView::_generateBasicView");
    worldPosEl->SetId(worldPosEl->GetId()+"_test");
    worldPosEl->SetInnerRML(Ogre::StringConverter::toString(_toolInfo->getNode()->getPosition()).c_str());
    rootEl->AppendChild(transformationEl);
}
