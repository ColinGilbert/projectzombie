#ifndef _ZGAME_ENTITY_DELEGATES_H
#define _ZGAME_ENTITY_DELEGATES_H
#include <Ogre.h>
#include "LifeCycleDelegates.h"
#include "ReplicaEnums.h"
#include "Replica.h"
#include "ReplicaManager.h"

namespace ZGame
{
    namespace Entities
    {
        typedef fd::delegate<bool(Ogre::Vector3 &pos, Ogre::Quaternion &orient) > EntityUpdateEvent;
        typedef fd::delegate<bool(Ogre::String &typeStr, RakNet::BitStream* outBitStream) > EntitySendConstruction;
        struct EntityAspects
        {
            //LifeCycleEvent onInitClient;
            //LifeCycleEvent onInitServer;
            //LifeCycleEvent onDestroyClient;
            //LifeCycleEvent onDestroyServer;
            EntitySendConstruction onSendConstruction;
            EntityUpdateEvent onRead;
            EntityUpdateEvent onWrite;
            
        };
        struct EntityLifeCycleObserver
        {
            LifeCycle::LifeCycleEvent onInit;
            LifeCycle::LifeCycleEvent onRead;
            LifeCycle::LifeCycleEvent onDestroy;
        };
    }
}

#endif