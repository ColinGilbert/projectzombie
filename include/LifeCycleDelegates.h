/*
 * LifeCycleDelegates.h
 *
 *  Created on: Aug 28, 2008
 *      Author: bey0nd
 */

#ifndef LIFECYCLEDELEGATES_H_
#define LIFECYCLEDELEGATES_H_

#include <Ogre.h>

#include "fastdelegate/FastDelegate.h"

namespace ZGame
{
  namespace LifeCycle
    {
        using namespace fastdelegate;

      typedef FastDelegate<bool(void) > LifeCycleEvent;
      typedef FastDelegate<bool(const Ogre::FrameEvent &evt) > LifeCycleEvent2;
      typedef FastDelegate<bool(Ogre::uint8 queueGroupId,
          const Ogre::String& invocation, bool& skipThisInvocation) > LifeCycleRdrQueueStart;

      struct LifeCycleObserver
            {
              LifeCycleEvent onInit;
              //LifeCycleEvent onUpdate;
              LifeCycleEvent2 onUpdate;
              LifeCycleEvent onDestroy;
              LifeCycleRdrQueueStart onRenderQueueStart;
            };

      //typedef FastDelegate<void(LifeCycleObserver) > LifeCycleSubject;
      //typedef FastDelegate<void(LifeCycleSubject&) > LifeCycleSubjectInjector;

      typedef FastDelegate<void(const LifeCycleObserver&) > LifeCycleSubject;
      //typedef FastDelegate1<LifeCycleSubject& > LifeCycleSubjectInjector;




    }
}

#endif /* LIFECYCLEDELEGATES_H_ */
