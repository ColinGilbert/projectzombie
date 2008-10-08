/*
 * LFCObsInjector.h
 *
 *  Created on: Oct 7, 2008
 *      Author: bey0nd
 */

#ifndef LFCOBSINJECTOR_H_
#define LFCOBSINJECTOR_H_

namespace ZGame
{

  namespace LifeCycle
  {
    struct LifeCycleObserver;
  }
  namespace EVENT
  {
    struct MouseEvtObserver;
    struct KeyboardEvtObserver;
  }
  /**
   * This class defines a Lifce Cycle Observer Injector interface
   */
  class LFCObsInjector
  {
  public:
    virtual
    ~LFCObsInjector()
    {
    }

    virtual void
    fillLfcObservers(LifeCycle::LifeCycleObserver &obs) = 0;

  protected:
    LFCObsInjector()
    {
    }

  };
  /**
   * This class defines a KeyboardEvtObserverInjector
   */
  class KeyEvtObsInjector
  {
  public:
    virtual
    ~KeyEvtObsInjector()
    {
    }

    virtual void
    fillKeyObservers(EVENT::KeyboardEvtObserver &obs) = 0;
  protected:
    KeyEvtObsInjector()
    {

    }
  };

  class MouseEvtObsInjector
  {
  public:
    virtual
    ~MouseEvtObsInjector()
    {

    }
    virtual void
    fillMouseObservers(EVENT::MouseEvtObserver &obs) = 0;
  protected:
    MouseEvtObsInjector()
    {

    }
  };

}

#endif /* LFCOBSINJECTOR_H_ */
