#include <cstdlib>

#include "net/NetController.h"
#include "ZNetEntityTypes.h"


using namespace ZGame::Networking;
using namespace ZGame;

NetController::NetController(bool isServer) : _isServer(isServer),
_rakPeer(RakNetworkFactory::GetRakPeerInterface())
{

}

NetController::~NetController()
{
    RakNetworkFactory::DestroyRakPeerInterface(_rakPeer);
}

bool NetController::onInit()
{
    using namespace std;
    //Must attach ReplicaManager in order for it to work. It is a RakNet plugin.
    _rakPeer->AttachPlugin(&_replicaManager);

    _rakPeer->SetNetworkIDManager(&_networkIDManager);

    //We want to auto have new connections to participate in whatever it is that replicaManager is supposed to be doing for us.
    _replicaManager.SetAutoParticipateNewConnections(true);

    _replicaManager.SetAutoSerializeInScope(true);

    _replicaManager.SetReceiveConstructionCB(getConstructionCB());

    _replicaManager.SetDownloadCompleteCB(getSetDownloadCompleteCB(),getReceiveDownloadCompleteCB());

    //Add the entity types. We use "static" string table here as an efficient lookup table. 2nd parameter is false means it is a static string.
    char typesBuffer1[25]; //Yes, a naked character array. We KNOW (and assume) that the total types of znet entities will be less than 256. Thus we should not over run here.
    char typesBuffer2[25];
    RakNet::StringTable::Instance()->AddString(itoa(ZGame::PLAYER_TYPE,typesBuffer1,10),false);
    //RakNet::StringTable::Instance()->AddString("PLAYER",false);
    RakNet::StringTable::Instance()->AddString(itoa(ZGame::MONSTER_TYPE,typesBuffer2,10),false);
    //RakNet::StringTable::Instance()->AddString("MONSTER",false);

    return true; 

}

bool NetController::onUpdate()
{
    return true;
}

bool NetController::onUpdate(const Ogre::FrameEvent &evt)
{
    return true;
}

bool NetController::onDestroy()
{
    return true;
}

