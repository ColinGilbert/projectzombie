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

DebugScreen::DebugScreen(GuiController* guiCtrl) : Screens(guiCtrl, "DebugScreen"),
    _key("_DEBUGSCREEN_KEY_"), _ctrlStr("DebugController")
{
    _docPath.push_back("debug/debugmain.rml");
}

DebugScreen::~DebugScreen()
{
}

void
    DebugScreen::_afterDocLoadedOnLoad()
{
    //Do nothing. This screen remains hidden on load.
    hide();
    p_setName(_docMap[_docPath[0]]->GetTitle());
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

    }
    else
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Action string is empty",
        "DebugScreen::ProcessEvent");
}