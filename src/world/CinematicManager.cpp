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

#include "world/CinematicManager.h"

using namespace ZGame::World;

const CAMERA_ID CinematicManager::_camCurId = 0;

CinematicManager::CinematicManager(Ogre::SceneManager* scnMgr) : _scnMgr(scnMgr),
    ORTHO_CAM_NAME("ORTHO_CAM"), PERSP_CAM_NAME("PERSP_CAM"), _rootCam(0)
{
}

CinematicManager::~CinematicManager()
{
}

CAMERA_ID
    CinematicManager::createPerspectiveCamera(Ogre::Real windowW, Ogre::Real windowH, const Ogre::Vector3 &initialPos,
    const Ogre::Quaternion &initialOrient)
{
    Ogre::Camera* cam = _scnMgr->createCamera(PERSP_CAM_NAME + Ogre::StringConverter::toString(_camCurId));
    cam->setNearClipDistance(0.5f);
    cam->setFarClipDistance(800.0f);
    cam->setPosition(initialPos);
    cam->setOrientation(initialOrient);
    cam->setAspectRatio(windowW / windowH);
    _camsVec.push_back(cam);
    return _camsVec.size();
}

CAMERA_ID
    CinematicManager::createOrthoCamera(Ogre::Real windowW, Ogre::Real windowH, const Ogre::Vector3 &initialPos,
    const Ogre::Quaternion &initialOrient)
{
    Ogre::Camera* cam = _scnMgr->createCamera(ORTHO_CAM_NAME + Ogre::StringConverter::toString(_camCurId));
    cam->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
    cam->setOrthoWindow(100, 50);
    cam->setNearClipDistance(1);
    //cam->setFarClipDistance(100.0f);
    cam->setFOVy(Ogre::Radian(Ogre::Math::PI / 2.0f));
    cam->setPosition(initialPos);
    cam->setOrientation(initialOrient);
    _camsVec.push_back(cam);
    return _camsVec.size();
}

void
    CinematicManager::setRootCam(CAMERA_ID id)
{
    if(id >= _camsVec.size())
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Invalid camera id", "CinematicManager::setRootCam");
    _rootCam = _camsVec[id];
}