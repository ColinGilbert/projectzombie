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
using namespace ZGame;
using namespace ZGame::World;

CinematicController::CinematicController(std::auto_ptr<CinematicManager> cineMgr, Ogre::RenderWindow* rendWin) 
    : _cineMgr(cineMgr), _rendWin(rendWin), _vp(0), _perspControl(new PerspectiveControl()), _disable(false)
{
    //test code. These nodes should be passed in.
    _centerNode = _cineMgr->getSceneManager()->getRootSceneNode()->createChildSceneNode();
}

CinematicController::~CinematicController()
{
    _cineMgr->getSceneManager()->getRootSceneNode()->removeChild(_centerNode);
}


void
    CinematicController::onCameraChange(CAMERA_ID camId)
{
    std::cout << "Camera id in onCameraChange: " << camId << std::endl;
    _cineMgr->setRootCam(camId);
    _vp->setCamera(_cineMgr->getRootCam());
    _currentOperation.setInput(_cineMgr->getRootCamInfo()->getCamera(), _cineMgr->getRootCamInfo()->getControl(), 
        _centerNode, _centerNode); //There is only one control right now for testing.
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
        //Camera positions and rotation are in camera local space.
        if(pers.compare(pair.first) == 0)
        {
                _cineMgr->createPerspectiveCamera(_rendWin->getWidth(), _rendWin->getHeight(), 
                    Ogre::Vector3(), Ogre::Quaternion(), _perspControl.get());
        }
        else if(ortho.compare(pair.first) == 0)
        {
                _cineMgr->createOrthoCamera(_rendWin->getWidth(), _rendWin->getHeight(), 
                    Ogre::Vector3(), Ogre::Quaternion(), _perspControl.get());
        }
        else
            OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Invalid camera type: must be PERSPECTIVE or ORTHOGRAPHIC",
            "CinematicController::loadCameras");
        if(i == rootCamIdx)
        { 
            _centerNode->setPosition(pair.second.first);
            _centerNode->rotate(pair.second.second);
        }
    }
    _cineMgr->setRootCam(rootCamIdx);
    _vp = _rendWin->addViewport(_cineMgr->getRootCam());
    onCameraChange(rootCamIdx);
    
}

bool
    CinematicController::onMouseMove(const OIS::MouseEvent& e)
{
    if(_disable)
    {
        return true;
    }
    _currentOperation.onMouseMove(e);
    return true;
}

bool
    CinematicController::onMouseDown(const OIS::MouseEvent& e, OIS::MouseButtonID id)
{
    if(_disable)
    {
        return true;
    }
    _currentOperation.onMouseDown(e, id);
    return true;
}

bool
    CinematicController::onMouseUp(const OIS::MouseEvent& e, OIS::MouseButtonID id)
{
    if(_disable)
        return true;
    _currentOperation.onMouseUp(e, id);
    return true;
}

bool
    CinematicController::onKeyDown(const OIS::KeyEvent& e)
{
    if(_disable)
        return true;
    _currentOperation.onKeyDown(e);
    return true;
}

bool
    CinematicController::onKeyUp(const OIS::KeyEvent& e)
{
    if(_disable)
        return true;
    _currentOperation.onKeyUp(e);
    return true;
}

void
    CinematicController::onSetCursor3dPosition(Toolset::ToolsetController* toolsetCtrl)
{
    //We do it this way is because typically we attach the camera to some scene node, and whatever is responsible for
    //that scene node will change it there. 
    _currentOperation.getAttachNode()->setPosition(toolsetCtrl->getCursor3dPosition());
}

/**
*Do we really have to have so many abstractions here? Current operatioion::upadte then in that control::update.
*Idea for this is to make it more generalized.
*/
bool
    CinematicController::onUpdate(const Ogre::FrameEvent &evt)
{
    if(_disable)
        _disable = false;
    _currentOperation.onUpdate(evt);
    return true;
}

CAM_INFO_CITERS
    CinematicController::getCameraInfosIterators()
{
    return _cineMgr->getIterators();
}


void
    CinematicOperation::setInput(Ogre::Camera* cam, Control* control, Ogre::SceneNode* attachNode,
    Ogre::SceneNode* lookAtNode)
{
    if(!attachNode)
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Invalid null pointer: attachNode", "CinematicOperation::setInput");
    if(!lookAtNode)
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Invalid null pointer: lookAtNode", "CinematicOperation::setInput");
    Ogre::SceneNode* camNode;
    camNode = static_cast<Ogre::SceneNode*>(cam->getParentNode());
    if(!camNode)
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Camera is missing a camera node", "CinematicOperation::setInput");
    _control = control;
    Ogre::Node* parentNode = camNode->getParent();
    if(parentNode)
        parentNode->removeChild(camNode);
    _cam = cam;
    _rootNode = attachNode;
    _rootNode->addChild(camNode);
    _rootNode->setInheritOrientation(false);
    _lookAtNode = lookAtNode;
    _cam->setFixedYawAxis(true);
}

EditorOperation::EditorOperation() : _curMode(NONE)
{
}

EditorOperation::~EditorOperation()
{
}

void
    EditorOperation::onMouseDown(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
    if(id == OIS::MB_Left && _curMode == NONE) //Enter operation mode
    {
        _curMode = CAM_OP_PITCH_YAW;
    }
    else if(id == OIS::MB_Right && _curMode == NONE)
    {
        _curMode = CAM_OP_DOLLY;
    }
}

void
    EditorOperation::onMouseUp(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
    if(id == OIS::MB_Left)
    {
        _curMode = NONE;
    }
    else if(id == OIS::MB_Right)
    {
        _curMode = NONE;
    }
}

void
    EditorOperation::onMouseMove(const OIS::MouseEvent &e)
{
    //Depend on modifier key and mode. Note: We have yet to implement modifier key events.
    if(_curMode == CAM_OP_PITCH_YAW)
    {
        //modifier mode....   
        _control->pitch(-e.state.Y.rel, static_cast<Ogre::SceneNode*>(_cam->getParentSceneNode()), _lookAtNode);
        _control->yaw(-e.state.X.rel, static_cast<Ogre::SceneNode*>(_cam->getParentSceneNode()), _lookAtNode);
    }
    else if(_curMode == CAM_OP_DOLLY)
    {
        _control->dolly(e.state.X.rel, static_cast<Ogre::SceneNode*>(_cam->getParentNode()), _lookAtNode);
    }
}

void
    EditorOperation::resetAttachNode(Ogre::SceneNode* attachNode)
{
    _rootNode = attachNode;
    _control->resetDistance();
}

void
    EditorOperation::onKeyDown(const OIS::KeyEvent &e)
{
    
}

void
    EditorOperation::onKeyUp(const OIS::KeyEvent &e)
{
}

void
    EditorOperation::onUpdate(const Ogre::FrameEvent& evt)
{
    _control->update(evt.timeSinceLastFrame, _cam, _cam->getParentSceneNode(), _lookAtNode);
}

void
    CinematicController::onDisableOneFrame()
{
    _disable = true;
}