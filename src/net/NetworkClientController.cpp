#include <iostream>
using std::cout;
using std::endl;

#include "net/NetworkClientController.h"
#include "LifeCycleRegister.h"
#include "MessageIdentifiers.h"



namespace ZGame
{
    namespace Networking
    {
        ReplicaReturnResult ReplicaConstructor::ReceiveConstruction(RakNet::BitStream* inBitStream, RakNetTime timestamp, NetworkID networkID,
            NetworkIDObject *existingObject, SystemAddress senderId, ReplicaManager *caller)
        {
            return REPLICA_PROCESSING_DONE;
        }

        ReplicaReturnResult ReplicaReceiver::ReceiveDownloadComplete(RakNet::BitStream* inBitStream, SystemAddress senderId, ReplicaManager* caller)
        {
            return REPLICA_PROCESSING_DONE;
        }


        ReplicaReturnResult ReplicaSender::SendDownloadComplete(RakNet::BitStream* outBitStream, RakNetTime currentTime, SystemAddress senderId,
            ReplicaManager* caller)
        {
            return REPLICA_PROCESSING_DONE;
        }

        NetworkClientController::NetworkClientController() : NetController(false), 
            _isConnected(false)
        {
        }

        NetworkClientController::~NetworkClientController()
        {
        }

        void NetworkClientController::printPacketId(unsigned char id)
        {
            using namespace std;
            switch(id)
            {
            case ID_CONNECTION_REQUEST_ACCEPTED:
                cout << "We have connected!" << endl;
                break;
            case ID_CONNECTION_ATTEMPT_FAILED:
                cout << "Our connection attempt has failed!" << endl;
                break;
            case ID_ALREADY_CONNECTED:
                cout << "We are connected to the server." << endl;
                break;
            case ID_NO_FREE_INCOMING_CONNECTIONS:
                cout << "The system we are attempting to connect does not have any more availible connections. Sorry." << endl;
                break;
            case ID_DISCONNECTION_NOTIFICATION:
                cout << "The server has disconnected." << endl;
                break;
            case ID_CONNECTION_LOST:
                cout << "We lost our connection to the server." << endl;
                break;
            default:
                break;
            }

        }
        //Lifecycle methods
        bool NetworkClientController::onInit()
        {
            //DO not foget to call onInit in base class. We need base class to do things for us.
            assert(NetController::onInit() && "Base class NetController failed onInit. Cannot continue!");

            if(!initClient())
            {
                cout << "initializing network client failed!." << endl;
                return false;
            }
            return true;
        }

        bool NetworkClientController::onUpdate(const Ogre::FrameEvent &evt)
        {
            handlePacket();
            return true;
        }

        bool NetworkClientController::onDestroy()
        {
            return true;
        }

        bool NetworkClientController::initClient()
        {
            _rakPeer->Startup(1,0,&SocketDescriptor(0,0),1);
            return true;
        }

        void NetworkClientController::shutdown()
        {
        }


        bool NetworkClientController::connect()
        {
            if(_isConnected)
                return true;
            _isConnected = true;
            cout << "Trying to connect. " << endl;
            _rakPeer->Connect("127.0.0.1",6666,0,0);
            return true;
        }

        bool NetworkClientController::disconnect()
        {
            if(!_isConnected)
                return true;
            
	    cout << "Disconnecting. " << endl;
            _rakPeer->CloseConnection(_serverSysAddress,true,0);
            _isConnected = false;
            return true;
        }

        void NetworkClientController::handlePacket()
        {
            unsigned char packetId;
            Packet *packet = _rakPeer->Receive();

            while(packet)
            {
                if(packet)
                {
                    packetId = getPackIdentifer(packet);
                    if(packetId == ID_CONNECTION_REQUEST_ACCEPTED)
                    {
                        _serverSysAddress = packet->systemAddress;
                    }
                    printPacketId(packetId);
                    _rakPeer->DeallocatePacket(packet);
                }

                packet=_rakPeer->Receive();
            }
        }

        ReceiveConstructionInterface*
            NetworkClientController::getConstructionCB() { return &_replicaConstructor;}
        SendDownloadCompleteInterface*
            NetworkClientController::getSetDownloadCompleteCB() { return &_sendDownloadComplete; }
        ReceiveDownloadCompleteInterface*
            NetworkClientController::getReceiveDownloadCompleteCB() { return &_recieveDownloadComplete; } 


    }



}
