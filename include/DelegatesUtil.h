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
#include "LifeCycleRegister.h"

namespace ZGame
{
    namespace LifeCycle
    {



#define HAS_MEM_FUNC(func, name)                                        \
    template<typename T, typename Sign>                                 \
        struct name {                                                       \
        typedef char yes[1];                                            \
        typedef char no [2];                                            \
        template <typename U, U> struct type_check;                     \
        template <typename _1> static yes &chk(type_check<Sign, &_1::func> *); \
        template <typename   > static no  &chk(...);                    \
        static bool const value = sizeof(chk<T>(0)) == sizeof(yes);     \
        }

        static const unsigned int LFC_ON_DESTROY = 0x01;
        static const unsigned int LFC_ON_INIT = 0x02;
        static const unsigned int LFC_ON_UPDATE = 0x04;
        static const unsigned int LFC_ON_RENDER_QUEUE_START = 0x08;

        static void clearLfcObs(LifeCycleObserver& lfcObs)
        {
            lfcObs.onDestroy.clear();
            lfcObs.onInit.clear();
            lfcObs.onUpdate.clear();
        }

        template<bool C, typename T = void>
        struct enable_if{
            typedef T type;
        };

        template<typename T>
        struct enable_if<false, T> {};

        HAS_MEM_FUNC(onInit, has_on_init);
        HAS_MEM_FUNC(onRenderQueueStart, has_on_render_queue_start);
        HAS_MEM_FUNC(onDestroy, has_on_destroy);
        HAS_MEM_FUNC(onUpdate, has_on_update_with_frame);


        template<typename T>
        void
            bindIfOnInit(T* t, LifeCycleObserver& lfcObs, typename enable_if<has_on_init<T, bool(T::*)(ZGame::ZInitPacket initPacket) >::value, T>::type* = 0)
        {
            lfcObs.onInit.bind(t, &T::onInit);
        }
        template<typename T>
        void
            bindIfOnInit(T* t, LifeCycleObserver& lfcObs, typename enable_if<!has_on_init<T, bool(T::*)(ZGame::ZInitPacket initPacket) >::value, T>::type* = 0)
        {
            lfcObs.onInit.clear();
        }

        template<typename T>
        void
            bindIfOnRenderQueueStart(T* t, LifeCycleObserver& lfcObs,  typename enable_if<has_on_render_queue_start<T, 
                bool(T::*)(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& skipThisInvocation) >::value, T>::type* = 0)
        {
            lfcObs.onRenderQueueStart.bind(t, &T::onRenderQueueStart);
        }

        template<typename T>
        void
            bindIfOnRenderQueueStart(T* t, LifeCycleObserver& lfcObs, typename enable_if<!has_on_render_queue_start<T, 
                bool(T::*)(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& skipThisInvocation) >::value, T>::type* = 0 )
        {
            lfcObs.onRenderQueueStart.clear();
        }

        template<typename T>
        void
            bindIfOnDestroy(T* t, LifeCycleObserver& lfcObs, typename enable_if<has_on_destroy<T, bool(T::*)() >::value, T>::type* = 0)
        {
            lfcObs.onDestroy.bind(t, &T::onDestroy);
        }

        template<typename T>
        void
            bindIfOnDestroy(T* t, LifeCycleObserver& lfcObs, typename enable_if<!has_on_destroy<T, bool(T::*)() >::value, T>::type* = 0)
        {
            lfcObs.onDestroy.clear();
        }

        template<typename T>
        void     
            bindIfOnUpdateWithFrame(T* t, LifeCycleObserver& lfcObs, typename enable_if<has_on_update_with_frame<T, 
                bool(T::*)(const Ogre::FrameEvent &evt) >::value, T>::type* = 0)
        {
            lfcObs.onUpdate.bind(t, &T::onUpdate);
        }

        template<typename T>
        void
            bindIfOnUpdateWithFrame(T* t, LifeCycleObserver& lfcObs, typename enable_if<!has_on_update_with_frame<T, 
                bool(T::*)(const Ogre::FrameEvent &evt) >::value, T>::type* = 0)
        {
            lfcObs.onUpdate.clear();
        }
        /**
        * This static function will bind LifeCycleObservers given any type. It is fine if the type does not provide all life-cycle functions.
        * \note This function was not written with performance on mind. You should call this function only a few times during initialization.
        **/
        template<typename T>
        static void bindAndRegisterLifeCycleObserver(LifeCycleRegister &lfcReg, LifeCycleObserver &lfcObs,
            T& binder, unsigned eventMask = LFC_ON_DESTROY | LFC_ON_INIT | LFC_ON_UPDATE | LFC_ON_RENDER_QUEUE_START)
        {
            eventMask & LFC_ON_INIT ? bindIfOnInit<T>(&binder, lfcObs) : lfcObs.onUpdate.clear();
            eventMask & LFC_ON_RENDER_QUEUE_START ? bindIfOnRenderQueueStart<T>(&binder, lfcObs) : lfcObs.onRenderQueueStart.clear();
            eventMask & LFC_ON_DESTROY ? bindIfOnDestroy<T>(&binder, lfcObs) : lfcObs.onDestroy.clear();
            eventMask & LFC_ON_UPDATE ? bindIfOnUpdateWithFrame<T>(&binder, lfcObs) : lfcObs.onUpdate.clear();

            lfcReg.registerLfcObs(lfcObs);
        }
    }
    namespace Entities
    {
        template<typename T>
        static void bindEntityAspects(EntityAspects &aspects, T& binder, bool isServer)
        {
            if(isServer)
                aspects.onDestroy.bind(&binder,&T::onDestroyServer);
            else
                aspects.onDestroy.bind(&binder,&T::onDestroyClient);
            aspects.onSendConstruction.bind(&binder,&T::onSendConstruction);
            aspects.onRead.bind(&binder,&T::onRead);
            aspects.onWrite.bind(&binder,&T::onWrite);
        }
    }
    namespace EVENT
    {
        template<typename T>
        static void bindKeyObserver(KeyboardEvtObserver &keyObs,
            T& binder)
        {
            keyObs.kde.bind(&binder,&T::onKeyDown);
            keyObs.kue.bind(&binder,&T::onKeyUp);
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
            mouseObs.mde.bind(&binder,&T::onMouseDown);
            mouseObs.mme.bind(&binder,&T::onMouseMove);
            mouseObs.mue.bind(&binder,&T::onMouseUp);
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
