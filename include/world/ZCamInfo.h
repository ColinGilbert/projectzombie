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

#include "ZPrerequisites.h"

namespace ZGame
{
    namespace World
    {
        class ZCameraInfo
        {
        public:
            ZCameraInfo(CAMERA_ID theid, Ogre::String types,
                Ogre::String names, Control* ctrl, Ogre::Camera* cam)
                : id(theid), type(types), name(names), _control(ctrl), _camera(cam)
            {
            }

           
            const Ogre::String&
                getName() const
            {
                return name;
            }

            const CAMERA_ID
                getId() const
            {
                return id;
            }

            Control*
                getControl()
            {
                return _control;
            }

            Ogre::Camera*
                getCamera()
            {
                return _camera;
            }
        private:
            Ogre::String type;
            Ogre::String name;
            CAMERA_ID id;
            Ogre::Camera* _camera;
            Control* _control;


        };
    }
}