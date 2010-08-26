#include <iostream>

#include <BitStream.h>

using namespace std;

#include "net/NetClientController.h"
#include "LifeCycleRegister.h"
#include "MessageIdentifiers.h"

#include "entities/PlayerEnt.h"
#include "entities/EntitiesManager.h"

#include "CommandController.h"
#include "CommandDelegates.h"

#include "net/ZNetEntity.h"

#include "command/Command.h"
#include "command/CommandList.h"

namespace ZGame 
{
    namespace Networking
    {
        template class NetworkEntitiesManager<NetworkID>;
        NetworkEntitiesManager<NetworkID> NetClientController::netEntManagerClient; //temporary client-side network entity manager--static variable.
        
        
        /**
        *This is the client-side call-back method for ReceiveConstruction events.
        *
        */
        ReplicaReturnResult ReplicaConstructor::ReceiveConstruction(RakNet::BitStream* inBitStream, RakNetTime timestamp, NetworkID networkID,
            NetworkIDObject *existingObject, SystemAddress senderId, ReplicaManager *caller)
        {
            
            using namespace Entities;
            char output[255];
            bool isServer = false;
            
            cout << "In Client-side ReplicaConstructor::ReceiveConstruction" << endl;
            //Decode type str from lookup table.
            RakNet::StringTable::Instance()->DecodeString(output,255,inBitStream);
            //Decode system address of creator.
            //Delegate creation based on type. (Types are hardcoded right now for testing.)
            if(strcmp(output,"PLAYER_TYPE")==0)
            {
                //Read whether we are the initator of this player type. This means we initiated this player object.
                bool initator = false;
                initator = inBitStream->ReadBit();
                cout << "Creating Player Type on client. Initator? : " << initator << endl;
                
                ///////////CREATE COMPOSITE CLIENT PLAYER_TYPE/////////////////
                boost::shared_ptr<ZNetEntity> netEntSmart;
                boost::shared_ptr<ZEntity> plyEntSmart;
                PlayerEntity* nakedEntPtr = 0; //We are creating a PlayerEntity here. See templated constructCompositeZNetEntity function.

                //create the composite ZNetEntity. UNASSIGNED_SYSTEM_ADDRESS because we are not on the server. Server will determine who is the initator of the object. False means not server.
                constructCompositeZNetEntity(plyEntSmart, netEntSmart, nakedEntPtr, caller, UNASSIGNED_SYSTEM_ADDRESS, &networkID, isServer);         
                
                /////////////Replica manager server/client construction.//////////////////
                //disable the client-specific call-back interfaces.
                caller->DisableReplicaInterfaces(netEntSmart.get(), REPLICA_SEND_CONSTRUCTION | REPLICA_SEND_DESTRUCTION | REPLICA_SEND_SCOPE_CHANGE);
                caller->Construct(netEntSmart.get(), false, UNASSIGNED_SYSTEM_ADDRESS, false);
               
                 cout << "NetID: systemAddress: " << networkID.systemAddress.ToString() 
                    << " localSystemAddress: " << networkID.localSystemAddress << endl;
                
                ////////////////Entity management.///////////////////////           
                NetClientController::netEntManagerClient.addEntity(networkID, netEntSmart, plyEntSmart);

                cout << "Finished adding entity to NetEntityManager." << endl;
                cout << "PLAYER_TYPE created on client-side." << endl;

            }
            else if(strcmp(output,"MONSTER_TYPE")==0)
            {
                //MONSTER_TYPE
            }
            else
            {
                //UNKOWN STRING: logical bug fail.
                assert(0);
            }
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

        NetClientController::NetClientController() : NetController(false), //Not a server.
            _isConnected(false),
            _CONNECT_CMD("connect"),
            _DISCONNECT_CMD("disconnect"), _replicaConstructor(new ReplicaConstructor()),
            _sendDownloadComplete(new ReplicaSender()), _receiveDownloadComplete(new ReplicaReceiver())
        {
            setupCommands();
        }

        NetClientController::NetClientController(ReceiveConstructionInterface* recCons, SendDownloadCompleteInterface* sendDownComplete,
            ReceiveDownloadCompleteInterface* receiveDownComplete) : NetController(false), //not a server
            _isConnected(false),
            _CONNECT_CMD("connect"),
            _DISCONNECT_CMD("disconnect"),_replicaConstructor(recCons),
            _sendDownloadComplete(sendDownComplete), _receiveDownloadComplete(receiveDownComplete)
        {
            setupCommands();
        }

        NetClientController::~NetClientController()
        {
            //warning naked pointers here.
            delete _replicaConstructor;
            delete _sendDownloadComplete;
            delete _receiveDownloadComplete;

        }

        /**
        *This method will register commands from this class, to be made availible for the CommandControl system. 
        *
        *\precondition The CommandController singleton MUST exist!
        *\postcondition The commands for this class is availible in CommandController, to be used by other systems.
        *
        */
        void
            NetClientController::setupCommands()
        {
            using namespace COMMAND;
            cout << "In NetClientController::setupCommands." << endl;
            cout << "Registering Commands to CommandController." << endl;
            //register commands for connect and disconnect.

            
            ZGame::CommandController* cmdCtrl = ZGame::CommandController::getSingletonPtr();
            COMMAND::ConsoleCommand cmd;
            cmd.bind(this,&ZGame::Networking::NetClientController::executeCmd);

            cmdCtrl->addCommand(CommandList::CLIENT_CONNECT,cmd.GetMemento());
            cmdCtrl->addCommand(CommandList::CLIENT_DISCONNECT,cmd.GetMemento());

            //StringCommand connectDiscontCmd(_CONNECT_CMD);
            
            
            /*
            connectDiscontCmd.setCommandMemento(cmd.GetMemento());
            cmdCtrl->addCommand(connectDiscontCmd);
            connectDiscontCmd.setKey(_DISCONNECT_CMD);
            cmdCtrl->addCommand(connectDiscontCmd);
            */
            /*
            cmdCtrl->addCommand(_CONNECT_CMD,cmd);
            cmdCtrl->addCommand(_DISCONNECT_CMD,cmd);
            */
        }

        bool
            NetClientController::executeCmd(const Ogre::StringVector &params)
        {
            if(_CONNECT_CMD.compare(params[0]) == 0)
            {
                connect();
            }
            else if(_DISCONNECT_CMD.compare(params[0]) == 0)
            {
                disconnect();
            }
            return 0;
        }

        void NetClientController::printPacketId(unsigned char id)
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
        bool NetClientController::onInit()
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

        bool NetClientController::onUpdate()
        {
            handlePacket();
            return true;
        }
        bool NetClientController::onUpdate(const Ogre::FrameEvent &evt)
        {
            handlePacket();
            return true;
        }

        bool NetClientController::onDestroy()
        {
            shutdown();
            return true;
        }

        bool NetClientController::initClient()
        {
            getRakPeerInterface()->Startup(1,30,&SocketDescriptor(0,0),1);
            //set the network id authority to false here because we are not Server.
            getNetworkIDManager()->SetIsNetworkIDAuthority(false);
            return true;
        }

        void NetClientController::shutdown()
        {
            handlePacket();
            //Proceed with disconnection if isConnect
            disconnect();
            //Handle any remaining packets.
            handlePacket();
            _isConnected = false;
            removeAllEntities();
        }


        bool NetClientController::connect()
        {
            if(_isConnected)
                return true;
            cout << "Trying to connect. " << endl;
            _isConnected = getRakPeerInterface()->Connect("127.0.0.1",6666,0,0);
            if(!_isConnected)
                cout << "Connect call failed!" << endl;
            return _isConnected;
        }

        bool NetClientController::disconnect()
        {
            if(!_isConnected)
                return true;
            cout << "Disconnecting. " << endl;
            //Note: By calling CloseConnection, we recieve recieve a 
            //ID_DISCONNECTION_NOTIFICATION packet. It is in the packet handler we
            //set _idsConnected to false. Because, it is at the point of that packet 
            //when we disconnect from the system--logically disconnected.
            getRakPeerInterface()->CloseConnection(_serverSysAddress,true,0);
            //_isConnected = false;
            return true;
        }

        void NetClientController::handlePacket()
        {
            unsigned char packetId;
            RakPeerInterface* _rakPeer = getRakPeerInterface();
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
                    processPacket(packetId,packet);
                    _rakPeer->DeallocatePacket(packet);
                }

                packet=_rakPeer->Receive();
            }
        }

