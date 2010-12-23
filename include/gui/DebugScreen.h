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

#include "gui/GuiPrerequisite.h"
#include "gui/Screens.h"

namespace ZGame
{
    namespace Gui
    {
        class DebugScreen : public Screens
        {
        public:
            DebugScreen(GuiController* guiCtrl);
            virtual ~DebugScreen();

           
            virtual Rocket::Core::EventListener*
                InstanceEventListener(const Rocket::Core::String& value);
            virtual void Release(){}; //do nothing
            virtual void ProcessEvent(Rocket::Core::Event& event);
            virtual const Rocket::Core::String&
                getControllerString()
            {
                return _ctrlStr;
            }


        protected:
        private:
            const Ogre::StringVector& 
                _getDocPath()
            {
                return _docPath;
            }
            virtual void
                _afterDocLoadedOnLoad();
            Ogre::StringVector _docPath;
            Rocket::Core::String _ctrlStr;
        };
    }
}