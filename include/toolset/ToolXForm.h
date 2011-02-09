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
    namespace Toolset
    {
        /**
        * This class is the super-class for Tool Transforms. 
        *\note Still testing this. Not sure if we have all the bases covered for doing it
        *this way. Basically the idea is we will use Bullet Physics to do geometric things
        *so we also need to transform Bullet Physics object when we transform this. I have
        *some experience with doing these kinds of programs when there is a Geomtry library
        *that I use; this is the first time I'm trying to do this without a dedicated geometry
        *lib.
        *
        **/
        class ToolXForm
        {
        public:
            virtual ~ToolXForm(){}

            /** 
            * This method will apply a scale transformation to a given node based on the cursor 
            *position. This scales always based on the lower-left corner of the AABB of 
            *the node and the cursor position. Thus, the node may be translated to conform
            *to this constraint (e.g. if you position the cursor to the left of the box, the
            *node will be translated to this position, then scaled based on the scaling factor
            *from this new left-corner, to the old left-corner. One can imagine the operation 
            *carried out by this function as a typical selection box operation.
            **/
            virtual ToolXForm&
                scaleApply(Ogre::SceneNode* node, const Ogre::Vector3 &anchorPos, const Ogre::Vector3 &cursorPos) = 0;
            virtual ToolXForm&
                translateApply(Ogre::SceneNode* node, const Ogre::Vector3 &anchorPos, const Ogre::Vector3 &cursorPos) = 0;
            virtual ToolXForm&
                setLowerLeft(Ogre::SceneNode* node, const Ogre::Vector3 &anchorPos, const Ogre::Vector3 &cursorPos) = 0;
            virtual ToolXForm&
                setUpperRight(Ogre::SceneNode* node, const Ogre::Vector3 &anchorPos, const Ogre::Vector3 &cursorPos) = 0;
            

        protected:
            ToolXForm(){}
        private:
        };
    }
}