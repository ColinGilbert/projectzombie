#include <iostream>
#include <csignal>
using namespace std;

#include <boost/thread/thread.hpp> //for thread sleep
#include <boost/date_time/posix_time/posix_time.hpp>

#include "NetworkClientTesterController.h"

using namespace ZGame;

bool STILL_RUNNING_ME = true;

using namespace ZGame::Networking;
ReplicaReturnResult TestClientConstructor::ReceiveConstruction(RakNet::BitStream* inBitStream, RakNetTime timestamp, NetworkID networkID,
                                                               NetworkIDObject *existingObject, SystemAddress senderId, ReplicaManager *caller)
{
    cout << "In TestClientConstructor ReceiveConstruction." << endl;
    return REPLICA_PROCESSING_DONE;
}

ReplicaReturnResult TestClientSender::SendDownloadComplete(RakNet::BitStream* outBitStream, RakNetTime currentTime, SystemAddress senderId,
                                                           ReplicaManager* caller)
{
    cout << "In TestClientSender SendDownload" << endl;
    return REPLICA_PROCESSING_DONE;
}

ReplicaReturnResult TestClientReceiver::ReceiveDownloadComplete(RakNet::BitStream* inBitStream, SystemAddress senderId, ReplicaManager* caller)
{
    cout << "In TestClientSender ReceiveDownloadComplete." << endl;
    return REPLICA_PROCESSING_DONE;
}




void stop_running_signal_handler_me(int signum)
{
    STILL_RUNNING_ME = false;
}

NetworkClientTesterController::NetworkClientTesterController() : MainController()
{
}

NetworkClientTesterController::~NetworkClientTesterController()
{

}

bool 
NetworkClientTesterController::onInit()
{

    cout << "In Net client tester onInit." << endl;

    signal(SIGINT,stop_running_signal_handler_me);


    _netClient.reset(new ZGame::Networking::NetClientController(new TestClientConstructor(), new TestClientSender(), 
        new TestClientReceiver()));
    _netClient->onInit();
    _netClient->connect();
    return true;
}

void
NetworkClientTesterController::run()
{
    cout << "In Net client tester run." << endl;
    while(STILL_RUNNING_ME)
    {
        _netClient->onUpdate();
        //Let's sleep for awhile. Before we don't have other processing going on.
        using namespace boost::posix_time;
        boost::thread::yield();
    }
}

void
NetworkClientTesterController::onDestroy()
{
    cout << "In net client test onDestroy." << endl;
    _netClient->onDestroy();
}