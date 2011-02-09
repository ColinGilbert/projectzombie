#pragma once
/**
*Copyright (c) 2010 Fdastero LLC

Permission is hereby granted, free of charge, to any person obtaining a copy
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

#include "ZPrerequisites.h"
#include "gui/GuiPrerequisite.h"
#include "gui/GuiView.h"

namespace ZGame
{
    namespace Gui
    {
        /**
        * This class defines a view on ToolInfo.
        *
        **/
        class ToolInfoView : public GuiView
        {
        public:
            ToolInfoView(Rocket::Core::String ctrlStr);
            virtual ~ToolInfoView();

            //interface for GuiView
            virtual Rocket::Core::Element* 
                getViewElement();

            void
                appendViewToElement(Rocket::Core::Element* root);
            /**
            * This method will update the given view element. It is assumed that the passed in element
            *correspond to this view.
            *
            **/
            void
                refreshViewElement(Rocket::Core::Element* el);

            void setToolInfo(Toolset::ToolInfo* toolInfo)
            {
                _toolInfo = toolInfo;
            }

            virtual VIEW_KEY
                getKey(){return _key;}


        private:
            Toolset::ToolInfo* _toolInfo;
            Rocket::Core::Element* _rootElement;
            VIEW_KEY _key;
            /** String for the controller on this view.**/
            Rocket::Core::String _ctrlStr;
            /** \brief This method will generate a basic view for this ToolInfo. e.g. transformation attributes.**/
            void
                _generateBasicView(Rocket::Core::Element* rootEl);
           
        };
    }
}