#pragma once
#include "net/NetController.h"
#include "EventDelegates.h"
#include "LifeCycleDelegates.h"
#include "Controller.h"
#include "entities/NetworkEntitiesManager.h"
#include "ZInitPacket.h"

namespace ZGame
{
    namespace Networking
    {
        /** \brief This class handles call back for receive construction calls.
        *
        *This class implements the ReceiveConstructionInterface (see RakNet) for call back handling of ReceiveConstruction calls.
        *
        */
        class ReplicaConstructor : public ReceiveConstructionInterface
        {
        public:
            /** \brief This is the call back method to do the actual construction. */
            ReplicaReturnResult ReceiveConstruction(RakNet::BitStream* inBitStream, RakNetTime timestamp, NetworkID networkID,
                NetworkIDObject *existingObject, SystemAddress senderId, ReplicaManager *caller);
        };

        /** \brief This class handles call back for send download complete calls.
        *
        *This class implements the SendDownloadCompleteInterface (see RakNet) for call back handling of SendDownloadComplete calls.
        */
        class ReplicaSender : public SendDownloadCompleteInterface
        {
        public:
            /** \brief This is the call back method to handle send download complete calls.*/
            ReplicaReturnResult SendDownloadComplete(RakNet::BitStream* outBitStream, RakNetTime currentTime, SystemAddress senderId,
                ReplicaManager* caller);
        };

        /** \brief This class handles call back for recieve download complete calls.
        *
        *This class implements the ReceiveDownloadCompleteInterface (see RakNet) for call back handling of ReceiveDownloadComplete calls
        */
        class ReplicaReceiver : public ReceiveDownloadCompleteInterface
        {
        public:
            /** \brief This is the call back method to handle recieve download complete calls.*/
            ReplicaReturnResult ReceiveDownloadComplete(RakNet::BitStream* inBitStream, SystemAddress senderId, ReplicaManager* caller);
        };
        /** \brief This is the controller class which handles networking on the client side.
        *
        *This class subclasses NetController and is the controller which handles aspects of networking on the client side. Since this class subclass NetController, it is a NetController. The class will 
        *react and respond appropriately for each element in the LifeCycle (see design documentation regarding life-cycles.) That is, one should expect this
        *class to be in a correct state at all times (incorrect state being the system in an invalid state.) 
        
        *This class also implements commands within the Command system.
        *That is, it makes it'self available to the command system (the commands are clear interfaces to the command system.) These two commands are:
        *"connect" and "disconnect".
        */
        class NetClientController : public NetController
        {
        public:
            static NetworkEntitiesManager<NetworkID> netEntManagerClient;
            /** \brief Default constructor. */
            NetClientController();
            NetClientController(ReceiveConstructionInterface* recCons, SendDownloadCompleteInterface* sendDownComplete,
                ReceiveDownloadCompleteInterface* receiveDownComplete);
            virtual ~NetClientController();

            

            bool
                executeCmd(const Ogre::StringVector &params);

            //LifeCycle functions
            virtual bool 
                onInit(ZGame::ZInitPacket packet);
            virtual bool 
                onUpdate(const Ogre::FrameEvent &evt);
            virtual bool
                onUpdate();
            virtual bool 
                onDestroy();
              /** \brief Method to call to connect. */
            bool
                connect();
            /** \brief Method to call to disconnect. */
            bool
                disconnect();

        protected:
            virtual ReceiveConstructionInterface*
                getConstructionCB();
            virtual SendDownloadCompleteInterface*
                getSetDownloadCompleteCB();
            virtual ReceiveDownloadCompleteInterface*
                getReceiveDownloadCompleteCB();
        private:
            bool _isConnected;
            SystemAddress _serverSysAddress;

            //constant command strings
            const Ogre::String _CONNECT_CMD;
            const Ogre::String _DISCONNECT_CMD;


            /** \brief Our replica constructor call back object. */
            ReceiveConstructionInterface* _replicaConstructor;
            /** \brief Our replica sender call back object. */
            SendDownloadCompleteInterface* _sendDownloadComplete;
            /** \brief Our replica receiver call back object. */
            ReceiveDownloadCompleteInterface* _receiveDownloadComplete;


            bool
                initClient();
            void 
                handlePacket();

            //unsigned char getPacketIdentifier(Packet* p);
            void printPacketId(unsigned char id);

            /** \brief Method to process a packet, used in handle packet.*/
            void processPacket(unsigned char id, Packet* packet);


          
            /** \brief Method to call to shutdown. */
            void 
                shutdown();
            /** \brief Method removes all current entities on the client-side.*/
            void
                removeAllEntities();
            /** \brief This method will setup connect and disconnect commands. */
            void
                setupCommands();
        };
    }
}
