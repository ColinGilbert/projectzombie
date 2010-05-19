#include <iostream>

#include <BitStream.h>

using namespace std;

#include "net/NetServerController.h"
#include "entities/PlayerEnt.h"
#include "DelegatesUtil.h"
#include "net/ZNetEntity.h"
#include "entities/EntitiesManager.h"

using namespace ZGame;
using namespace ZGame::Networking;

//tempalte declares.
//template class NetworkEntitiesManager<SystemAddress>;

NetworkEntitiesManager<SystemAddress> NetServerController::netEntManagerServer; //temporary server-side net entity manager, as a static variable.

/**
*This is server side call-back method for ReceiveConstruction events. 
*
* \note The server side should be careful about what the client wants to construct--for now, the client
*can construction nothing.
*
*/
ReplicaReturnResult ServerReplicaConstructor::ReceiveConstruction(RakNet::BitStream* inBitStream, RakNetTime timestamp, NetworkID networkID,
                                                                  NetworkIDObject *existingObject, SystemAddress senderId, ReplicaManager *caller)
{
    cout << "In ServerReplicaConstructor::ReceiveConstruction." << endl;
    //The client can construction nothing at this point.
    return REPLICA_PROCESSING_DONE;
}

ReplicaReturnResult ServerReplicaSender::SendDownloadComplete(RakNet::BitStream* outBitStream, RakNetTime currentTime, SystemAddress senderId,
                                                              ReplicaManager* caller)
{
    return REPLICA_PROCESSING_DONE;
}

ReplicaReturnResult ServerReplicaReceiver::ReceiveDownloadComplete(RakNet::BitStream* inBitStream, SystemAddress senderId, ReplicaManager* caller)
{
    return REPLICA_PROCESSING_DONE;
}


NetServerController::NetServerController(void)
{
}

NetServerController::~NetServerController(void)
{
}

int 
NetServerController::execute(const Ogre::StringVector &params)
{
    return 0;
}

bool
NetServerController::onInit()
{
    using namespace std;
    //Do not forget to call onInit in base class. We need base class to do some initialization.
    assert(NetController::onInit() && "Base class NetController failed onInit. Cannot continue!");
    if(!initServer())
    {
        cout << "Initializing server failed." << endl;
        return false;
    }
    cout << "Server started." << endl;
    return true;
}

bool
NetServerController::initServer()
{
    const int MAX_CONNECT = 8;
    const int SERVER_PORT = 6666;
    RakPeerInterface* peer = getRakPeerInterface();
    SocketDescriptor socketDescriptor(SERVER_PORT,0);
    if(!peer->Startup(MAX_CONNECT,0,&socketDescriptor,1))
    {
        return false;
    }
    getNetworkIDManager()->SetIsNetworkIDAuthority(true); //set this to true since we are server.
    peer->SetMaximumIncomingConnections(MAX_CONNECT);
    return true;
}

bool
NetServerController::shutdownServer()
{
    using namespace std;
    cout << "Shutting down server!" << endl;
    getRakPeerInterface()->Shutdown(300); 
    return true;
}

bool
NetServerController::onUpdate()
{
    handlePacket();
    return true;
}

bool
NetServerController::onUpdate(const Ogre::FrameEvent &evt)
{
    return true;
}



bool
NetServerController::onDestroy()
{
    shutdownServer();
    return true;
}

bool
NetServerController::handlePacket()
{
    unsigned char packetId;

    RakPeerInterface* peer = getRakPeerInterface();

    Packet* packet = peer->Receive();

    while(packet)
    {

        packetId = getPackIdentifer(packet);
        printPacketId(packetId);
        processPacket(packetId,packet);
        peer->DeallocatePacket(packet);

        packet = peer->Receive();

    }

    return true;
}