        void 
            NetClientController::processPacket(unsigned char id, Packet* packet)
        {
            cout << "In NetClientController::processPacket" << endl;
            using namespace std;
            switch(id)
            {            
            case ID_CONNECTION_LOST:
            case ID_DISCONNECTION_NOTIFICATION:
                if(!_isConnected) //Note: We should NEVER RECIEVE this packet if we were connected.
                    break;
                _isConnected = false;
                cout << "processing ID_CONNECTION_LOST & DISCONNECTION_NOTIFICATION." << endl;
                //process when connection is lost or disconnected from server.
                //Remove all replicas.
                removeAllEntities();
                //disconnect(); //disconnect rakpeer.
                break;
            default:
                break;
            }
        }
        /**
        *This method will remove invoke the client-side NetEntitiesManager to clear all managed entities.
        *
        *\precondition: It is assumed that we are in a valid state. That is to say, the logic for handling disconnections should be correct.
        *\postcondition: all entities are safely and cleanly removed from the system. There should not be any thread related state invalidation issues. Meaning,
        *there shouldn't be a case where someone is still tryint to read/write to an delete entitiy. (We need to worry about such things due to threading.)
        */
        void
            NetClientController::removeAllEntities()
        {
            cout << "In NetClientController::removeAllEntities" << endl;
            NetClientController::netEntManagerClient.clearAll();
            cout << "Done NetClientController::removeAllEntities" << endl;
        }


        ReceiveConstructionInterface*
            NetClientController::getConstructionCB() { return _replicaConstructor;}
        SendDownloadCompleteInterface*
            NetClientController::getSetDownloadCompleteCB() { return _sendDownloadComplete; }
        ReceiveDownloadCompleteInterface*
            NetClientController::getReceiveDownloadCompleteCB() { return _receiveDownloadComplete; } 
    }



}
