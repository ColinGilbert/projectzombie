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
#include "entities/Component.h"
#include "delegates/EntityDelegates.h"

namespace ZGame
{
    namespace Entities
    {
        /**
        * This class is a component which represents a Render entity. It is a rendering based component.
        *
        **/
        class RenderEntityComp : public Component
        {
        public:
            RenderEntityComp(const ZEntityResource* res, Ogre::SceneManager* scnMgr, Ogre::SceneNode* root);
            virtual ~RenderEntityComp();

            virtual const fastdelegate::DelegateMemento
                getInputDM(const Ogre::String &memName);

            virtual void
                addOutputDM(const Ogre::String &memName, const fastdelegate::DelegateMemento dm);

            virtual void
                deleteOutputDM(const Ogre::String &memName, const fastdelegate::DelegateMemento dm);

            bool
                onRead(const Ogre::Vector3 &pos, const Ogre::Quaternion &orient);

            void
                removeAll(Ogre::SceneManager* scnMgr);

            void
                onEvent(unsigned int event);


        private:
            Ogre::SceneNode* _node;

            void
                _create(const ZEntityResource *res, Ogre::SceneManager* scnMgr, Ogre::SceneNode* root);
        };
    }
}