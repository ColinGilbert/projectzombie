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
    _vp->setCamera(_cineMgr->getCamera(camId));
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
        CAMERA_ID camId;

        if(pers.compare(pair.first) == 0)
        {
            camId = _cineMgr->createPerspectiveCamera(_rendWin->getWidth(), _rendWin->getHeight(), 
                pair.second.first, pair.second.second);
            _camInfoVec.push_back(ZCameraInfo(camId, pair.first, _cineMgr->getCamera(camId)->getName(),
                _perspControl.get()));
        }
        else if(ortho.compare(pair.first) == 0)
        {
            camId = _cineMgr->createOrthoCamera(_rendWin->getWidth(), _rendWin->getHeight(), 
                pair.second.first, pair.second.second);
            _camInfoVec.push_back(ZCameraInfo(camId, pair.first, _cineMgr->getCamera(camId)->getName(),
                _perspControl.get()));
        }
        else
            OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Invalid camera type: must be PERSPECTIVE or ORTHOGRAPHIC",
            "CinematicController::loadCameras");
    }
    _cineMgr->setRootCam(rootCamIdx);
    _vp = _rendWin->addViewport(_cineMgr->getRootCam());
}

bool
    CinematicController::onMouseMove(const OIS::MouseEvent& e)
{
    return true;
}

bool
    CinematicController::onMouseDown(const OIS::MouseEvent& e, OIS::MouseButtonID id)
{
    return true;
}

bool
    CinematicController::onMouseUp(const OIS::MouseEvent& e, OIS::MouseButtonID id)
{
    return true;
}

bool
    CinematicController::onKeyDown(const OIS::KeyEvent& e)
{
    return true;
}

bool
    CinematicController::onKeyUp(const OIS::KeyEvent& e)
{
    return true;
}