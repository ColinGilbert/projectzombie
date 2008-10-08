/*
 * LifeCycleDelegatesUtil.h
 *
 *  Created on: Sep 28, 2008
 *      Author: bey0nd
 */

#ifndef LIFECYCLEDELEGATESUTIL_H_
#define LIFECYCLEDELEGATESUTIL_H_

#include "LifeCycleDelegates.h"

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
}

#endif /* LIFECYCLEDELEGATESUTIL_H_ */
