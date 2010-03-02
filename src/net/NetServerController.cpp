#include <iostream>

#include "net/NetServerController.h"

using namespace ZGame;
using namespace ZGame::Networking;

ReplicaReturnResult ServerReplicaConstructor::ReceiveConstruction(RakNet::BitStream* inBitStream, RakNetTime timestamp, NetworkID networkID,
                                                                  NetworkIDObject *existingObject, SystemAddress senderId, ReplicaManager *caller)
{
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
        peer->DeallocatePacket(packet);

        packet = peer->Receive();

    }

    return true;
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
        cout << "A connection is incoming." << endl;
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

