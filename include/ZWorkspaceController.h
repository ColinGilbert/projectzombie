#pragma once
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
    private:
        ZWorkspace* _workspace;
        /* TEMP VARIABLES. The way we are doing input out is not FINISHED*/
        Ogre::Real _windowHeight;
        Ogre::Real _windowWidth;
    };
}