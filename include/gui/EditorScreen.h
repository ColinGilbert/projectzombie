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
        class EditorScreen : public Screens
        {
        public:
            EditorScreen(GuiController* guiCtrl);
            virtual ~EditorScreen();

            virtual Rocket::Core::EventListener*
                InstanceEventListener(const Rocket::Core::String& value);
            virtual void Release(){};
            virtual void ProcessEvent(Rocket::Core::Event& evt);
            virtual const Rocket::Core::String&
                getControllerString()
            {
                return _ctrlStr;
            }
            void
                setCineView(std::auto_ptr<Gui::CineView> cineView);
            void
                setToolsetView(std::auto_ptr<Gui::ToolsetView> toolsetView);
        protected:
        private:

             enum MAIN_TABS
            {
                PROJECT=0, WORLD, VIEW, PHYSICS, MATERIALS, GAME
            };


            const Ogre::StringVector&
                _getDocPath()
            {
                return _docPath;
            }
            virtual void
                _afterDocLoadedOnLoad();

            void
                _buildEditorTab(Rocket::Controls::ElementTabSet* tab);
            void
                _elementIsValid(Rocket::Core::Element* element);

            Ogre::StringVector _docPath;
            Rocket::Core::String _ctrlStr;

            std::auto_ptr<Gui::CineView> _cineView;
            std::auto_ptr<Gui::ToolsetView> _toolsetView;

        };
    }
}

