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
#include "gui/GuiView.h"


namespace ZGame
{
    namespace Gui
    {
        /**
        * This class defines a GUIView on Game. (See the Game panel in EditorScreen.)
        **/
        class GameView : public GuiView, public Rocket::Core::EventListenerInstancer, 
            public Rocket::Core::EventListener
        {
        public:
            /** \brief this is a temp. hack so we can test somethings **/
            GameView(World::GameController* gameCtrl); 
            virtual ~GameView();

            /** \brief WARNING HACK BAD **/
            virtual Rocket::Core::Element*
                getViewElement(){return 0;}

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

            /** \note this shoudl be addded to the base GuiView class**/
            void
                updatePanel(Rocket::Core::Element* panel);


        private:
            VIEW_KEY _key;
            World::GameController* _gameCtrl;
            Rocket::Core::String _CTRL_STR;
            Rocket::Core::Element* _viewPanel;
        };
    }
}