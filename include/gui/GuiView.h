#pragma once

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
#include "ZPrerequisites.h"
#include "gui/GuiPrerequisite.h"

namespace ZGame
{
    namespace Gui
    {
        /**
        *This is the super class for Gui based View classes. 
        **/
        class GuiView
        {
        public:
            virtual VIEW_KEY
                getKey() = 0;
            virtual Rocket::Core::Element*
                getViewElement(){ return 0;}
            virtual Rocket::Core::String
                getViewRml(){return Rocket::Core::String();}
            virtual void 
                actionElementUpdate(Rocket::Core::Element* actionElement){}
            virtual ~GuiView(){}

            void
                setTempalteCloner(TemplateCloner* templateCloner)
            {
                _templateCloner = templateCloner;
            }

            TemplateCloner* 
                getTemplateCloner()
            {
                if(!_templateCloner)
                    OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, 
                    "Null pointer for template cloner", "getTemplateCloner");
                return _templateCloner;
            }


        protected:
            GuiView() : _templateCloner(0){}

        private:
            TemplateCloner* _templateCloner;


        };
    }
}