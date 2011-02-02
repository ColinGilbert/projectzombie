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
using std::cout; using std::endl;


/*
*Note: This piece of code may not have the best of implementation. Maybe we should use quaternions 
*instead of appending to Radians, then creating quaternion from Radians. Using quaternion the numbers 
*will always be periodic, but using Radians we may have overflow.
*
*/

PerspectiveControl::PerspectiveControl() : _currentRadYaw(0.0f), _currentRadPitch(0.0f),
    _distance(1.0f)
{

}

PerspectiveControl::~PerspectiveControl()
{
}

void
    PerspectiveControl::reset()
{
    _currentRadYaw = 0.0f;
    _currentRadPitch = 0.0f;
    _distance = 1.0f;
}
void
    PerspectiveControl::resetDistance()
{
    _distance = 1.0f;
}
void
    PerspectiveControl::yaw(Ogre::Real fraction, Ogre::SceneNode* camNode,
    Ogre::SceneNode* lookAtNode)
{
    //For perspective yaw, we're going to ASSUME camNode's has a parent node node and that it will represen the center of mass, that is,
    //all rotations are done in local space of camNode.
    using Ogre::Quaternion;
    _currentRadYaw += Ogre::Radian(0.015f * fraction);
}

void
    PerspectiveControl::pitch(Ogre::Real fraction, Ogre::SceneNode* camNode, Ogre::SceneNode* lookAtNode)
{
    _currentRadPitch += Ogre::Radian(0.015f * fraction);
}
void
    PerspectiveControl::translate(Ogre::Real fraction, Ogre::SceneNode* camNode, Ogre::SceneNode* lookAtNode)
{
}

void
    PerspectiveControl::dolly(Ogre::Real fraction, Ogre::SceneNode* camNode, Ogre::SceneNode* lookAtNode)
{
    Ogre::Camera* cam = static_cast<Ogre::Camera*>(camNode->getAttachedObject(0));
    _distance += fraction;
  }

void
    PerspectiveControl::update(Ogre::Real dt, Ogre::Camera* cam, Ogre::SceneNode* camNode, Ogre::SceneNode* lookAtNode)
{
    using Ogre::Quaternion;
    camNode->setPosition(0.0f, 0.0f, 0.0f);
    cam->setPosition(0.0f, 0.0f, 0.0f);
    Quaternion yQuat = Quaternion(_currentRadYaw, Ogre::Vector3::UNIT_Y);
    Quaternion pQuat = yQuat * Quaternion(_currentRadPitch, Ogre::Vector3::UNIT_X);
    camNode->setOrientation(yQuat * pQuat);
    camNode->setPosition(camNode->getOrientation() *  Ogre::Vector3(0.0f, 0.0f, _distance));
}