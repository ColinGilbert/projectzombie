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
#include "world/Control.h"

using namespace ZGame::World;

const CAMERA_ID CinematicManager::_camCurId = 0;

CinematicManager::CinematicManager(Ogre::SceneManager* scnMgr) : _scnMgr(scnMgr),
    ORTHO_CAM_NAME("ORTHO_CAM"), PERSP_CAM_NAME("PERSP_CAM"), _rootCam(0)
{
}

CinematicManager::~CinematicManager()
{
    for(size_t i = 0; i < _camsVec.size(); ++i)
    {
        delete _camsVec[i];
    }
}
//We're not sure whether return a reference to a vector is such a great idea. 
void
    CinematicManager::createPerspectiveCamera(Ogre::Real windowW, Ogre::Real windowH, const Ogre::Vector3 &initialPos,
    const Ogre::Quaternion &initialOrient, Control* control)
{
    Ogre::Camera* cam = _scnMgr->createCamera(PERSP_CAM_NAME + Ogre::StringConverter::toString(_camCurId));
    cam->setNearClipDistance(5.0f);
    cam->setFarClipDistance(800.0f);
    cam->setPosition(0.0f, 0.0f, 0.0f);
    cam->setAspectRatio(windowW / windowH);
    Ogre::SceneNode* defaultNode = _scnMgr->createSceneNode();
    defaultNode->setPosition(initialPos); //It's in local space.
    defaultNode->setOrientation(initialOrient);
    defaultNode->attachObject(cam);
    _camsVec.push_back(new ZCameraInfo(_camsVec.size(), "PERSPECTIVE", cam->getName(), control, cam));
}

void
    CinematicManager::createOrthoCamera(Ogre::Real windowW, Ogre::Real windowH, const Ogre::Vector3 &initialPos,
    const Ogre::Quaternion &initialOrient, Control* control)
{
    Ogre::Camera* cam = _scnMgr->createCamera(ORTHO_CAM_NAME + Ogre::StringConverter::toString(_camCurId));
    cam->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
    cam->setOrthoWindow(100, 50);
    cam->setNearClipDistance(1);
    cam->setPosition(0.0f, 0.0f,  0.0f);
    //cam->setFarClipDistance(100.0f);
    cam->setFOVy(Ogre::Radian(Ogre::Math::PI / 2.0f));
    Ogre::SceneNode* defaultNode = _scnMgr->createSceneNode();
    defaultNode->setPosition(initialPos);
    defaultNode->setOrientation(initialOrient);
    defaultNode->attachObject(cam);
    _camsVec.push_back(new ZCameraInfo(_camsVec.size(), "ORTHOGRAPHIC", cam->getName(), control, cam));
}

void
    CinematicManager::setRootCam(CAMERA_ID id)
{
    if(id >= _camsVec.size())
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Invalid camera id", "CinematicManager::setRootCam");
    _rootCam = _camsVec[id];
}