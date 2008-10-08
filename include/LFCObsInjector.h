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
    class LFCObsInjector
    {
    public:
      virtual
      ~LFCObsInjector();

      virtual void fillLfcObservers(LifeCycleObserver &obs) = 0;

    protected:
      LFCObsInjector();

    };

  }

}

#endif /* LFCOBSINJECTOR_H_ */
