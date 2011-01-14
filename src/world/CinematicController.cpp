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

#include "world/CinematicController.h"
#include "world/CinematicManager.h"
#include "world/PerspectiveControl.h"

using namespace ZGame::World;

CinematicController::CinematicController(std::auto_ptr<CinematicManager> cineMgr, Ogre::RenderWindow* rendWin) 
    : _cineMgr(cineMgr), _rendWin(rendWin), _vp(0), _perspControl(new PerspectiveControl())
{
}

CinematicController::~CinematicController()
{
}

void
    CinematicController::onCameraChange(CAMERA_ID camId)
{
    std::cout << "Camera id in onCameraChange: " << camId << std::endl;
    _cineMgr->setRootCam(camId);
    _vp->setCamera(_cineMgr->getRootCam());
    _currentOperation.setInput(_cineMgr->getRootCamInfo()->getCamera(), _cineMgr->getRootCamInfo()->getControl(), 
        _cineMgr->getRootCamInfo()->getNode()); //There is only one control right now for testing.
}

void
    CinematicController::loadCameras(std::vector<CAM_PAIR> camStates, size_t rootCamIdx)
{
    if(rootCamIdx >= camStates.size())
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Invalid root cam index",
        "CinematicController::loadCameras");
    Ogre::String pers("PERSPECTIVE");
    Ogre::String ortho("ORTHOGRAPHIC");
    for(size_t i=0; i < camStates.size(); ++i)
    {
        CAM_PAIR pair = camStates[i];
        if(pers.compare(pair.first) == 0)
        {
                _cineMgr->createPerspectiveCamera(_rendWin->getWidth(), _rendWin->getHeight(), 
                pair.second.first, pair.second.second, _perspControl.get());
        }
        else if(ortho.compare(pair.first) == 0)
        {
                _cineMgr->createOrthoCamera(_rendWin->getWidth(), _rendWin->getHeight(), 
                pair.second.first, pair.second.second, _perspControl.get());
        }
        else
            OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Invalid camera type: must be PERSPECTIVE or ORTHOGRAPHIC",
            "CinematicController::loadCameras");
    }
    _cineMgr->setRootCam(rootCamIdx);
    _vp = _rendWin->addViewport(_cineMgr->getRootCam());
    onCameraChange(rootCamIdx);
    
}

bool
    CinematicController::onMouseMove(const OIS::MouseEvent& e)
{
    _currentOperation.onMouseMove(e);
    return true;
}

bool
    CinematicController::onMouseDown(const OIS::MouseEvent& e, OIS::MouseButtonID id)
{
    _currentOperation.onMouseDown(e, id);
    return true;
}

bool
    CinematicController::onMouseUp(const OIS::MouseEvent& e, OIS::MouseButtonID id)
{
    _currentOperation.onMouseUp(e, id);
    return true;
}

bool
    CinematicController::onKeyDown(const OIS::KeyEvent& e)
{
    _currentOperation.onKeyDown(e);
    return true;
}

bool
    CinematicController::onKeyUp(const OIS::KeyEvent& e)
{
    _currentOperation.onKeyUp(e);
    return true;
}

CAM_INFO_CITERS
    CinematicController::getCameraInfosIterators()
{
    return _cineMgr->getIterators();
}


EditorOperation::EditorOperation()
{
}

EditorOperation::~EditorOperation()
{
}

void
    EditorOperation::onMouseDown(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
    if(id == OIS::MB_Left) //Enter operation mode
    {
        _curMode = CAM_OP;
    }
}

void
    EditorOperation::onMouseUp(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
    if(id == OIS::MB_Left)
    {
        _curMode = NONE;
    }
}

void
    EditorOperation::onMouseMove(const OIS::MouseEvent &e)
{
    //Depend on modifier key and mode. Note: We have yet to implement modifier key events.
    if(CAM_OP)
    {
        //modifier mode....
        _control->yaw(e.state.X.rel, _cam, _node);
        _control->pitch(e.state.Y.rel, _cam, _node);
    }
}

void
    EditorOperation::onKeyDown(const OIS::KeyEvent &e)
{
}

void
    EditorOperation::onKeyUp(const OIS::KeyEvent &e)
{
}