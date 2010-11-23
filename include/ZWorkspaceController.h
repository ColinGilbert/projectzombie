#pragma once
#include "ZPrerequisites.h"
#include <OIS/OIS.h>

namespace ZGame
{
    class ZWorkspace;
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
            setZWorkspaceController(ZWorkspace* workspace)
        {
            _workspace = workspace;
        }

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
    };
}