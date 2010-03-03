#ifndef _ZGAME_NETSERVER_CONTROLLER_H
#define _ZGAME_NETSERVER_CONTROLLER_H

#include <Ogre.h>

#include "net/NetController.h"
#include "Controller.h"

namespace ZGame
{
    namespace Networking
    {
        /** \brief This class handles call back for receive construction calls.
        *
        *This class implements the ReceiveConstructionInterface (see RakNet) for call back handling of ReceiveConstruction calls.
        *
        */
        class ServerReplicaConstructor : public ReceiveConstructionInterface
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
        class ServerReplicaSender : public SendDownloadCompleteInterface
        {
            /** \brief This is the call back method to handle send download complete calls.*/
            ReplicaReturnResult SendDownloadComplete(RakNet::BitStream* outBitStream, RakNetTime currentTime, SystemAddress senderId,
                ReplicaManager* caller);
        };
        /** \brief This class handles call back for recieve download complete calls.
        *
        *This class implements the ReceiveDownloadCompleteInterface (see RakNet) for call back handling of ReceiveDownloadComplete calls
        */
        class ServerReplicaReceiver : public ReceiveDownloadCompleteInterface
        {
            /** \brief This is the call back method to handle recieve download complete calls.*/
            ReplicaReturnResult ReceiveDownloadComplete(RakNet::BitStream* inBitStream, SystemAddress senderId, ReplicaManager* caller);
        };

        class NetServerController :
            public NetController
        {
        public:
            NetServerController(void);
            virtual ~NetServerController(void);

            virtual int
                execute(const ZGame::Command &cmd) {return 0;}
            virtual int
                execute(const Ogre::StringVector &params);

            virtual bool
                onInit();

            virtual bool 
                onUpdate();

            virtual bool
                onUpdate(const Ogre::FrameEvent &evt);

            virtual bool
                onDestroy();

        protected:
            virtual ReceiveConstructionInterface*
                getConstructionCB();
            virtual SendDownloadCompleteInterface*
                getSetDownloadCompleteCB();
            virtual ReceiveDownloadCompleteInterface*
                getReceiveDownloadCompleteCB();
        private:

            ServerReplicaConstructor _replicaConstructor;
            ServerReplicaSender _sendDownloadComplete;
            ServerReplicaReceiver _recieveDownloadComplete;

            bool
                initServer();
            bool
                shutdownServer();
            bool
                handlePacket();

            void
                printPacketId(unsigned char id);

        };
    }
}
#endif