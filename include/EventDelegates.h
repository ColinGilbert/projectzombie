/*
 * ZombieDefs.h
 *
 *  Created on: Aug 24, 2008
 *      Author: bey0nd
 */

#ifndef ZOMBIEDEFS_H_
#define ZOMBIEDEFS_H_

#define OIS_DYNAMIC_LIB
#include <OIS/OIS.h>
#include "fastdelegate/delegate.h"

namespace ZGame
{
  namespace EVENT
  {
    typedef fd::delegate<bool(const OIS::MouseEvent&)> ZMouseMoveEvt;
    typedef fd::delegate<bool(const OIS::MouseEvent&,const OIS::MouseButtonID)> ZMouseUpEvt;
    typedef fd::delegate<bool(const OIS::MouseEvent&,const OIS::MouseButtonID)> ZMouseDownEvt;
    typedef fd::delegate<bool(const OIS::KeyEvent&)> ZKeyUpEvt;
    typedef fd::delegate<bool(const OIS::KeyEvent&)> ZKeyDownEvt;

    struct MouseEvtObserver
    {
      ZMouseMoveEvt mme;
      ZMouseUpEvt mue;
      ZMouseDownEvt mde;
    };

    struct KeyboardEvtObserver
    {
      ZKeyUpEvt kue;
      ZKeyDownEvt kde;
    };

    typedef fd::delegate<void(const KeyboardEvtObserver&)> KeyEvtSubject;
    typedef fd::delegate<void(const MouseEvtObserver&)> MouseEvtSubject;
    typedef fd::delegate<void(const KeyEvtSubject&)> KeyEvtSubjectInjector;

  }

}


#endif /* ZOMBIEDEFS_H_ */
