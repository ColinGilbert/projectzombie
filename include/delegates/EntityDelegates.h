#pragma once

#include "ZPrerequisites.h"

#include "LifeCycleDelegates.h"
#include "ReplicaEnums.h"
#include "Replica.h"
#include "ReplicaManager.h"

using fastdelegate::FastDelegate;

namespace ZGame
{
    namespace Entities
    {
        static const unsigned int ONBUILD = 0;

        class ZEntity;
        class RenderEntitiesManager;
        class EntitiesManager;
        class ZEntityResource;
        typedef FastDelegate<bool(Ogre::Vector3 &pos, Ogre::Quaternion &orient) > EntityUpdateEvent;
        typedef FastDelegate<bool(const Ogre::Vector3 &pos, const Ogre::Quaternion &orient) > ConstEntityUpdateEvent;
        typedef FastDelegate<void(unsigned int) > EntityOnEventEvent;

        //typedef FastDelegate2<bool(Ogre::String &typeStr, RakNet::BitStream* outBitStream) > EntitySendConstruction;
        typedef FastDelegate<bool(Ogre::String &typeStr, RakNet::BitStream* outBitStream) > EntitySendConstruction;
        //typedef FastDelegate<bool(const ZEntityResource* const res, const EntityUpdateEvent* const read) > CreateRenderEntDlg; 
        typedef FastDelegate<Entities::RenderEntitiesManager* (void) > GetRenderEntitiesManager;
        typedef FastDelegate<Entities::EntitiesManager* (void) > GetEntitiesManager;
        struct EntityAspects
        {
            LifeCycle::LifeCycleEvent onDestroy;
            EntitySendConstruction onSendConstruction;
            EntityUpdateEvent onRead;
            EntityUpdateEvent onWrite;

        };
	/**
        struct EntityLifeCycleObserver
        {
            LifeCycle::LifeCycleEvent onInit;
            LifeCycle::LifeCycleEvent onRead;
            LifeCycle::LifeCycleEvent onDestroy;
	    };**/
    }
}