void
NetServerController::processPacket(unsigned char id,Packet* packet)
{
    using namespace std;
    switch(id)
    {
        case ID_NEW_INCOMING_CONNECTION:
            processNewConnection(packet);
            break;
        //handle cases for disconnection.
        case ID_DISCONNECTION_NOTIFICATION:
        case ID_CONNECTION_LOST:
            processDisconnection(packet);
            break;
        default:
            break;
    }
}
/**
*This method handles the prcoessing of new connections. It will be responsible for creating a new player type.
*This method will construct the Raknet entity. It will disable server-side specific raknet interface call-backs.
*
*\precondition It is assumed Raknet has been setup.
*\postcondition the composite Entity (ZEntity and ZNetEntity) is created and managed by NetEntityManager.
*
* \note: We may need to refactor to use Interfaces instead of Delegate here.
* \note: We may need to refactor so put all the common creation steps for client and server into a static function. And only to the client/server specific things here. However, let's just keep it simple, for now.
since there isn't much code for this. And we do not expect to duplicate this code everywhere (only in server and client controllers when one creates the composite entities.)
*
*/
void
NetServerController::processNewConnection(Packet* packet)
{
    using namespace Entities;
    using namespace Networking;
    bool isServer = true;
    cout << "In NetServerController::processNewConnection" << endl;
    
    ReplicaManager* replicaManager = getReplicaManager();

    /////////////CREATE COMPOSITE SERVER PLAYER_TYPE///////////////
    boost::shared_ptr<ZNetEntity> netEntSmart;
    boost::shared_ptr<ZEntity> plyEntSmart;
    PlayerEntity* nakedEntPtr = 0;

    //server-side composite entity construction. Since we are on the server, we don't need to pass NetworkID (see below.) true is server.
    //Note: Need to pass in NetworkIDManager for server because GetNetworkID will crash. Unless you call replicaManager->DisableReplicaInterface first.
    constructCompositeZNetEntity(plyEntSmart, netEntSmart, nakedEntPtr, replicaManager, packet->systemAddress, 0, isServer);

    ////////////Replica manager server/client construction.//////////////////////////////
    //Disable server-side specific interfaces.
    replicaManager->DisableReplicaInterfaces(netEntSmart.get(), 
                    REPLICA_RECEIVE_DESTRUCTION | REPLICA_RECEIVE_SCOPE_CHANGE);
    
    //Now the composite entity is considered created, let's send the Construct event. We set broadcast to true to send to everyone.
    replicaManager->Construct(netEntSmart.get(),false,UNASSIGNED_SYSTEM_ADDRESS,true);

    ////////////Entity management.//////////////////////////
    cout << "SystemAddress mapped to the Net Entity: " << packet->systemAddress.ToString() << endl;
    //Let NetEntityManager manage the entities.
    netEntManagerServer.addEntity(packet->systemAddress,netEntSmart,plyEntSmart);
    cout << "New Connection processed." << endl;
}

/**
*This method will process disconnection for the client specified in Packet->systemAddress.
*
*\param Packet. The given packet which contains the systemAddress of the disconnected client.
*
*\precondition It is assumed the the client specified in Packet->systemAddress has ceased communication with the server.
*\postcondition The disconnection of the client is processed. (All association and references of the server to the client has been cleared.)
*/
void
NetServerController::processDisconnection(Packet* packet)
{
    //Ask the Network Entities manager to clear the client with the given SystemAddress.
    netEntManagerServer.clearEntity(packet->systemAddress);
}

void
NetServerController::printPacketId(unsigned char id)
{
    using namespace std;
    switch(id)
    {
    case ID_REMOTE_DISCONNECTION_NOTIFICATION:
        cout << "Another client has disconnected" << endl;
        break;
    case ID_REMOTE_CONNECTION_LOST:
        cout << "Another client has lost connection" << endl;
        break;

    case ID_REMOTE_NEW_INCOMING_CONNECTION:
        cout << "Another client has connected" << endl;

        break;
    case ID_CONNECTION_REQUEST_ACCEPTED:
        cout << "Client request accepted" << endl;
        break;

    case ID_NEW_INCOMING_CONNECTION:
        cout << "A remote system has successfully connected." << endl;
        break;

    case ID_DISCONNECTION_NOTIFICATION:
        cout << "A client has disconnected" << endl;
        break;

    case ID_CONNECTION_LOST:

        cout << "A client has lost connect." << endl;
        break;
    default:
        break;
    }

}




ReceiveConstructionInterface*
NetServerController::getConstructionCB()
{
    return &_replicaConstructor;
}
SendDownloadCompleteInterface*
NetServerController::getSetDownloadCompleteCB()
{
    return &_sendDownloadComplete;
}
ReceiveDownloadCompleteInterface*
NetServerController::getReceiveDownloadCompleteCB()
{
    return &_recieveDownloadComplete;
}
