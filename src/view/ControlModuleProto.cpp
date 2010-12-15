/*
* ControlModuleProto.cpp
*
*  Created on: Sep 29, 2008
*      Author: bey0nd
*/

#include <sstream>
#include <iostream>
using std::cout;
using std::endl;
#include "ControlModuleProto.h"
#include "EventDelegates.h"
#include "LifeCycleDelegates.h"
#include "CommandDelegates.h"
#include "CommandController.h"
#include "command/CommandList.h"

namespace ZGame
{
    using namespace std;
    using namespace Ogre;

    ControlModuleProto::ControlModuleProto() :
    _transVector(0.0f, 0.0f, 0.0f), _dTrans(10.0f), _transFactor(1.0), _rotYaw(0.0f), _rotPitch(0.0f), _rotFactor(50 * 3.145), _mx_rel(0), _my_rel(0), 
        _cam(0), _lookAtNode(0), _cameraNode(0), _camLocalZOffset(40.0f), _zoomFactor(0.1f)
    {
        //Ogre::String CAMERAATTACH("camera_attach");
        ZGame::CommandController* cmdCtrl = ZGame::CommandController::getSingletonPtr();
        COMMAND::ConsoleCommand cmd;
        //cmd.bind(this,&ZGame::ControlModuleProto::attachNode);
        cmd.bind(this, &ZGame::ControlModuleProto::attachNode);
        cmdCtrl->addCommand(CommandList::CAMERA_ATTACH, cmd.GetMemento());
    }

    ControlModuleProto::ControlModuleProto(float startTrans) :
    _transVector(0.0f, 0.0f, 0.0f), _dTrans(startTrans), _transFactor(10.0), _rotYaw(0.0f), _rotPitch(0.0f), _rotFactor(0.07f), 
        _cam(0), _lookAtNode(0), _cameraNode(0), _camLocalZOffset(40.0f), _zoomFactor(0.1f)
    {
        //Ogre::String CAMERAATTACH("camera_attach");
        ZGame::CommandController* cmdCtrl = ZGame::CommandController::getSingletonPtr();
        COMMAND::ConsoleCommand cmd;
        //cmd.bind(this,&ZGame::ControlModuleProto::attachNode);
        cmd.bind(this, &ZGame::ControlModuleProto::attachNode);
        cmdCtrl->addCommand(CommandList::CAMERA_ATTACH, cmd.GetMemento());
    }

    ControlModuleProto::~ControlModuleProto()
    {
        // TODO Auto-generated destructor stub
    }

    bool
        ControlModuleProto::onInit(ZGame::ZInitPacket packet)
    {
        _cam = packet.initialCamera;
        using COMMAND::CommandList;
        _cameraNode = packet.sceneManager->createSceneNode("CAMERA_NODE");
        _cameraNode->attachObject(_cam);
        return true;
    }

    bool
        ControlModuleProto::onKeyDown(const OIS::KeyEvent &evt)
    {
        if (evt.key == OIS::KC_W)
        {
            _transVector.z -= _dTrans;
        }
        else if (evt.key == OIS::KC_S)
        {
            _transVector.z += _dTrans;
        }
        else if (evt.key == OIS::KC_Q)
        {
            updateTransFactor(-0.1f);
        }
        else if (evt.key == OIS::KC_E)
        {
            updateTransFactor(0.1f);
        }
        else if (evt.key == OIS::KC_A)
        {
            _transVector.x -= _dTrans;
        }
        else if (evt.key == OIS::KC_D)
        {
            _transVector.x += _dTrans;
        }
        else if (evt.key == OIS::KC_UP)
        {
            _transVector.y += _dTrans;
        }
        else if (evt.key == OIS::KC_DOWN)
        {
            _transVector.y -= _dTrans;
        }
        else if (evt.key == OIS::KC_U)
        {
            _camLocalZOffset += 0.1f;
        }
        return true;
    }

