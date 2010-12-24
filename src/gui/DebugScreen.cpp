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
#include "gui/DebugScreen.h"
#include <iostream>
#include "command/Command.h"
#include "command/CommandList.h"
using std::cout; using std::endl;
using namespace ZGame::Gui;

DebugScreen::DebugScreen(GuiController* guiCtrl) : Screens(guiCtrl, "DebugScreen"),_ctrlStr("DebugController"),
    _curMenu(0)
{
    p_setKey("_DEBUGSCREEN_KEY_");
    _docPath.push_back("debug/debugmain.rml");
    //Note: We do this because we couldn't get update StyleProperty to work. Undefined symbols.
    //It's not exported out fro LibRocket. It's not in public includes.
    _docPath.push_back("debug/graphics.rml");
}

DebugScreen::~DebugScreen()
{
}

void
    DebugScreen::_afterDocLoadedOnLoad()
{
    //Do nothing. This screen remains hidden on load.
    _docManager->defineRoot(_docPath[0].c_str());
    p_setName(_docManager->getRootDocument()->GetTitle());
   
    hide();
   
    //translate offscreen into place. Note: We don't know why this won't work in ScreenXForm.
    StrToDocumentMap& docMap = _docManager->getAll();
    for(StrToDocumentMap::iterator iter = docMap.begin();
        iter != docMap.end(); iter++)
    {
        Rocket::Core::Element* offsetParent = iter->second->GetOffsetParent();
        if(!offsetParent)
            cout << "Not offset parent" << endl;
        //transform offscreen. Assuming that all root documents are the have same extents. 
        float width = iter->second->GetClientWidth();
        cout << "toDocs client width: " << width << endl;
        iter->second->SetOffset(Rocket::Core::Vector2f(width, 0.0f), offsetParent, true);
    }
}

Rocket::Core::EventListener*
    DebugScreen::InstanceEventListener(const Rocket::Core::String& value)
{
    if(_ctrlStr == value)
    {
        cout << "In place DebugController is found" << endl;
        return this;
    }
    else
    {
        cout << "In place controller string passed through is: " << value.CString() << endl;
    }
    return 0;
}

void
    DebugScreen::ProcessEvent(Rocket::Core::Event& event)
{
    Rocket::Core::Element* el = event.GetCurrentElement();
    Rocket::Core::Element* tel = event.GetTargetElement();
    cout << "DebugScreen::ProcessEvent" << endl;
    cout << "Event Tag: " << el->GetTagName().CString() << endl;
    cout << "Event Type: " << event.GetType().CString() << endl;
    cout << "Event Element id: " << el->GetId().CString() << endl;
    const Rocket::Core::String actionStr(el->GetAttribute<Rocket::Core::String>("action", ""));
    cout << "Event Element action: " << actionStr.CString() << endl;
   
    if(!actionStr.Empty())
    {
        const Rocket::Core::String switchTo("switchto");
        const Rocket::Core::String switchBack("switchback");
        const Rocket::Core::String graphics("graphics");
        const Rocket::Core::String more("more");
        const Rocket::Core::String more2("more2");
        if(switchTo == actionStr)
            _guiCtrl->pushScreenTransition(getKey());
        if(switchBack == actionStr)
            _guiCtrl->popScreenTransition();
        if(graphics == actionStr)
        {
           
            //Rocket::Core::Element* menu = 
                //_docManager->get(_docPath[1].c_str())->GetElementById("body_content")->Clone();
            _showMenu(_docManager->getRootDocument()->GetElementById("graphics_menu"));
        }
        if(more == actionStr)
        {
            _showMenu(_docManager->getRootDocument()->GetElementById("test1_menu"));
        }
        if(more2 == actionStr)
        {
            _showMenu(_docManager->getRootDocument()->GetElementById("test2_menu"));
        }
            
        
    }
    else
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Action string is empty",
        "DebugScreen::ProcessEvent");
}
/**
* This method shows a menu and hides the current meu.
*
* \note We do it this way because we could get set local style property to work yet. Ask forums.
**/
void
    DebugScreen::_showMenu(Rocket::Core::Element* menu)
{
    if(!menu)
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "menu is not valid",
            "DebugScreen::_showMenu");
    }
    Rocket::Core::ElementDocument* doc = _docManager->getRootDocument();
    Rocket::Core::Element* theMenu = doc->GetElementById("content_area");
    //find menu
    Rocket::Core::String menuInnerRml;
    menu->GetInnerRML(menuInnerRml);
    theMenu->SetInnerRML(menuInnerRml);
    
}