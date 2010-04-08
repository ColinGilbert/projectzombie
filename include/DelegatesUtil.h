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
#include "delegates/EntityDelegates.h"
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
        template<typename T>
        static void bindLifeCycleObserver(LifeCycleObserver &lfcObs,
            T& binder)
        {
            lfcObs.onInit.bind(&T::onInit,&binder);
            lfcObs.onUpdate.bind(&T::onUpdate,&binder);
            lfcObs.onDestroy.bind(&T::onDestroy,&binder);
        }
    }
    namespace Entities
    {
        template<typename T>
        static void bindEntityLifeCycleObserver(EntityLifeCycleObserver &lfcObs,
            T& binder)
        {
            lfcObs.onInit.bind(&T::onInit,&binder);
            lfcObs.onUpdate.bind(&T::onUpdate,&binder);
            lfcObs.onDestroy.bind(&T::onDestroy,&binder)
        }
        template<typename T>
        static void bindEntityAspects(EntityAspects &aspects, T& binder)
        {
            //aspects.onInitClient(&T::onInitClient,&binder);
            //aspects.onInitServer(&T::onInitServer,&binder);
            //aspects.onDestroyClient(&T::onDestroyClient,&binder);
            //aspects.onDestroyServer(&T::onDestroyServer,&binder);
            aspects.onSendConstruction.bind(&T::onSendConstruction,&binder);
            aspects.onRead.bind(&T::onRead,&binder);
            aspects.onWrite.bind(&T::onWrite,&binder);
        }
    }
    namespace EVENT
    {
        template<typename T>
        static void bindKeyObserver(KeyboardEvtObserver &keyObs,
            T& binder)
        {
            keyObs.kde.bind(&T::onKeyDown,&binder);
            keyObs.kue.bind(&T::onKeyUp,&binder);
        }
        static void clearKeyObs(EVENT::KeyboardEvtObserver &keyObs)
        {
            keyObs.kde.clear();
            keyObs.kue.clear();
        }
        template<typename T>
        static void bindMouseObserver(MouseEvtObserver &mouseObs,
            T& binder)
        {
            mouseObs.mde.bind(&T::onMouseDown,&binder);
            mouseObs.mme.bind(&T::onMouseMove,&binder);
            mouseObs.mue.bind(&T::onMouseUp,&binder);
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
