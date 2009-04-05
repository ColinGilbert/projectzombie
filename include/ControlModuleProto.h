/*
 * ControlModuleProto.h
 * This is a prototype for the control module (for controlling objects). This is a place holder only. We want
 * to concentrate our efforts on implement other more important features for now and worry about
 * proper design of control module later.
 *  Created on: Sep 29, 2008
 *      Author: bey0nd
 */

#ifndef CONTROLMODULEPROTO_H_
#define CONTROLMODULEPROTO_H_
#include <Ogre.h>
#include <OIS/OIS.h>
#include "ObsInjectors.h"
namespace ZGame
{

  namespace EVENT
  {
    struct KeyboardEvtObserver;
    struct MouseEvtObserver;
  }
  namespace LifeCycle
  {
    struct LifeCycleObserver;
  }
  class ControlModuleProto : public LFCObsInjector,
      public KeyEvtObsInjector,
      public MouseEvtObsInjector
  {
  public:
    ControlModuleProto();
    virtual
    ~ControlModuleProto();

    //register observers for event
    void
    fillKeyObservers(EVENT::KeyboardEvtObserver &obs);
    void
    fillMouseObservers(EVENT::MouseEvtObserver &obs);
    void
    fillLfcObservers(LifeCycle::LifeCycleObserver &obs);
    //events methods
    bool
    onKeyDown(const OIS::KeyEvent &evt);
    bool
    onKeyUp(const OIS::KeyEvent &evt);
    bool
    onMouseMove(const OIS::MouseEvent &evt);
    bool
    onMouseUp(const OIS::MouseEvent &evt, const OIS::MouseButtonID id);
    bool
    onMouseDown(const OIS::MouseEvent &evt, const OIS::MouseButtonID id);
    bool
    onUpdate(const Ogre::FrameEvent &evt);
  protected:

	Ogre::Vector3 _transVector;
	Ogre::Real _dTrans; //change in translation
	Ogre::Real _transFactor; //translation factor;
	Ogre::Real _rotYaw;
	Ogre::Real _rotPitch;
	Ogre::Real _rotFactor;
    Ogre::Camera* _cam;

    enum TransMode
    {
      forw, backw, left, right, up, down
    };
    bool _transMode[6];

    void
	updateTransFactor(Ogre::Real factor);
    void
    toggleMode(enum TransMode mode);
  };

}

#endif /* CONTROLMODULEPROTO_H_ */
