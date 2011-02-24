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
    
    /**
    * This class defines a controller object corresponding to ZWorkspace. The main purpose of this
    *class is a controller abstraction for ZWorkspace, in that it delegate certain control events
    *to objects that composes ZWorkspace.
    *
    *
    **/
    class ZWorkspaceController
    {
    public:
        ZWorkspaceController();
        ~ZWorkspaceController();

        /**
        *\brief This method sets the workspace to corresponding to this controller.
        *
        * \note please make sure workspace is attached before doing anything with this
        *controller. No checking is done.
        */
        void
            setZWorkspace(ZWorkspace *workspace)
        {
            _workspace = workspace;
        }

        ZWorkspace*
            getZWorkspace()
        {
            return _workspace;
        }


        bool
            onInit(ZInitPacket *packet);
        bool
            onDestroy(){return true;}
        bool 
            onMouseUp(const OIS::MouseEvent &evt, const OIS::MouseButtonID id);
        bool
            onMouseDown(const OIS::MouseEvent &evt, const OIS::MouseButtonID id);
        bool
            onMouseMove(const OIS::MouseEvent &evt);
        bool
            onKeyDown(const OIS::KeyEvent &evt);
        bool 
            onKeyUp(const OIS::KeyEvent &evt);

        void
            onModifierStateEvent(const unsigned int modifierState);
        /** 
        * This is a hack method to allow anyone to use the selection tool. We should refactor the way
        *we're doing this. Maybe implement a global raise event system WRT to the screens and controllers.
        **/
        void
            onEnterSelectionMode();


    private:
        ZWorkspace* _workspace;
        /* TEMP VARIABLES. The way we are doing input out is not FINISHED*/
        Ogre::Real _windowHeight;
        Ogre::Real _windowWidth;

        unsigned int _modifierState;



    };
}