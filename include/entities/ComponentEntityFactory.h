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
        * This is a factory class which will create different types of component-based entities given respective Component managers.
        *Current, the depedencies between each component for a given component-based entity is made explicity in the creation methods.
        *I know this may seem stupid, but it will work for now for prototyping. In the future, we can implmement methods that can 
        *do this in a programable way. AND such that we can have new component types register themselves for creation.
        *
        **/
        class ComponentEntityFactory
        {
        public:
            ComponentEntityFactory();
            virtual 
                ~ComponentEntityFactory();

            void
                create(const Ogre::String& typeName, 
                std::vector<Ogre::Any> &componentAnys, size_t numsToCreate);

        protected:
        private:
            void
                _createZEntities(std::vector<Ogre::Any> &componentAnys, size_t nums);
            void
                _makeConnections(Component* inComp, Component* outComp,
                const Ogre::String& inPort, const Ogre::String& outPort);
        };
    }
}