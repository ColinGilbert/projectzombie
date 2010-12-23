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
#include <algorithm>
#include <utility>
#include "gui/GuiPrerequisite.h"
#include "gui/GuiController.h"
namespace ZGame
{

    class EngineController;
    namespace Gui
    {

        /**
        * This class represents the concept of Screens. A Screen is a ADT for a collection
        *of libRocket documents.
        **/
        class Screens : Rocket::Core::EventListenerInstancer, public Rocket::Core::EventListener
        {
        public:

            static void
                s_docShow(StrToDocumentPair pair)
            {
                pair.second->Show();
            }
            static void
                s_docHide(StrToDocumentPair pair)
            {
                pair.second->Hide();
            }

            virtual ~Screens();

            virtual Rocket::Core::EventListener*
                InstanceEventListener(const Rocket::Core::String& value) = 0;
            virtual void Release();
            virtual void ProcessEvent(Rocket::Core::Event& event) = 0;
            virtual const Rocket::Core::String& 
                getKey() = 0;
            void
                onLoad();
            virtual const Rocket::Core::String&
                getControllerString() = 0;

            const Rocket::Core::String&
                getName(){ return _name;}

            StrToDocumentMap&
                getDocumentMap()
            {
                return _docMap;
            }

            bool
                getVisible()
            {
                return _visible;
            }
            void
                setVisible(bool vis)
            {
                _visible = vis;
            }

            void
                show()
            {
                std::for_each(_docMap.begin(), _docMap.end(), Screens::s_docShow);
                setVisible(false);
            }
            void
                hide()
            {
                std::for_each(_docMap.begin(), _docMap.end(), Screens::s_docHide);
                setVisible(false);
            }

        protected:
            Screens(GuiController* guiCtrl, const Rocket::Core::String &name);
            GuiController* _guiCtrl;
            StrToDocumentMap _docMap;

            StrToDocumentMap& 
                p_buildDocMap(const Ogre::StringVector& path);
            void
                p_setName(const Rocket::Core::String& name)
            {
                _name = name;
            }
           
        private:
            Rocket::Core::String _name;
            bool _visible;
            virtual 
                const Ogre::StringVector& _getDocPath() = 0;
            virtual void
                _afterDocLoadedOnLoad() = 0;
        };
    }
}