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
#include "gui/SceensFactory.h"
#include "gui/DebugScreen.h"
#include "gui/MainMenuScreen.h"

using namespace ZGame::Gui;

Screens*
    ScreensFactory::createScreens(const Rocket::Core::String &screensClass,
    GuiController* guiCtrl)
{
    using Rocket::Core::String;

    const String MainMenuScreen("MainMenuScreen");
    const String DebugScreen("DebugScreen");

    if(MainMenuScreen == screensClass)
    {
        return new Gui::MainMenuScreen(guiCtrl);
    }
    else if(DebugScreen == screensClass)
    {
        return new Gui::DebugScreen(guiCtrl);
    }

    return 0;

}