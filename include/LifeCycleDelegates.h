/*
 * LifeCycleDelegates.h
 *
 *  Created on: Aug 28, 2008
 *      Author: bey0nd
 */

#ifndef LIFECYCLEDELEGATES_H_
#define LIFECYCLEDELEGATES_H_

#include <Ogre.h>

#include "fastdelegate/delegate.h"

namespace ZGame
{
  namespace LifeCycle
    {

      typedef fd::delegate<bool(void)> LifeCycleEvent;
      typedef fd::delegate<bool(const Ogre::FrameEvent &evt)> LifeCycleEvent2;

      struct LifeCycleObserver
            {
              LifeCycleEvent onInit;
              //LifeCycleEvent onUpdate;
              LifeCycleEvent2 onUpdate;
              LifeCycleEvent onDestroy;
            };

      typedef fd::delegate<void(LifeCycleObserver)> LifeCycleSubject;
      typedef fd::delegate<void(LifeCycleSubject&)> LifeCycleSubjectInjector;



    }
}

#endif /* LIFECYCLEDELEGATES_H_ */
