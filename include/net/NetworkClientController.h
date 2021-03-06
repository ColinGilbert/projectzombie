#ifndef _NETWORK_CLIENT_STATE_H
#define _NETWORK_CLIENT_STATE_H
#include <Ogre.h>


#include "NetController.h"
#include "EventDelegates.h"
#include "LifeCycleDelegates.h"
#include "Controller.h"

namespace ZGame
{
    namespace Networking
    {
        /** \brief This class handles call back for receive construction calls.
        *
        *This class implements the ReceiveConstructionInterface )see RakNet for call back handling of ReceiveConstruction calls.
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
            /** \brief This is the call back method to handle recieve download complete calls.*/
            ReplicaReturnResult ReceiveDownloadComplete(RakNet::BitStream* inBitStream, SystemAddress senderId, ReplicaManager* caller);
        };
        class NetworkClientController : public NetController, public Controller
        {
        public:
            NetworkClientController();
            virtual ~NetworkClientController();

            bool
                connect();
            bool
                disconnect();
            void 
                shutdown();

            //LifeCycle functions
            virtual bool 
                onInit();
            virtual bool 
                onUpdate(const Ogre::FrameEvent &evt);
            virtual bool 
                onDestroy();
            //void
            //regLfcObs(LifeCycleRegister &lfcReg);
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

            /** \brief Our replica constructor call back object. */
            ReplicaConstructor _replicaConstructor;
            /** \brief Our replica sender call back object. */
            ReplicaSender _sendDownloadComplete;
            /** \brief Our replica receiver call back object. */
            ReplicaReceiver _recieveDownloadComplete;

            bool
                initClient();

            void handlePacket();

            //unsigned char getPacketIdentifier(Packet* p);
            void printPacketId(unsigned char id);
        };
    }
}
#endif
