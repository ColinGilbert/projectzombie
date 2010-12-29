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
#include <vector>
#include <utility>
class HDRCompositor;

namespace ZGame
{
    
    namespace Gui
    {
        /** 
        *THis class is a view on the settings for HDR. 
        *
        */
        class HDRSettingsView
        {
        public:
            HDRSettingsView(HDRCompositor* compoistor);
            ~HDRSettingsView();

            /** This method will return an element containing the view of HDRSettings.**/
            void getViewElement();

            /** \brief This method is called by a controller on an action string corresponding to this view,
            and the Element passed in is the Element of the corresponding action. **/
            void updateElement(Rocket::Core::Element* actionElement);

        private:
            Rocket::Core::String _DIV_CLASS; //class for the div element.
            void
                _generateElement();

            Rocket::Core::Element* _rootElement;

            typedef std::pair<Rocket::Core::String, Rocket::Core::String> SELECT_OPT;

            void
                _constructSelectInput(Rocket::Core::Element* select, std::vector<SELECT_OPT>& 
                optionPairsVec);

        };

    }
}