    bool
        ControlModuleProto::onKeyUp(const OIS::KeyEvent &evt)
    {
        //turn off control signal
        if (evt.key == OIS::KC_W)
        {
            _transVector.z = 0;
            cout << "position: " << _cam->getPosition() << endl;
        }
        else if (evt.key == OIS::KC_S)
        {
            _transVector.z = 0;
            cout << "position: " << _cam->getPosition() << endl;
        }
        else if (evt.key == OIS::KC_A)
        {
            _transVector.x = 0;
        }
        else if (evt.key == OIS::KC_D)
        {
            _transVector.x = 0;
        }
        else if (evt.key == OIS::KC_UP)
        {
            _transVector.y = 0;
        }
        else if (evt.key == OIS::KC_DOWN)
        {
            _transVector.y = 0;
        }
        else if (evt.key == OIS::KC_L)
        {
            _zoomFactor += 0.5f;
            _camLocalZOffset = Math::Exp(_zoomFactor);
        }
        else if (evt.key == OIS::KC_I)
        {
            _zoomFactor -= 0.5f;
            _camLocalZOffset = Math::Exp(_zoomFactor);
        }
        return true;
    }

    void
        ControlModuleProto::_doMouseMove(Ogre::Real dt)
    {
        if (_lookAtNode)
        {
            //_cameraNode->yaw(Radian(-Ogre::Math::DegreesToRadians(_rotFactor*evt.state.X.rel)),Ogre::Node::TS_WORLD);
            _cameraNode->yaw(Radian(-Ogre::Math::DegreesToRadians(_rotFactor * _mx_rel)));
            _cameraNode->pitch(Radian(-Ogre::Math::DegreesToRadians(_rotFactor * _my_rel)));
        }
        else
        {
            //locked in at 60fps
            _cam->yaw(Radian(-Ogre::Math::DegreesToRadians(0.016 * 25.0 *_mx_rel)));
            _cam->pitch(Radian(-Ogre::Math::DegreesToRadians(0.016 * 25.0*_my_rel)));
        }
        _mx_rel = 0;
        _my_rel = 0;
    }

    bool
        ControlModuleProto::onMouseMove(const OIS::MouseEvent &evt)
    {
        _mx_rel = evt.state.X.rel;// - _mx_rel;
        _my_rel = evt.state.Y.rel;// - _my_rel;

        //_mx_rel = _mx + evt.state.X.abs;// - _mx_rel;
        //_my_rel = _my + evt.state.Y.abs;// - _my_rel;
        //_mx = evt.state.X.abs;
        //_my = evt.state.Y.abs;
        return true;
    }

    bool
        ControlModuleProto::onMouseUp(const OIS::MouseEvent &evt, const OIS::MouseButtonID id)
    {

        return true;
    }

    bool
        ControlModuleProto::onMouseDown(const OIS::MouseEvent &evt, const OIS::MouseButtonID id)
    {
        return true;
    }

    bool
        ControlModuleProto::onUpdate(const Ogre::FrameEvent &evt)
    {
        //if(_theNode)
        //_theNode->moveRelative(_transVector);
        //_theNode->translate(_theNode->getPosition()+_transVector);
        //_cam->moveRelative(_transVector);

        if (_lookAtNode)
        {

            _lookAtNode->translate(_lookAtNode->getOrientation() * _transVector);
            //then rotate about lookatNode
            _cameraNode->setPosition(Ogre::Vector3::ZERO);
            _cameraNode->translate(_cameraNode->getLocalAxes(), Ogre::Vector3(0.0, 0.0, _camLocalZOffset));
            _doMouseMove(evt.timeSinceLastEvent);

        }
        else
        {
            if(evt.timeSinceLastFrame < 0.05)
            {
                _cam->moveRelative(_transVector * evt.timeSinceLastFrame);
                _doMouseMove(evt.timeSinceLastFrame);
            }
        }
        return true;
    }

    void
        ControlModuleProto::updateTransFactor(Ogre::Real factor)
    {
        _transFactor += factor;
        _dTrans = Math::Exp(_transFactor);
    }

    void
        ControlModuleProto::cleanUpNodes()
    {

    }

    bool
        ControlModuleProto::attachNode(const Ogre::StringVector &params)
    {
    
        return true;
    }

}
