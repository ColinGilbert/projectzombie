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
#include <utility>
#include "gui/GuiPrerequisite.h"
#include "gui/GuiController.h"
#include "gui/DocumentManager.h"
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

            static void s_showElement(Rocket::Core::Element* el);
            static void s_hideElement(Rocket::Core::Element* el);


            virtual ~Screens();

            virtual Rocket::Core::EventListener*
                InstanceEventListener(const Rocket::Core::String& value) = 0;
            virtual void Release();
            virtual void ProcessEvent(Rocket::Core::Event& event) = 0;
            const Rocket::Core::String& 
                getKey()
            {
                if(_key.Empty())
                    OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, 
                    "Screen key is empty", "Screens::getKey");
                return _key;
            }
            void
                onLoad();
            virtual const Rocket::Core::String&
                getControllerString() = 0;

            const Rocket::Core::String&
                getName(){ return _name;}

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
                StrToDocumentMap docMap = _docManager->getAll();
                std::for_each(docMap.begin(), docMap.end(), Screens::s_docShow);
                setVisible(true);
            }
            void
                hide()
            {   
                StrToDocumentMap docMap = _docManager->getAll();
                std::for_each(docMap.begin(), docMap.end(), Screens::s_docHide);
                setVisible(false);
            }

            DocumentManager*
                getDocManager()
            {
                return _docManager.get();
            }

        protected:
            Screens(GuiController* guiCtrl, const Rocket::Core::String &name);
            GuiController* _guiCtrl;
            std::auto_ptr<DocumentManager> _docManager;
         
            void
                p_setName(const Rocket::Core::String& name)
            {
                _name = name;
            }

            void
                p_setKey(const Rocket::Core::String& key)
            {
                _key = key;
            }
            

           
        private:
            Rocket::Core::String _name;
            Rocket::Core::String _key;
            bool _visible;
            virtual 
                const Ogre::StringVector& _getDocPath() = 0;
            virtual void
                _afterDocLoadedOnLoad() = 0;
            

        };
    }
}