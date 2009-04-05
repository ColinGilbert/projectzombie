/*
 * ControlModuleProto.cpp
 *
 *  Created on: Sep 29, 2008
 *      Author: bey0nd
 */

#include "ControlModuleProto.h"
#include "EventDelegates.h"
#include "LifeCycleDelegates.h"
#include "EngineView.h"
namespace ZGame
{
  using namespace std;
  using namespace Ogre;
  
  ControlModuleProto::ControlModuleProto() :_transVector(Ogre::Vector3::ZERO),_dTrans(0.1),_transFactor(0.05),_rotYaw(0.0),
  _rotPitch(0.0),_rotFactor(0.07),
  _cam(EngineView::getSingleton().getCurrentCamera())
  {
    // TODO Auto-generated constructor stub
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

  void ControlModuleProto::fillKeyObservers(EVENT::KeyboardEvtObserver &obs)
  {
    obs.kde.bind(&ControlModuleProto::onKeyDown,this);
    obs.kue.bind(&ControlModuleProto::onKeyUp,this);
  }

  void ControlModuleProto::fillMouseObservers(EVENT::MouseEvtObserver &obs)
  {
    obs.mde.bind(&ControlModuleProto::onMouseDown,this);
    obs.mue.bind(&ControlModuleProto::onMouseUp,this);
    obs.mme.bind(&ControlModuleProto::onMouseMove,this);
  }

  void ControlModuleProto::fillLfcObservers(LifeCycle::LifeCycleObserver &obs)
  {
    obs.onUpdate.bind(&ControlModuleProto::onUpdate,this);
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
        updateTransFactor(-0.1);
      }
    else if(evt.key == OIS::KC_E)
      {
        updateTransFactor(0.1);
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
    return true;
  }

  bool ControlModuleProto::onMouseMove(const OIS::MouseEvent &evt)
  {
	  _cam->yaw(Radian(-Ogre::Math::DegreesToRadians(_rotFactor*evt.state.X.rel)));
	  _cam->pitch(Radian(-Ogre::Math::DegreesToRadians(_rotFactor*evt.state.Y.rel)));

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
    _cam->moveRelative(_transVector);
    return true;
  }

  void ControlModuleProto::updateTransFactor(Ogre::Real factor)
  {
    _transFactor += factor;
    _dTrans = Math::Exp(_transFactor);
  }

}
