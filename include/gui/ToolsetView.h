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
/**
* \author beyzend
* \email llwijk@gmail.com
**/

#include "ZPrerequisites.h"
#include "gui/GuiPrerequisite.h"
#include "gui/GuiView.h"
#include "gui/ToolInfoView.h"
#include "toolset/ToolsetController.h"
namespace ZGame
{
    namespace Gui
    {
        /** 
        * This class defines a GuiView on Toolsets.
        *
        **/
        class ToolsetView : public GuiView, public Rocket::Core::EventListenerInstancer, 
            public Rocket::Core::EventListener, public Toolset::ToolsetControllerListener
        {
        public:
            ToolsetView(Toolset::ToolsetController* toolCtrl);
            virtual ~ToolsetView();

            virtual Rocket::Core::Element*
                getViewElement();

            //interface for events.
            virtual Rocket::Core::EventListener*
                InstanceEventListener(const Rocket::Core::String& value);
            virtual void
                Release(){}
            virtual void
                ProcessEvent(Rocket::Core::Event& event);

            virtual VIEW_KEY
                getKey()
            {
                return _key;
            }

            void
                updatePanel(Rocket::Core::Element* panel);

          
            void
                onChange(Toolset::ToolsetController* controller);


        private:

            class ToolsetIdFormatter : public Rocket::Controls::DataFormatter
            {
            public:
                ToolsetIdFormatter() : Rocket::Controls::DataFormatter("tool_id_formatter")
                {}
                ~ToolsetIdFormatter(){}
                //Implements data formatter
                virtual void
                    FormatData(Rocket::Core::String& formatted_data, const Rocket::Core::StringList& raw_data);
            };

            Toolset::ToolsetController* _toolCtrl;
            Rocket::Core::String _ctrlStr;
            Rocket::Core::Element* _rootElement;
            Rocket::Controls::ElementFormControlSelect* _toolSelect;
            VIEW_KEY _key;
            Rocket::Core::String _TOOL_SELECT_ID;
            Rocket::Core::String _CREATE_SHAPE_ID;
            Rocket::Core::String _SCALE_MODE_ID;
            ToolsetIdFormatter _idFormatter;

            ToolInfoView _toolInfoView; //generic toolInfoView.
            Rocket::Core::Element* _viewPanel;
            void
                _generateRootElement();
            void
                _refreshRightPanel(int toolId);
        };
    }
}