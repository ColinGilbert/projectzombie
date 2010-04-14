/**
* ZNetEntity.h
*
* This class defines a Networked enabled entity. 
* Subclass Replica and implements all virtual interfaces, in order to implment networking specific elements.
**/
#ifndef _ZGAME_NETENTITY_H
#define _ZGAME_NETENTITY_H

#include <Ogre.h>

#include <RakNetTypes.h>
#include <PacketPriority.h>
#include <ReplicaEnums.h>
#include <Replica.h>
#include <ReplicaManager.h>

#include "ZEntity.h"
#include "delegates/EntityDelegates.h"
#include "DelegatesUtil.h"

using namespace RakNet;
namespace ZGame
{
    namespace Networking
    {
        class ZNetEntity : public Replica
        {
        public:

            /** \brief Constructor for this class which takes in entity and mesh names.
            *
            * The constructor maps directly the entity name and mesh name into Ogre entity and mesh.
            */
            ZNetEntity(ReplicaManager &replica,SystemAddress ownerAddress, bool isServer);
            /** \brief Virtual destructor will clean and destroy this ZNetEntity.
            *
            */
            virtual ~ZNetEntity();

            /** \brief called to send construction */
            virtual ReplicaReturnResult
                SendConstruction(RakNetTime currentTime, SystemAddress systemAddress, unsigned int &flags,
                RakNet::BitStream* outBitStream, bool*includeTimestamp);
            /** \brief called to SendConstruction. */
            virtual ReplicaReturnResult
                SendDestruction(RakNet::BitStream *outBitStream, SystemAddress systemAddress, bool* includeTimestamp);

            /** \brief called to recieve destruction.*/
            virtual ReplicaReturnResult
                ReceiveDestruction(RakNet::BitStream* inBitStream, SystemAddress systemAddress, RakNetTime timestamp);

            /** \brief called to send scope change. */
            virtual ReplicaReturnResult
                SendScopeChange(bool inScope, RakNet::BitStream* outBitStream, RakNetTime currentTime, SystemAddress sytemAddress,
                bool* includeTimestamp);

            /** \brief called to recieve scope change. */
            virtual ReplicaReturnResult
                ReceiveScopeChange(RakNet::BitStream* inBitStream, SystemAddress sytemAddress, RakNetTime timestamp);

            /** \brief called to serialize this entity. */
            virtual ReplicaReturnResult
                Serialize(bool* sendTimestamp, RakNet::BitStream* outBitStream, RakNetTime lastSendTime,
                PacketPriority* priority, PacketReliability* reliability, RakNetTime currentTIme, SystemAddress systemAddress, unsigned int &flags);

            /** \brief called to deserialize this entity. */
            virtual ReplicaReturnResult
                Deserialize(RakNet::BitStream* inBitStream, RakNetTime timestamp, RakNetTime lastDeserializeTime, SystemAddress systemAddress);

            /** \brief called to get the sort priority for this entity during serialization. */  
            int
                GetSortPriority(void) const {return 0;}

            void setEntityAspects(Entities::EntityAspects &entAspects) { _entityAspects = entAspects;}

            /** \brief return whether this object is to be destructed **/


        protected:
        private:
            ReplicaManager &_replica;
            /** \brief The owner address of this entity.**/
            SystemAddress _initatorAddress; 
            Entities::EntityAspects _entityAspects;
            bool _isServer;
            //Ogre::String _typeStr;
        };


        //static methods.
        /*\brief This templated static method will construct the composite ZNetEntity given EntityClass.
        *
        *\precondition: We expect EntityClass dervies from ZEntity, and thus we will not do any type checking here.
        *\postcondition: the composite entity is created.
        *\note: For server you need to pass in the NetworkIDManager. It seems that if you call GetNetworkID() before calling construction (or DisableInterface) of replica it will crash.
        */
        ///////////template methods//////////////////
        template<typename EntityClass>
        void
            constructCompositeZNetEntity(boost::shared_ptr<ZEntity> &entityShrPtr, boost::shared_ptr<ZNetEntity> &znetEntityShrPtr,EntityClass* entPtr,
            ReplicaManager* replicaManager, SystemAddress creatorAddress, NetworkID* netId,
            bool isServer)
        {
            cout << "In ZNetEntity construct composite ZNetEntity." << endl;

            //create ZNetEntity.
            entPtr = new EntityClass();
            znetEntityShrPtr.reset(new ZNetEntity(*replicaManager, creatorAddress, isServer));
            entityShrPtr.reset(static_cast<ZEntity*>(entPtr));


            if(isServer)
            {
                //Note: We need to call this in order to get around a bug in Raknet (crashes if we don't call.) If we manually set the NetworkIDManager right after creating the replica, it will work.
                //Instead, let's just call disable replica interface. One can call this function elsewhere after the composite object has been created. It won't affect anything.
                replicaManager->DisableReplicaInterfaces(znetEntityShrPtr.get(), 
                    REPLICA_RECEIVE_DESTRUCTION | REPLICA_RECEIVE_SCOPE_CHANGE); 
                NetworkID nid;
                nid = znetEntityShrPtr->GetNetworkID();
                ostringstream oss;
                oss << "Entity" << nid.localSystemAddress;
                entPtr->setEntityName(oss.str());
            }
            else
            {
                znetEntityShrPtr->SetNetworkID(*netId);
                ostringstream oss;
                oss << "Entity" << netId->localSystemAddress;
                entPtr->setEntityName(oss.str());

            }


            //////////////We need to setup server/client specific entity aspects.//////////////////
            Entities::EntityAspects entAspects;
            Entities::bindEntityAspects(entAspects,*entPtr,isServer);
            if(isServer)
                entPtr->onInitServer();
            else
                entPtr->onInitClient();
            znetEntityShrPtr->setEntityAspects(entAspects); //DO NOT forget to set, else exception will be thrown.
        }
    }
}

#endif
