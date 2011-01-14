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

#include "world/PerspectiveControl.h"

using namespace ZGame::World;

PerspectiveControl::PerspectiveControl()
{
}

PerspectiveControl::~PerspectiveControl()
{
}

void
    PerspectiveControl::yaw(Ogre::Real fraction, Ogre::Camera* cam, Ogre::Node* node)
{
    //For perspective yaw, we're going to use node as the center of mass to rotate about. 
    using Ogre::Quaternion;

    Ogre::Radian dr = Ogre::Radian(Ogre::Degree(1.0f*fraction));
    Quaternion offsetQuat = Quaternion(dr, Ogre::Vector3::UNIT_Y);

    cam->rotate(node->getOrientation() * offsetQuat);
}

void
    PerspectiveControl::pitch(Ogre::Real fraction, Ogre::Camera* cam, Ogre::Node* node)
{

}

void
    PerspectiveControl::translate(Ogre::Real fraction, Ogre::Camera* cam, Ogre::Node* node)
{
}

void
    PerspectiveControl::dolly(Ogre::Real fraction, Ogre::Camera* cam, Ogre::Node* node)
{
    cam->moveRelative(Ogre::Vector3::UNIT_Z*fraction);
}

