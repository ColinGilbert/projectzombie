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
        class CineView : public GuiView, public Rocket::Core::EventListenerInstancer, public Rocket::Core::EventListener
        {
        public:
            CineView(World::CinematicController* cineCtrl);
            virtual ~CineView();
            //interface methods for GuiView
            /** \brief This method will returns the Rocket based element representing this view.**/
            virtual Rocket::Core::Element*
                getViewElement();
            

            //Interfaces for events
            virtual Rocket::Core::EventListener*
                InstanceEventListener(const Rocket::Core::String& value);
            virtual void
                Release(){}
            virtual void
                ProcessEvent(Rocket::Core::Event& event);

            VIEW_KEY
                getKey()
            {
                return _key;
            }

        protected:
        private:
            World::CinematicController* _cineCtrl;
            Rocket::Core::String _ctrlStr;
            Rocket::Core::Element* _rootElement;
            VIEW_KEY _key;
            Rocket::Core::String _CAM_SELECT_ID;
            void
                _generateRootElement();
        };
    }
}