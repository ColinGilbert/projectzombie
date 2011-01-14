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
                Ogre::String names, Control* ctrl, Ogre::Camera* cam,
                Ogre::SceneNode* node)
                : id(theid), type(types), name(names), _control(ctrl), _camera(cam),
                _node(node), _defaultNode(node)
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

            Ogre::SceneNode*
                getNode()
            {
                return _node;
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

            void
                setNode(Ogre::SceneNode* node)
            {
                _node = node;
            }

            /** This will reset to default node. Every Camera requires a default node.**/
            void resetToDefaultNode()
            {
                _node = _defaultNode;
            }

        private:
            Ogre::String type;
            Ogre::String name;
            CAMERA_ID id;
            Ogre::SceneNode* _defaultNode; //Every camera has to have a node. 
            Ogre::SceneNode* _node;
            Ogre::Camera* _camera;
            Control* _control;


        };
    }
}