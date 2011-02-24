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


namespace ZGame
{
    namespace Entities
    {
        /**
        * This class is the controller for Components. Every instance of ZGame will have components.
        *
        **/
        class ComponentController
        {
        public:
            ComponentController(std::auto_ptr<EntitiesManager> entsMgr, 
                std::auto_ptr<RenderEntitiesManager> renderEntsMgr);
            virtual
                ~ComponentController();

            bool
                onInit(ZGame::ZInitPacket* packet);
            bool
                onDestroy();
            bool
                onUpdate(const Ogre::FrameEvent &evt);

            Entities::EntitiesManager*
                getEntitiesManager()
            {
                return _entsMgr.get();
            }

            Entities::RenderEntitiesManager*
                getRenderEntitiesManager()
            {
                return _renderEntsMgr.get();
            }

        protected:
        private:
            /** This is the manager for the ZEntities component. Every Entity will have this component.**/
            std::auto_ptr<EntitiesManager> _entsMgr;
            /** This is the component manager for Render Entities. **/
            std::auto_ptr<RenderEntitiesManager> _renderEntsMgr;

        };
    }
}