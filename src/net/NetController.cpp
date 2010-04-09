#include <cstdlib>

#include "net/NetController.h"
#include "net/ZNetEntityTypes.h"


using namespace ZGame::Networking;
using namespace ZGame;

//NetworkEntitiesManager NetController::netEntManager; //the static variable in NetController class.

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
    //We want to auto construct to new participants
    _replicaManager.SetAutoConstructToNewParticipants(true);

    _replicaManager.SetAutoSerializeInScope(true);

    _replicaManager.SetReceiveConstructionCB(getConstructionCB());

    _replicaManager.SetDownloadCompleteCB(getSetDownloadCompleteCB(),getReceiveDownloadCompleteCB());

    //NetworkEntitiesManager* entManager = getNetworkEntitiesManager();
    //entManager->setNetEntitiesVector(_netEntities);
    //entManager->setEntitiesVector(_entities);

    //Add the entity types. We use "static" string table here as an efficient lookup table. 2nd parameter is false means it is a static string.
    //The plan here is to read this from our DB. For now, let's just hard-code the two types: Player and Monster.
    RakNet::StringTable::Instance()->AddString("PLAYER_TYPE",false);
    RakNet::StringTable::Instance()->AddString("MONSTER_TYPE",false);
    /*
    Ogre::StringStream types;
    types << ZGame::PLAYER_TYPE << "TYPE";
    RakNet::StringTable::Instance()->AddString(types.str().c_str(),false);
    types << "";
    types << ZGame::MONSTER_TYPE << "TYPE";
    RakNet::StringTable::Instance()->AddString(types.str().c_str(),false);
    */



    
    //RakNet::StringTable::Instance()->AddString(itoa(ZGame::PLAYER_TYPE,typesBuffer1,10),false);
    //RakNet::StringTable::Instance()->AddString("PLAYER",false);
    //RakNet::StringTable::Instance()->AddString(itoa(ZGame::MONSTER_TYPE,typesBuffer2,10),false);
    //RakNet::StringTable::Instance()->AddString("MONSTER",false);

    cout << "NetController::onInit() finished." << endl;

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

