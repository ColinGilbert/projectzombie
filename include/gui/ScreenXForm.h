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
#include <algorithm>
#include "gui/GuiPrerequisite.h"
namespace ZGame
{
    namespace Gui
    {

        class ScreenTransition
        {
        public:

            ~ScreenTransition(){}
            virtual void
                step(float dt) = 0;
            virtual bool
                isDone() = 0;

        protected:
            ScreenTransition()
            {

            }

        };
        class ScreenTransitionTranslate : public ScreenTransition
        {
        public:
            ScreenTransitionTranslate() : ScreenTransition(),
                _isDone(true), _allocatedTimeInSecs(1.0f), _accumulatedT(0.0f)
            {
            }
            ~ScreenTransitionTranslate()
            {
            }

            virtual bool
                isDone()
            {
                return _isDone;
            }

            virtual void
                step(float dt);

            void
                pushTransition(Gui::Screens* from, Gui::Screens* to);


            void
                popTransition(Screens* from, Screens* to);
        private:
            bool _isDone;
            StrToDocumentMap* _fromDocs;
            StrToDocumentMap* _toDocs;
            Screens* _from;
            Screens* _to;
            Ogre::Vector2 _totalOffset;
            Ogre::Vector2 _accumulatedOffset;
            Ogre::Real _accumulatedT;
            Ogre::Real _allocatedTimeInSecs;
            int _negate;

            void
                _resetParams();


        };
    }
}