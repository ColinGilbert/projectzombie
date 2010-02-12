/*
 * ControlModuleProto.cpp
 *
 *  Created on: Sep 29, 2008
 *      Author: bey0nd
 */

#include <sstream>

#include "ControlModuleProto.h"
#include "EventDelegates.h"
#include "LifeCycleDelegates.h"
#include "EngineView.h"
#include "CommandDelegates.h"
#include "CommandController.h"


namespace ZGame
{
  using namespace std;
  using namespace Ogre;
  
  ControlModuleProto::ControlModuleProto() :_transVector(Ogre::Vector3::ZERO),_dTrans(0.01f),_transFactor(0.05f),_rotYaw(0.0f),
  _rotPitch(0.0f),_rotFactor(0.07f),
  _cam(EngineView::getSingleton().getCurrentCamera()),
  _lookAtNode(0),
  _cameraNode(0),
  _camLocalZOffset(1.0f)
  {
    // TODO Auto-generated constructor stub
    
    _cameraNode = EngineView::getSingleton().getSceneManager()->createSceneNode("CAMERA_NODE");
    _cameraNode->attachObject(_cam);

    Ogre::String CAMERAATTACH("camera_attach");
    ZGame::CommandController* cmdCtrl = ZGame::CommandController::getSingletonPtr();
    COMMAND::ConsoleCommand cmd;
    cmd.bind(&ZGame::ControlModuleProto::attachNode,this);
    cmdCtrl->addCommand(CAMERAATTACH,cmd);
    _transMode[forw]=false;
    _transMode[backw]=false;
    _transMode[left]=false;
    _transMode[right]=false;
    _transMode[up]=false;
    _transMode[down]=false;
  }

  ControlModuleProto::~ControlModuleProto()
  {
    // TODO Auto-generated destructor stub
  }

  void ControlModuleProto::toggleMode(enum TransMode mode)
  {
    _transMode[mode] = !_transMode[mode];
  }

  bool ControlModuleProto::onKeyDown(const OIS::KeyEvent &evt)
  {
    if(evt.key == OIS::KC_W)
      {
        _transVector.z -= _dTrans;
        toggleMode(forw);
      }
    else if(evt.key == OIS::KC_S)
      {
        _transVector.z += _dTrans;
        toggleMode(backw);
      }
    else if(evt.key == OIS::KC_Q)
      {
        updateTransFactor(-0.1f);
      }
    else if(evt.key == OIS::KC_E)
      {
        updateTransFactor(0.1f);
      }
    else if(evt.key == OIS::KC_A)
      {
        _transVector.x -= _dTrans;
        toggleMode(left);
      }
    else if(evt.key == OIS::KC_D)
      {
        _transVector.x += _dTrans;
        toggleMode(right);
      }
    else if(evt.key == OIS::KC_UP)
      {
       _transVector.y += _dTrans;
       toggleMode(up);
      }
    else if(evt.key == OIS::KC_DOWN)
      {
        _transVector.y -= _dTrans;
        toggleMode(down);
      }
    else if(evt.key == OIS::KC_U)
    {
      _camLocalZOffset += 0.1f;
    }
    return true;
  }

  bool ControlModuleProto::onKeyUp(const OIS::KeyEvent &evt)
  {
    //turn off control signal
    if(evt.key == OIS::KC_W)
          {
            _transVector.z = 0;
            toggleMode(forw);
          }
        else if(evt.key == OIS::KC_S)
          {
            _transVector.z = 0;
            toggleMode(backw);
          }
        else if(evt.key == OIS::KC_A)
          {
            _transVector.x = 0;
            toggleMode(left);
          }
        else if(evt.key == OIS::KC_D)
          {
            _transVector.x = 0;
            toggleMode(right);
          }
        else if(evt.key == OIS::KC_UP)
          {
           _transVector.y = 0;
           toggleMode(up);
          }
        else if(evt.key == OIS::KC_DOWN)
          {
            _transVector.y = 0;
            toggleMode(down);
          }
        else if(evt.key == OIS::KC_L)
        {
          if(_lookAtNode)
          {
            _lookAtNode->scale(0.9f,0.9f,0.9f);
          }
        }
    return true;
  }

