#include <iostream>

using namespace std;

#include <BitStream.h>
#include "StringTable.h"

#include "net/ZNetEntity.h"
#include "entities/EntitiesDefs.h"
#include "net/NetClientController.h"

using namespace ZGame;
using namespace Networking;




ZNetEntity::ZNetEntity(ReplicaManager &replica,SystemAddress initatorAddress,
                       bool isServer) : 
_replica(replica), _initatorAddress(initatorAddress),
_isServer(isServer)
{    
}
/**
*This will do clean up operations on the ZNetEntity. It will dereplicate. The de-replication processes is server/client dependent. 
*
*\precondition We assume during server-side and client-side construction of of this object, send deconstruction interface call-backs 
*has been set with respect to server and client. Meaning, we send out dereplication on the server to clients. While destruction on the client-side
*no deconstruction events shall be sent. This is due to our design where we disable client-side construction of ZNetEntities. We ALSO assume that _entityAspects will
*be valid when we make the delegate call. THIS IMPLIES an ordering to the destruction of the composite Entity.
*
*/
ZNetEntity::~ZNetEntity()
{
    cout << "In ZNetEntity::~ZNetEntity() " << endl;
    _entityAspects.onDestroy();
    _replica.Destruct(this, UNASSIGNED_SYSTEM_ADDRESS, true);
    _replica.DereferencePointer(this);
}

/**
*This method is called to tell others sytems to create an instance of this class. 
*
*@param currentTime Current time, in milliseconds since Jan 1, 1970.
*@param systemAddress The Raknet system address of the participant.
*@param flags Some sort of flag.
*@param outBitStream The Raknet out bit stream.
*@param includeTimeStamp Boolean to determine whether to include timestamp.
*/
ReplicaReturnResult
ZNetEntity::SendConstruction(RakNetTime currentTime, SystemAddress systemAddress, unsigned int &flags,
                             RakNet::BitStream* outBitStream, bool*includeTimestamp)
{
    using namespace RakNet;
    //RakNet::StringTable::Instance()->EncodeString(_typeStr.c_str(),255,outBitStream);
    Ogre::String typeStr;
    //Call delegate for onSendConstruction.
    _entityAspects.onSendConstruction(typeStr,outBitStream);
    RakNet::StringTable::Instance()->EncodeString(typeStr.c_str(),255,outBitStream);
    //Write out whether is owner.
    if(_initatorAddress == systemAddress)
        outBitStream->Write1();
    else
        outBitStream->Write0();

    cout << "Sending construction no play." << endl;
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
    //Okay, the way we're deleting this object is a bit convoluted. Maybe I"m not understand the code,
    //There is no ReplicaDesctuctor like there is a ReplicaConstructor. In the example code, he simply deletes an EXTERN variable, which is a pointer to the Replica.
    //He only has a single Replica for Player, and a Single repliac for Monster. Where we need to have N number of replicas. Another way to do this is to generate an event,
    //and return. The system will detect this event outside and handle it.
    cout << "In ZEntity ReceiveDestruction" << endl;

    if(!_isServer)
    {
        NetClientController::netEntManagerClient.clearEntity(this->GetNetworkID());
    }
    cout << "Out of ZNetEntity ReceiveDestruction" << endl;
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





