#include "ZWorkspace.h"
#include "ZWorkspaceController.h"
#include "world/WorldController.h"
#include "EngineView.h"
#include <iostream>
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
    ZWorkspaceController::onMouseUp(const OIS::MouseEvent &evt, const OIS::MouseButtonID id)
{
    Ogre::Real height = EngineView::getSingleton().getCurrentCamera()->getViewport()->getActualHeight();
    Ogre::Real width  = EngineView::getSingleton().getCurrentCamera()->getViewport()->getActualWidth();
    //Depending on mode.
    if(id == OIS::MouseButtonID::MB_Left)
    {
        _workspace->getWorldController()->addBlock(evt.state.X.abs / width, 
            evt.state.Y.abs / height);
    }
    else if(id == OIS::MouseButtonID::MB_Right)
    {
        _workspace->getWorldController()->removeBlock(evt.state.X.abs / width,
            evt.state.Y.abs / height);
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
