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

#include <Rocket/Core/String.h>
#include <Rocket/Core/Context.h>
#include <Ogre.h>
#include <ZInitPacket.h>
#include <vector>

class SystemInterfaceOgre3D;
class RenderInterfaceOgre3D;

namespace ZGame
{
    namespace Gui
    {
       
        class GuiController
        {
        public:
            GuiController();
            ~GuiController();

            bool
                onInit(ZGame::ZInitPacket initPacket);
            bool
                onUpdate(const Ogre::FrameEvent &evt);
            bool
                onRenderQueueStart(Ogre::uint8 queueGroupId,
                const Ogre::String& invocation, bool& skipThisInvocation);
            bool
                onDestroy();
        protected:
        private:
            void 
                ConfigureRenderSystem();
            void
                BuildProjectionMatrix(Ogre::Matrix4& projection_matrix);

            SystemInterfaceOgre3D* ogre_system;
            RenderInterfaceOgre3D* ogre_renderer;
            Rocket::Core::Context* context;

            Rocket::Core::String _data_path;
            Rocket::Core::String FONT_PATH;

            std::vector<Rocket::Core::String> _fontStrVec;
            
            unsigned int _W_WIDTH;
            unsigned int _W_HEIGHT;

            Ogre::Viewport* _vp;

        };
    }
}