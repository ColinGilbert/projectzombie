/*
 * LifeCycleDelegatesUtil.h
 *
 *  Created on: Sep 28, 2008
 *      Author: bey0nd
 */

#ifndef LIFECYCLEDELEGATESUTIL_H_
#define LIFECYCLEDELEGATESUTIL_H_

#include "LifeCycleDelegates.h"
#include "EventDelegates.h"
namespace ZGame
{
  namespace LifeCycle
  {
    static void clearLfcObs(LifeCycleObserver& lfcObs)
    {
      lfcObs.onDestroy.clear();
      lfcObs.onInit.clear();
      lfcObs.onUpdate.clear();
    }
  }
  namespace EVENT
  {
    static void clearKeyObs(EVENT::KeyboardEvtObserver &keyObs)
    {
      keyObs.kde.clear();
      keyObs.kue.clear();
    }
    static void clearMouseObs(EVENT::MouseEvtObserver &mouseObs)
    {
      mouseObs.mde.clear();
      mouseObs.mme.clear();
      mouseObs.mue.clear();
    }
  }
}

#endif /* LIFECYCLEDELEGATESUTIL_H_ */
