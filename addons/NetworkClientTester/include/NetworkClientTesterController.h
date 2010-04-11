#ifndef _ZGAME_NETWORK_CLIENT_TEST_CTRL_H
#define _ZGAME_NETWORK_CLIENT_TEST_CTRL_H

#include <memory>
#include <vector>
#include <Ogre.h>

#include <MainController.h>
#include <net/NetClientController.h>


namespace ZGame
{

    namespace Networking
    {
        /** \brief This class handles is for receive construction calls which will create the TestClientReplica */
        class TestClientConstructor : public ReceiveConstructionInterface
        {
        public:
            ReplicaReturnResult ReceiveConstruction(RakNet::BitStream* inBitStream, RakNetTime timestamp, NetworkID networkID,
                NetworkIDObject *existingObject, SystemAddress senderId, ReplicaManager *caller);
        };

        class TestClientSender : public SendDownloadCompleteInterface
        {
        public:
            ReplicaReturnResult SendDownloadComplete(RakNet::BitStream* outBitStream, RakNetTime currentTime, SystemAddress senderId,
                ReplicaManager* caller);
        };

        class TestClientReceiver : public ReceiveDownloadCompleteInterface
        {
        public:
            ReplicaReturnResult ReceiveDownloadComplete(RakNet::BitStream* inBitStream, SystemAddress senderId, ReplicaManager* caller);
        };
    }

    class NetworkClientTesterController : public MainController
    {
    public:
        NetworkClientTesterController();
        virtual
            ~NetworkClientTesterController();
        //implements virtual method for MainController
        bool
            onInit();
        void
            run();
        void
            onDestroy();
    protected:
    private:
        auto_ptr<ZGame::Networking::NetClientController> _netClient;
    };
}

#endif