#pragma once
/**
*Copyright (c) 2010 Fdastero LLC

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

#include "world/Control.h"

namespace ZGame
{
    namespace World
    {
        class PerspectiveControl : public Control
        {
        public:
            PerspectiveControl();
            virtual ~PerspectiveControl();

            /** This method will yaw control a given camera by the fraction of some internal unit.**/
            virtual void
                yaw(Ogre::Real fraction, Ogre::Camera* cam, Ogre::Node* node);
            virtual void
                pitch(Ogre::Real fraction, Ogre::Camera* cam, Ogre::Node* node);
            /** This method will translate a given camera by the fraction of some internal unit. **/
            virtual void
                translate(Ogre::Real fraction, Ogre::Camera* cam, Ogre::Node* node);
            /** This method will dolly control a given camera by the fraction of some internal unit. **/
            virtual void
                dolly(Ogre::Real fraction, Ogre::Camera* cam, Ogre::Node* node);
        protected:
        private:
        };
    }
}