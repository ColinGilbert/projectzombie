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

#pragma once

#include <Ogre.h>
#include <OIS/OIS.h>
#include <Rocket/Core.h>
#include <Rocket/Controls.h>
#include <Rocket/Debugger.h>
#include <Rocket/Core/RenderInterface.h>
#include <Rocket/Core/String.h>
#include <Rocket/Core/Context.h>
//#include <Rocket/Controls/ElementFormControlInput.h>



class SystemInterfaceOgre3D;
class RenderInterfaceOgre3D;

namespace ZGame
{
    namespace Gui
    {
        typedef Rocket::Core::String VIEW_KEY;
        typedef std::pair<Rocket::Core::String, Rocket::Core::String> SELECT_OPT;
        class GuiController;
        class CineView;
        class Screens;
        class HDRSettingsView;
        typedef std::pair<Rocket::Core::String, Rocket::Core::ElementDocument* > StrToDocumentPair;
        typedef std::map<Rocket::Core::String, Rocket::Core::ElementDocument* > StrToDocumentMap;
        typedef Ogre::map<Rocket::Core::String, Screens* >::type SCREENS_MAP;
    }
}
