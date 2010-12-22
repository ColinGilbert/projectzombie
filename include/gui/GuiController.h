/**
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
*author: beyzend 
*email: llwijk@gmail.com
**/

#pragma once

#include "gui/GuiPrerequisite.h"
#include <Ogre.h>
#include <ZInitPacket.h>
#include <vector>
#include <utility>

namespace ZGame
{
    namespace Gui
    {
        class Screens;
        class GuiController : Rocket::Core::EventListenerInstancer
        {
        public:
            GuiController();
            ~GuiController();

            virtual Rocket::Core::EventListener*
                InstanceEventListener(const Rocket::Core::String& value);
            virtual void Release();

            void
                addScreens(Rocket::Core::Context* context, 
                Screens* screen, StrToDocumentMap &docMap);
            void
                removeScreens(Rocket::Core::Context* context, const Rocket::Core::String &key);

            bool
                onInit(ZGame::ZInitPacket initPacket);
            bool
                onUpdate(const Ogre::FrameEvent &evt);
            bool
                onRenderQueueStart(Ogre::uint8 queueGroupId,
                const Ogre::String& invocation, bool& skipThisInvocation);
            bool
                onRenderQueueEnd(Ogre::uint8 queueGroupId,
                const Ogre::String& invocation, bool& skipThisInvocation);
            bool
                onDestroy();
            bool
                onKeyUp(const OIS::KeyEvent &e);
            bool
                onKeyDown(const OIS::KeyEvent &e);
            bool
                onMouseUp(const OIS::MouseEvent &e, OIS::MouseButtonID id);
            bool
                onMouseDown(const OIS::MouseEvent &e, OIS::MouseButtonID id);
            bool
                onMouseMove(const OIS::MouseEvent& e);
            
          

            Rocket::Core::Context* 
                getGui2d()
            {
                return _gui2d;
            }


        
        private:
            void 
                ConfigureRenderSystem();
            void
                BuildProjectionMatrix(Ogre::Matrix4& projection_matrix);

            void BuildKeyMaps();
            int GetKeyModifierState();
            //This method will create the GUi2D context.
            void
                _createGui2d();

            void
                _loadDocumentsWithContext(Rocket::Core::Context* context, 
                StrToDocumentMap &docMap);

            typedef Ogre::map<OIS::KeyCode, Rocket::Core::Input::KeyIdentifier >::type KeyIdentifierMap;

            KeyIdentifierMap key_identifiers;

            SystemInterfaceOgre3D* ogre_system;
            RenderInterfaceOgre3D* ogre_renderer;
            Rocket::Core::Context* _gui2d;

            Rocket::Core::String _data_path;
            Rocket::Core::String FONT_PATH;

            std::vector<Rocket::Core::String> _fontStrVec;
            
            unsigned int _W_WIDTH;
            unsigned int _W_HEIGHT;

            Ogre::Viewport* _vp;

            OIS::Keyboard* mKeyboard;

            typedef Ogre::map<Rocket::Core::String, Screens* >::type SCREENS_MAP;
            SCREENS_MAP _screensMap;

        };
    }
}