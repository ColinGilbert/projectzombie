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
#include "gui/MainMenuScreen.h"
#include <iostream>
#include "command/Command.h"
#include "command/CommandList.h"
using std::cout; using std::endl;
using namespace ZGame::Gui;

MainMenuScreen::MainMenuScreen(GuiController* guiCtrl) : Screens(guiCtrl, "MainMenuScreen"),
    _ctrlStr("MainMenuController")
{
    p_setKey("__MainMenuScreenKey__");
    _docPath.push_back("mainmenu/mainmenu.rml");
}

MainMenuScreen::~MainMenuScreen()
{
}



void
    MainMenuScreen::_afterDocLoadedOnLoad()
{
    _docManager->defineRoot(_docPath[0].c_str()); //define the root document to be first document in doc path. 
    p_setName(_docManager->getRootDocument()->GetTitle()); //set the root document name
    show();
    //Add persistent menus
    Rocket::Core::Element* content = _docManager->get(_docPath[0].c_str())->GetElementById("content_bar");
    
    if(content)
        _guiCtrl->addPersistentScreenButtons(content);
    else
        OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, "content_bar not found",
        "MainMenuScreen::_afterDocLoadedOnLoad");
    _docPath.clear(); //Do not expect to use this again. It's private!
}



Rocket::Core::EventListener*
    MainMenuScreen::InstanceEventListener(const Rocket::Core::String& value)
{
    if(_ctrlStr == value) //where is stringCompare???
    {
        cout << "In place MainMenuController controller is found" << endl;
        return this;
    }
    else
    {
        cout << "In place event string passed through is: " << value.CString() << endl;
    }
    return 0;
}

void
    MainMenuScreen::ProcessEvent(Rocket::Core::Event& event)
{
    Rocket::Core::Element* el = event.GetCurrentElement();
    Rocket::Core::Element* tel = event.GetTargetElement();
    cout << "MainMenuScreen::ProcessEvent" << endl;
    cout << "Event Tag: " << el->GetTagName().CString() << endl;
    cout << "Event Type: " << event.GetType().CString() << endl;
    cout << "Event Element id: " << el->GetId().CString() << endl;
    const Rocket::Core::String actionStr(el->GetAttribute<Rocket::Core::String>("action", ""));
    cout << "Event Element action: " << actionStr.CString() << endl;
    if(!actionStr.Empty())
    {

        const Rocket::Core::String start("start");
        const Rocket::Core::String load("load");
        const Rocket::Core::String quit("quit");

        if(actionStr == start)
        {
            COMMAND::StringCommand cmdStart(COMMAND::CommandList::ENGINECTRL_CMD, 2);
            cmdStart.push_back("switchstate");
            cmdStart.push_back("mainmenu");
            cmdStart.executeThis();
        }
        else if(actionStr == load)
        {
            
        }
        else if(actionStr == quit)
        {          
            COMMAND::StringCommand cmdQuit(COMMAND::CommandList::ENGINECTRL_CMD);
            cmdQuit.push_back("quit");
            cmdQuit.executeThis();
        }

    }
    else
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Action string is empty.", "MainMenuScreen::ProcessEvent");
}