  bool ControlModuleProto::onMouseMove(const OIS::MouseEvent &evt)
  {
	  //_cam->yaw(Radian(-Ogre::Math::DegreesToRadians(_rotFactor*evt.state.X.rel)));
	  //_cam->pitch(Radian(-Ogre::Math::DegreesToRadians(_rotFactor*evt.state.Y.rel)));
    if(_lookAtNode)
    {
      _cameraNode->yaw(Radian(-Ogre::Math::DegreesToRadians(_rotFactor*evt.state.X.rel)),Ogre::Node::TS_WORLD);
    }
    

    return true;
  }

  bool ControlModuleProto::onMouseUp(const OIS::MouseEvent &evt, const OIS::MouseButtonID id)
  {
    return true;
  }

  bool ControlModuleProto::onMouseDown(const OIS::MouseEvent &evt, const OIS::MouseButtonID id)
  {
    return true;
  }

  bool ControlModuleProto::onUpdate(const Ogre::FrameEvent &evt)
  {
    //if(_theNode)
      //_theNode->moveRelative(_transVector);
      //_theNode->translate(_theNode->getPosition()+_transVector);
    //_cam->moveRelative(_transVector);
    
    if(_lookAtNode)
    {
      
      _lookAtNode->translate(_lookAtNode->getOrientation()*_transVector);
      //then rotate about lookatNode
      _cameraNode->setPosition(Ogre::Vector3::ZERO);
      _cameraNode->translate(_cameraNode->getLocalAxes(),Ogre::Vector3(0.0,0.0,10.0));
    }
    return true;
  }

  void ControlModuleProto::updateTransFactor(Ogre::Real factor)
  {
    _transFactor += factor;
    _dTrans = Math::Exp(_transFactor);
  }

  void ControlModuleProto::cleanUpNodes()
  {

    
  }

  bool ControlModuleProto::attachNode(std::vector<Ogre::String> &params)
  {
    ostringstream oss;
    
    bool fail = true;
    
    if(params.size() == 2)
    {
      oss << "Attaching Camera to node: " << params[1] << endl;
      //Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL,oss.str());
      Ogre::SceneManager* scnMgr = EngineView::getSingleton().getSceneManager();

      
      if(_lookAtNode)
      {
        oss << "Removing child! " << _cameraNode->getName() << endl;
        _lookAtNode->removeChild(_cameraNode->getName());
        _cameraNode->setPosition(Ogre::Vector3::ZERO);
      }
      
      //Get the specified Node as the LookAtNode.
      Ogre::SceneNode* root = scnMgr->getRootSceneNode();
      _lookAtNode = dynamic_cast<Ogre::SceneNode*>(root->getChild(params[1]));

      if(_lookAtNode)
      {
      oss << "Adding _cameraNode as a child to _lookAtNode: " << _cameraNode->getName() << endl;
     
      _lookAtNode->addChild(_cameraNode);
     
      //_lookAtNode->scale(0.0078125,0.0078125,0.0078125);
      _cameraNode->resetOrientation();
      Ogre::Quaternion rotQuat(Radian(-Ogre::Math::DegreesToRadians(45.0f)),Ogre::Vector3::UNIT_Y);
      //_cameraNode->setOrientation(rotQuat);
      //_cameraNode->setPosition(Ogre::Vector3(0.0,0.0,10.0));

      //_cameraNode->yaw(Radian(-Ogre::Math::DegreesToRadians(45.0f)),Ogre::Node::TS_WORLD);
      _cameraNode->translate(_cameraNode->getLocalAxes(),Ogre::Vector3(0.0,0.0,10.0));
      
      //_cameraNode->setAutoTracking(true,_lookAtNode,Ogre::Vector3::NEGATIVE_UNIT_Z,Ogre::Vector3(0.0,1.0,0.0));
      //_cameraNode->setFixedYawAxis(true);
      fail = false;
      }
    }
    else
    {
    oss << "camera_attach node_name will attach the scene camera to Ogre::Node node_name" << endl;
    oss << "you entered the wrong syntax. " << endl;
    }

    Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL,oss.str());

    return fail;
  }

}
