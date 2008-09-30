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

  ControlModuleProto::ControlModuleProto() :_ctrlModes(NONE),_dz(0.1),_transFactor(0.05),
  _cam(EngineView::getSingleton().getCurrentCamera())
  {
    // TODO Auto-generated constructor stub
  }

  ControlModuleProto::~ControlModuleProto()
  {
    // TODO Auto-generated destructor stub
  }

  void ControlModuleProto::fillKeyObs(EVENT::KeyboardEvtObserver &obs)
  {
    obs.kde.bind(&ControlModuleProto::onKeyDown,this);
    obs.kue.bind(&ControlModuleProto::onKeyUp,this);
  }

  void ControlModuleProto::fillMouseObs(EVENT::MouseEvtObserver &obs)
  {
    obs.mde.bind(&ControlModuleProto::onMouseDown,this);
    obs.mue.bind(&ControlModuleProto::onMouseUp,this);
    obs.mme.bind(&ControlModuleProto::onMouseMove,this);
  }

  void ControlModuleProto::fillLfcObs(LifeCycle::LifeCycleObserver &obs)
  {
    obs.onUpdate.bind(&ControlModuleProto::onUpdate,this);
  }

  bool ControlModuleProto::onKeyDown(const OIS::KeyEvent &evt)
  {
    if(evt.key == OIS::KC_W)
      {
        _ctrlModes = FORWARD;
      }
    else if(evt.key == OIS::KC_S)
      {
        _ctrlModes = BACKWARD;
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
        _ctrlModes = LEFT;
      }
    else if(evt.key == OIS::KC_D)
      {
        _ctrlModes = RIGHT;
      }
    else if(evt.key == OIS::KC_UP)
      {
       _ctrlModes = UP;
      }
    else if(evt.key == OIS::KC_DOWN)
      {
        _ctrlModes = DOWN;
      }
    return true;
  }

  bool ControlModuleProto::onKeyUp(const OIS::KeyEvent &evt)
  {
    _ctrlModes = NONE;
    return true;
  }

  bool ControlModuleProto::onMouseMove(const OIS::MouseEvent &evt)
  {
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
    switch(_ctrlModes)
    {
      case FORWARD:
        _cam->moveRelative(Vector3(0.0,0.0,-_dz));
        break;
      case BACKWARD:
        _cam->moveRelative(Vector3(0.0,0.0,_dz));
        break;
      case LEFT:
        _cam->moveRelative(Vector3(-_dz,0.0,0.0));
        break;
      case RIGHT:
        _cam->moveRelative(Vector3(_dz,0.0,0.0));
        break;
      case UP:
        _cam->moveRelative(Vector3(0.0,_dz,0.0));
        break;
      case DOWN:
        _cam->moveRelative(Vector3(0.0,-_dz,0.0));
        break;
      default:
        break;
    }
    return true;
  }

  void ControlModuleProto::updateTransFactor(Real factor)
  {
    _transFactor += factor;
    _dz = Math::Exp(_transFactor);
  }

}
