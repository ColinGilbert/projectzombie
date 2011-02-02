#include "gui/TemplateCloner.h"

using namespace ZGame::Gui;

TemplateCloner::TemplateCloner()
{
}

TemplateCloner::~TemplateCloner()
{
}

Rocket::Core::Element*
    TemplateCloner::getCloneById(const Rocket::Core::String& id)
{
    if(!_templateDoc)
        OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, "The template document is null pointer", "TemplateCloner::getCloneById");
    //find and clone the element.
    Rocket::Core::Element* retEl = _templateDoc->GetElementById(id);

    if(!retEl)
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "The element with the given ID was not found in this template document", 
        "TemplateCloner::getCloneById");
    return retEl->Clone();
}