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
        class Screens : Rocket::Core::EventListenerInstancer
        {
        public:
            virtual ~Screens();

            virtual Rocket::Core::EventListener*
                InstanceEventListener(const Rocket::Core::String& value) = 0;
            virtual void Release();

            virtual const Rocket::Core::String& 
                getKey() = 0;

            void
                onLoad();


            Rocket::Core::ElementDocument*
                getRootDoc()
            {
                return _rootDoc;
            }

        protected:
            Screens(GuiController* guiCtrl);
            
        private:

            virtual StrToDocumentMap& 
                _buildDocMap() = 0;
            virtual void
                _afterDocLoadedOnLoad() = 0;

            GuiController* _guiCtrl;
            Rocket::Core::ElementDocument* _rootDoc;
        };
    }
}