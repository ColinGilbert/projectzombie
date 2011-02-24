#include "ZWorkspace.h"
#include "ZWorkspaceController.h"
#include "world/WorldController.h"
#include "ZInitPacket.h"
#include "geometry/GeometryManager.h"
#include "world/CinematicController.h"
#include "world/CinematicManager.h"
#include "world/GameController.h"
#include "toolset/ToolsetController.h"
using std::cout;
using std::endl;
using namespace ZGame;

ZWorkspaceController::ZWorkspaceController() : _modifierState(0)
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

void
    ZWorkspaceController::onModifierStateEvent(const unsigned int modifierState)
{
    _modifierState = modifierState;
}

/**
** \note This method is NOT FINISHED!
**/
bool
    ZWorkspaceController::onMouseUp(const OIS::MouseEvent &evt, const OIS::MouseButtonID id)
{
    if(id == OIS::MB_Left)
    {
        if(_workspace->getToolsetController()->isCursorMode())
        {
            _workspace->getToolsetController()->onSetCursor3dPosition(_workspace->getWorldController());

            if(_workspace->getGameController()->requireWorkspace())
                _workspace->getGameController()->onWorkspace(_workspace);

        }
    }
    return true;
}

bool
    ZWorkspaceController::onMouseDown(const OIS::MouseEvent &evt, 
    const OIS::MouseButtonID id)
{
    if(id == OIS::MB_Left)
    {
    }
    return true;
}

bool
    ZWorkspaceController::onMouseMove(const OIS::MouseEvent &evt)
{
    if(_workspace->getToolsetController()->isCursorMode())
    {
        //Get voxel position.
        //Just pass in the 3d position of where the user clicked. This is predefined to be a certain distance into the screen.
        Ogre::Camera* cam = _workspace->getCinematicController()->getCinematicManager()->getRootCam();
        Ogre::Real x, y;
        x = evt.state.X.abs / _windowWidth;
        y = evt.state.Y.abs / _windowHeight;

        Ogre::Ray rayTo;
        Ogre::Vector3 position;
        Ogre::Real searchDistance = 64.0f;
        rayTo  = cam->getCameraToViewportRay(x, y);
        
        //Shift modifies cursor to be constraint to a plane.
        if(_modifierState & OIS::Keyboard::Shift)
        {
            Ogre::Plane constraintPlane;
            bool hit = _workspace->getToolsetController()->getConstraintPlane(rayTo, constraintPlane);
            if(hit)
                _workspace->getWorldController()->getCursor3dPosition(rayTo, position, constraintPlane);
            else
                _workspace->getWorldController()->getCursor3dPosition(rayTo, position, searchDistance);
        }
        else
        {
            _workspace->getWorldController()->getCursor3dPosition(rayTo, position, searchDistance);
        }
        
        _workspace->getToolsetController()->onCursorPosition3d(position);
        _workspace->getCinematicController()->onDisableOneFrame();
    }
    return true;
}

bool
    ZWorkspaceController::onKeyDown(const OIS::KeyEvent &evt)
{
    if(_workspace->getToolsetController()->isCursorMode())
    {
         _workspace->getCinematicController()->onDisableOneFrame();
    }
    return true;
}

bool
    ZWorkspaceController::onKeyUp(const OIS::KeyEvent &evt)
{
    if(evt.key == OIS::KC_S)
        _workspace->getToolsetController()->setToolType(Toolset::ToolsetController::SELECT);
    if(evt.key == OIS::KC_C)
        _workspace->getToolsetController()->setToolType(Toolset::ToolsetController::CURSOR);
    return true;
}

void
    ZWorkspaceController::onEnterSelectionMode()
{
    _workspace->getToolsetController()->setToolsetMode(Toolset::ToolsetController::FILL);
}