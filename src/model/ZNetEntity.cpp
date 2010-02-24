#include <iostream>

#include "ZNetEntity.h"

using namespace ZGame;

ZNetEntity::ZNetEntity(const Ogre::String &entName, const Ogre::String &meshName) : 
ZEntity(entName,meshName)
{
}

/**
*This method is called for to tell others sytems to create an instance of this class. 
*
*@param currentTime Current time, in milliseconds since Jan 1, 1970.
*@param systemAddress The Raknet system address of the caller.
*@param flags Some sort of flag.
*@param outBitStream The Raknet out bit stream.
*@param includeTimeStamp Boolean to determine whether to include timestamp.
*/
ReplicaReturnResult
ZNetEntity::SendConstruction(RakNetTime currentTime, SystemAddress systemAddress, unsigned int &flags,
                             RakNet::BitStream* outBitStream, bool*includeTimestamp)
{
    return REPLICA_PROCESSING_DONE;
}

/**
*Called when telling other systems the destruction of this entity. One can destroy this entity when calling this.
*
*
*/
ReplicaReturnResult
ZNetEntity::SendDestruction(RakNet::BitStream *outBitStream, SystemAddress systemAddress, bool* includeTimestamp)
{
    return REPLICA_PROCESSING_DONE;
}
/**
*Called when receiving destruction events for this entity from some other remote system.
*/
ReplicaReturnResult
ZNetEntity::ReceiveDestruction(RakNet::BitStream* inBitStream, SystemAddress systemAddress, RakNetTime timestamp)
{
    return REPLICA_PROCESSING_DONE;
}

/**
*Called when sending scope change.
*/
ReplicaReturnResult
ZNetEntity::SendScopeChange(bool inScope, RakNet::BitStream* outBitStream, RakNetTime currentTime, SystemAddress sytemAddress,
                            bool* includeTimestamp)
{
    return REPLICA_PROCESSING_DONE;
}

/**
*Called when receiving scope changes.
*/
ReplicaReturnResult
ZNetEntity::ReceiveScopeChange(RakNet::BitStream* inBitStream, SystemAddress sytemAddress, RakNetTime timestamp)
{
    return REPLICA_PROCESSING_DONE;
}
/**
*Called when serializing this entity.
*/
ReplicaReturnResult
ZNetEntity::Serialize(bool* sendTimestamp, RakNet::BitStream* outBitStream, RakNetTime lastSendTime,
                      PacketPriority* priority, PacketReliability* reliability, RakNetTime currentTIme, SystemAddress systemAddress, unsigned int &flags)
{
    return REPLICA_PROCESSING_DONE;
}

/**
*Called when deserializing this entity.
*/
ReplicaReturnResult
ZNetEntity::Deserialize(RakNet::BitStream* inBitStream, RakNetTime timestamp, RakNetTime lastDeserializeTime, SystemAddress systemAddress)
{
    return REPLICA_PROCESSING_DONE;
}





