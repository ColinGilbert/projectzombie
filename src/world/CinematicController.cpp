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

using namespace ZGame::World;

CinematicController::CinematicController(std::auto_ptr<CinematicManager> cineMgr, Ogre::RenderWindow* rendWin) 
    : _cineMgr(cineMgr), _rendWin(rendWin), _vp(0)
{
}

CinematicController::~CinematicController()
{
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
                pair.second.first, pair.second.second);
        }
        else if(ortho.compare(pair.first) == 0)
        {
            _cineMgr->createOrthoCamera(_rendWin->getWidth(), _rendWin->getHeight(), 
                pair.second.first, pair.second.second);
        }
        else
            OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Invalid camera type: must be PERSPECTIVE or ORTHOGRAPHIC",
            "CinematicController::loadCameras");
    }
    _cineMgr->setRootCam(rootCamIdx);
    _vp = _rendWin->addViewport(_cineMgr->getRootCam());
}


