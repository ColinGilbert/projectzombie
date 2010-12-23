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



        class ScreenTransition
        {
        public:
           

            virtual void
                step(float dt) = 0;
            virtual bool
                isDone();

        protected:
             ScreenTransition(SCREENS_MAP* screenMap, const Rocket::Core::String& transitionToKey)
                : _screenMap(screenMap), _transToKey(transitionToKey)
            {

            }
            SCREENS_MAP* _screenMap;
            Rocket::Core::String _transToKey;
        };
        class ScreenTransitionTranslate : ScreenTransition
        {
            ScreenTransitionTranslate(SCREENS_MAP* screenMap, 
                const Rocket::Core::String& transitionToKey) : ScreenTransition(screenMap, transitionToKey),isDone(false)
            {
            }

            virtual void
                step(float dt)
            {
            }

        private:
            bool isDone;
            std::vector<Screens*> _visibleScreens;
      
        };
    }
}