/** \file
* \brief Contains networking based controllers and related class.
*
*/

#ifndef _ZGAME_NETCONTROLLER_H
#define _ZGAME_NETCONTROLLER_H

#include <Ogre.h>

#include "StringTable.h" #include "RakNetworkFactory.h" #include "RakPeerInterface.h" //#include "ReplicaMember.h" #include "BitStream.h" #include "MessageIdentifiers.h" #include "ReplicaEnums.h" #include "Replica.h" #include "ReplicaManager.h" #include "NetworkIDManager.h"

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
            /** \brief Default constructor. */
            ReplicaConstructor(bool isServer=true):ReceiveConstructionInterface(),_isServer(isServer){}
            /** \brief This is the call back method to do the actual construction. */
            ReplicaReturnResult ReceiveConstruction(RakNet::BitStream* inBitStream, RakNetTime timestamp, NetworkID networkID,
                NetworkIDObject *existingObject, SystemAddress senderId, ReplicaManager *caller);
        private:
            bool _isServer;
        };

        /** \brief This class handles call back for send download complete calls.
        *
        *This class implements the SendDownloadCompleteInterface (see RakNet) for call back handling of SendDownloadComplete calls.
        */
        class ReplicaSender : public SendDownloadCompleteInterface
        {
        public:
            ReplicaSender(bool isServer=true):SendDownloadCompleteInterface(),_isServer(isServer){}
            /** \brief This is the call back method to handle send download complete calls.*/
            ReplicaReturnResult SendDownloadComplete(RakNet::BitStream* outBitStream, RakNetTime currentTime, SystemAddress senderId,
                ReplicaManager* caller);
        private:
            bool _isServer;
        };

        /** \brief This class handles call back for recieve download complete calls.
        *
        *This class implements the ReceiveDownloadCompleteInterface (see RakNet) for call back handling of ReceiveDownloadComplete calls
        */
        class ReplicaReceiver : public ReceiveDownloadCompleteInterface
        {
        public:
            ReplicaReceiver(bool isServer=true):ReceiveDownloadCompleteInterface(),_isServer(isServer){}
            /** \brief This is the call back method to handle recieve download complete calls.*/
            ReplicaReturnResult ReceiveDownloadComplete(RakNet::BitStream* inBitStream, SystemAddress senderId, ReplicaManager* caller);
        private:
            bool _isServer;
        };

        /** \brief This class is the super class for networking based controllers.
        *
        *This is the super class for network based controller. It encapsulates the usage of ReplicaManager. See RakNet ReplicaManager examples.
        */
        class NetController
        {
        public:
            virtual ~NetController();

            //LifeCycle functions
            virtual bool
                onInit();

            virtual bool
                onUpdate();

            virtual bool
                onUpdate(const Ogre::FrameEvent &evt);
            virtual bool
                onDestroy(); 

        protected:
            /** \brief The RakPeer interface. */
            RakPeerInterface* _rakPeer;
            bool _isServer;
            /** \brief Default constructor. */
            NetController(bool isServer=true);

            unsigned char
                getPackIdentifer(Packet* p)
            {
                if((unsigned char)p->data[0] == ID_TIMESTAMP)                     return (unsigned char)p->data[sizeof(unsigned char)+sizeof(unsigned long)];                 else                     return (unsigned char)p->data[0];
            }

        private:
            

            /** \brief Stores a list of network object pointers, and indices to look them up by. 
            *
            *This class needs to registered with both RakPeer and with each created NetworkIDObject.
            */
            NetworkIDManager _networkIDManager;

            /** \brief Our Replica Manager. */
            ReplicaManager _replicaManager;
            /** \brief Our replica constructor call back object. */
            ReplicaConstructor _replicaConstructor;
            /** \brief Our replica sender call back object. */
            ReplicaSender _sendDownloadComplete;
            /** \brief Our replica receiver call back object. */
            ReplicaReceiver _recieveDownloadComplete;

            
        };
    }
}

#endif