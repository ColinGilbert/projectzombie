#include "ZWorkspace.h"
#include "ZWorkspaceController.h"
#include "world/WorldController.h"
#include "ZInitPacket.h"
#include "geometry/GeometryManager.h"
#include "world/CinematicController.h"
#include "world/CinematicManager.h"
#include "toolset/ToolsetController.h"
using std::cout;
using std::endl;
using namespace ZGame;

ZWorkspaceController::ZWorkspaceController() 
{
}

ZWorkspaceController::~ZWorkspaceController()
{
}

bool
    ZWorkspaceController::onInit(ZInitPacket *packet)
{
    //Note: We are not finished with implementing this so right now it looks actualy.
    //By thtat I mean we should minimize dependencies between this and anything view related.
    //i.e. we should need to get window height and width here.
    _windowHeight = packet->initialCamera->getViewport()->getActualHeight();
    _windowWidth = packet->initialCamera->getViewport()->getActualWidth();

    return true;
}

/**
** \note This method is NOT FINISHED!
**/
bool
    ZWorkspaceController::onMouseUp(const OIS::MouseEvent &evt, const OIS::MouseButtonID id)
{
    return true;
}

bool
    ZWorkspaceController::onMouseDown(const OIS::MouseEvent &evt, 
    const OIS::MouseButtonID id)
{
    //Depending on mode.
    if(id == OIS::MouseButtonID::MB_Left)
    {
        //Just pass in the 3d position of where the user clicked. This is predefined to be a certain distance into the screen.
        Ogre::Camera* cam = _workspace->getCinematicController()->getCinematicManager()->getRootCam();
        Ogre::Real x, y;
        x = evt.state.X.abs / _windowWidth;
        y = evt.state.Y.abs / _windowHeight;

        Ogre::Ray rayTo;
        Ogre::Real distanceForward = 4.0f; //4 units into screen is where cursor will be.
        rayTo  = cam->getCameraToViewportRay(x, y);

        /*
        *This business is stupid. We only doing this now for prototyping. 
        *
        */
        if(_workspace->getToolsetController()->onCursorPosition3d(rayTo.getPoint(distanceForward)))
        {
            _workspace->getCinematicController()->onDisableOneFrame();
        }


    }
    else if(id == OIS::MouseButtonID::MB_Right)
    {     
    }
    return true;
}

bool
    ZWorkspaceController::onMouseMove(const OIS::MouseEvent &evt)
{
    return true;
}

bool
    ZWorkspaceController::onKeyDown(const OIS::KeyEvent &evt)
{
    return true;
}

bool
    ZWorkspaceController::onKeyUp(const OIS::KeyEvent &evt)
{
    return true;
}
