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
#include "KeyEventRegister.h"
#include "MouseEventRegister.h"

namespace ZGame
{
    namespace LifeCycle
    {
        template<bool C, typename T = void>
        struct enable_if{
            typedef T type;
        };

        template<typename T>
        struct enable_if<false, T> {};

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
#define BIND_IF(bindname, func1, func2, name, sign)   \
        template<typename T> \
        bool     \
            bindname(T* t, LifeCycleObserver& lfcObs, typename enable_if<name<T, \
            sign >::value, T>::type* = 0)    \
        {   \
            func1;  \
            return true;    \
        }   \
        template<typename T>    \
        bool    \
            bindname(T* t, LifeCycleObserver& lfcObs, typename enable_if<!name<T,    \
            sign>::value, T>::type* = 0)    \
        {   \
            func2;  \
            return false;   \
        }

        static const unsigned int LFC_ON_DESTROY = 0x01;
        static const unsigned int LFC_ON_INIT = 0x02;
        static const unsigned int LFC_ON_UPDATE = 0x04;
        static const unsigned int LFC_ON_RENDER_QUEUE_START = 0x08;
        static const unsigned int LFC_ON_RENDER_QUEUE_END = 0x10;
     
        static const unsigned int LFC_DEFAULT = LFC_ON_INIT | LFC_ON_UPDATE | LFC_ON_DESTROY;

        static void clearLfcObs(LifeCycleObserver& lfcObs)
        {
            lfcObs.onDestroy.clear();
            lfcObs.onInit.clear();
            lfcObs.onUpdate.clear();
        }

        HAS_MEM_FUNC(onInit, has_on_init);
        BIND_IF(bindIfOnInit, lfcObs.onInit.bind(t, &T::onInit), lfcObs.onInit.clear(), has_on_init, bool(T::*)(ZGame::ZInitPacket initPacket));
        
        HAS_MEM_FUNC(onRenderQueueStart, has_on_render_queue_start);
        BIND_IF(bindIfOnRenderQueueStart, lfcObs.onRenderQueueStart.bind(t, &T::onRenderQueueStart), 
            lfcObs.onRenderQueueStart.clear(), has_on_render_queue_start, 
            bool(T::*)(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& skipThisInvocation));

        HAS_MEM_FUNC(onRenderQueueEnd, has_on_render_queue_end);
        BIND_IF(bindIfOnRenderQueueEnd, lfcObs.onRenderQueueEnd.bind(t, &T::onRenderQueueEnd),
            lfcObs.onRenderQueueEnd.clear(), has_on_render_queue_end,
            bool(T::*)(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& skipThisInvocation));
        
        HAS_MEM_FUNC(onDestroy, has_on_destroy);
        BIND_IF(bindIfOnDestroy, lfcObs.onDestroy.bind(t, &T::onDestroy), 
            lfcObs.onDestroy.clear(), has_on_destroy, bool(T::*)());
        
        HAS_MEM_FUNC(onUpdate, has_on_update_with_frame);
        BIND_IF(bindIfOnUpdateWithFrame, lfcObs.onUpdate.bind(t, &T::onUpdate), 
            lfcObs.onUpdate.clear(), has_on_update_with_frame, bool(T::*)(const Ogre::FrameEvent &evt));

        /**
        * This static function will bind LifeCycleObservers given any type. It is fine if the type does not provide all life-cycle functions.
        * \note This function was not written with performance on mind. You should call this function only a few times during initialization.
        **/
        template<typename T>
        static void bindAndRegisterLifeCycleObserver(LifeCycleRegister &lfcReg, LifeCycleObserver &lfcObs,
            T& binder, unsigned eventMask = LFC_DEFAULT) 
        {
            using Ogre::Exception;
            LifeCycle::clearLfcObs(lfcObs);
            if(eventMask & LFC_ON_INIT) 
            {
                if(!bindIfOnInit<T>(&binder, lfcObs))
                {
                    OGRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "OnInit is not implemented", "DeletatesUtil::bindAndRegisterLifeCycleObserver");
                }
            }    

            if(eventMask & LFC_ON_RENDER_QUEUE_START)
            {
                if(!bindIfOnRenderQueueStart<T>(&binder, lfcObs))
                {
                    OGRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "On Render Queue Start is not implemented", "DeletatesUtil::bindAndRegisterLifeCycleObserver");
                }
            }
            if(eventMask & LFC_ON_RENDER_QUEUE_END)
            {
                if(!bindIfOnRenderQueueEnd<T>(&binder, lfcObs))
                {
                    OGRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "On Render Queue End is not implemented", "DeletatesUtil::bindAndRegisterLifeCycleObserver");
                }
            }

            if(eventMask & LFC_ON_DESTROY) 
            {
                if(!bindIfOnDestroy<T>(&binder, lfcObs))
                {
                    OGRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "onDestroy is not implemented", "DeletatesUtil::bindAndRegisterLifeCycleObserver");
                }
            }
            if(eventMask & LFC_ON_UPDATE)
            {
                if(!bindIfOnUpdateWithFrame<T>(&binder, lfcObs))
                {
                    OGRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "onUpdate is not implemented", "DeletatesUtil::bindAndRegisterLifeCycleObserver");
                }
            }
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
        static void bindAndRegisterKeyObserver(KeyEventRegister &keyReg, KeyboardEvtObserver &keyObs,
            T& binder)
        {
            keyObs.kde.bind(&binder,&T::onKeyDown);
            keyObs.kue.bind(&binder,&T::onKeyUp);
            keyReg.registerKeyObs(keyObs);
            clearKeyObs(keyObs);

        }
        static void clearKeyObs(EVENT::KeyboardEvtObserver &keyObs)
        {
            keyObs.kde.clear();
            keyObs.kue.clear();
        }
        template<typename T>
        static void bindAndRegisterMouseObserver(MouseEventRegister &mouseReg, MouseEvtObserver &mouseObs,
            T& binder)
        {
            mouseObs.mde.bind(&binder,&T::onMouseDown);
            mouseObs.mme.bind(&binder,&T::onMouseMove);
            mouseObs.mue.bind(&binder,&T::onMouseUp);
            mouseReg.registerMouseObs(mouseObs);
            clearMouseObs(mouseObs);
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
