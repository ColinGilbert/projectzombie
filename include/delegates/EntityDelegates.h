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
        //typedef FastDelegate2<bool(Ogre::Vector3 &pos, Ogre::Quaternion &orient) > EntityUpdateEvent;
        typedef fastdelegate::FastDelegate<bool(Ogre::Vector3 &pos, Ogre::Quaternion &orient) > EntityUpdateEvent;
        //typedef FastDelegate2<bool(Ogre::String &typeStr, RakNet::BitStream* outBitStream) > EntitySendConstruction;
        typedef fastdelegate::FastDelegate<bool(Ogre::String &typeStr, RakNet::BitStream* outBitStream) > EntitySendConstruction;
        struct EntityAspects
        {
            LifeCycle::LifeCycleEvent onDestroy;
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