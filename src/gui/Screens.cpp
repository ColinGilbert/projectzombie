/**
Permission is hereby granted by Fdastero LLC, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
**/
/**
*author: beyzend 
*email: llwijk@gmail.com
**/

#include "gui/Screens.h"
#include "gui/GuiController.h"
#include "EngineController.h"
#include "gui/DocumentManager.h"
#include <ElementStyle.h>
using namespace ZGame::Gui;

Screens::Screens(GuiController* guiCtrl, const Rocket::Core::String &name) : _guiCtrl(guiCtrl), _name(name),
    _visible(false), _docManager(new DocumentManager())
{
   
}

Screens::~Screens()
{
   
}

void
    Screens::s_showElement(Rocket::Core::Element* el)
{
    if(!el)
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,"Element is not valid",
        "Screens::s_showElement");
    el->SetProperty("visibility", "visible");
    el->SetProperty("display", "block");
}
void
    Screens::s_hideElement(Rocket::Core::Element* el)
{
    if(!el)
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,"Element is not valid",
        "Screens::s_hideElement");
    el->SetProperty("visibility", "hidden");
    el->SetProperty("display", "none");
}

void
    Screens::Release()
{
}

void
    Screens::onLoad()
{
    const Ogre::StringVector& docPath = _getDocPath();
    if(docPath.empty())
        OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, "Document path is empty",
        "Screens::onLoad");
    _docManager->define(docPath);
    _guiCtrl->addScreens(_guiCtrl->getGui2d(),
        this);
    _afterDocLoadedOnLoad();
}

/**
* This method shows a menu and hides the current meu.
*
* \note We do it this way because we could get set local style property to work yet. Ask forums.
**/
void
    Screens::p_showMenu(Rocket::Core::Element* menu, Rocket::Core::Element** currentMenu)
{
    if(!menu)
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "menu is not valid",
            "DebugScreen::_showMenu");
    }
    if(*currentMenu)
    {
        s_hideElement(*currentMenu);
    }   
    s_showElement(menu);
    *currentMenu = menu;
}