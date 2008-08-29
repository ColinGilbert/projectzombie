/*
 * LifeCycleDelegates.h
 *
 *  Created on: Aug 28, 2008
 *      Author: bey0nd
 */

#ifndef LIFECYCLEDELEGATES_H_
#define LIFECYCLEDELEGATES_H_

#include "fastdelegate/delegate.h"

namespace ZGame
{
  namespace LifeCycle
    {

      typedef fd::delegate<bool(void)> LifeCycleEvent;

      struct LifeCycleObserver
            {
              LifeCycleEvent onInit;
              LifeCycleEvent onUpdate;
              LifeCycleEvent onDestroy;
            };

      typedef fd::delegate<void(LifeCycleObserver)> LifeCycleSubject;
      typedef fd::delegate<void(LifeCycleSubject&)> LifeCycleSubjectInjector;



    }
}

#endif /* LIFECYCLEDELEGATES_H_ */
