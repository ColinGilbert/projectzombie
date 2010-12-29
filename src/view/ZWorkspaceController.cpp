#include "ZWorkspace.h"
#include "ZWorkspaceController.h"
#include "world/WorldController.h"
#include "ZInitPacket.h"
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
    //Depending on mode.
    if(id == OIS::MouseButtonID::MB_Left)
    {
        _workspace->getWorldController()->addBlock(evt.state.X.abs / _windowWidth, 
            evt.state.Y.abs / _windowHeight);
    }
    else if(id == OIS::MouseButtonID::MB_Right)
    {
        _workspace->getWorldController()->removeBlock(evt.state.X.abs / _windowWidth,
            evt.state.Y.abs / _windowHeight);
    }
    return true;
}

bool
    ZWorkspaceController::onMouseDown(const OIS::MouseEvent &evt, 
    const OIS::MouseButtonID id)
{
    
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
