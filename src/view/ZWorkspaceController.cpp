#include "ZWorkspace.h"
#include "ZWorkspaceController.h"
using namespace ZGame;

ZWorkspaceController::ZWorkspaceController(ZWorkspace* workspace) : 
_workspace(workspace)
{
}

ZWorkspaceController::~ZWorkspaceController()
{
}

bool
    ZWorkspaceController::onMouseUp(const OIS::MouseEvent &evt, const OIS::MouseButtonID id)
{
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
