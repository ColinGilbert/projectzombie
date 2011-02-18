/*
* ZombieDefs.h
*
*  Created on: Aug 24, 2008
*      Author: bey0nd
*/

#ifndef ZOMBIEDEFS_H_
#define ZOMBIEDEFS_H_

//#define OIS_DYNAMIC_LIB
#include <OIS\OIS.h>
#include "fastdelegate/FastDelegate.h"

namespace ZGame
{
    namespace EVENT
    {
        
        using namespace fastdelegate;
        typedef FastDelegate<bool(const OIS::MouseEvent&) > ZMouseMoveEvt;
        typedef FastDelegate<bool(const OIS::MouseEvent&, const OIS::MouseButtonID) > ZMouseEvt;
        //typedef FastDelegate<bool(const OIS::MouseEvent&, const OIS::MouseButtonID) > ZMouseDownEvt;
        typedef FastDelegate<bool(const OIS::KeyEvent&) > ZKeyUpEvt;
        typedef FastDelegate<bool(const OIS::KeyEvent&) > ZKeyDownEvt;
        typedef FastDelegate<void(const unsigned int modifierState) > ZModifierStateEvt;

        struct MouseEvtObserver
        {
            ZMouseMoveEvt mme;
            ZMouseEvt mue;
            ZMouseEvt mde;
        };

        struct KeyboardEvtObserver
        {
            ZKeyUpEvt kue;
            ZKeyDownEvt kde;
        };



        typedef FastDelegate<void(const KeyboardEvtObserver&) > KeyEvtSubject;
        typedef FastDelegate<void(const MouseEvtObserver&) > MouseEvtSubject;
        typedef FastDelegate<void(const KeyEvtSubject&) > KeyEvtSubjectInjector;

    }

}


#endif /* ZOMBIEDEFS_H_ */
