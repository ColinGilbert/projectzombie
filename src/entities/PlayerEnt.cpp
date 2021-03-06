#include "CommandController.h"
#include "command/CommandList.h"
#include "net/ZNetEntityTypes.h"
#include "entities/PlayerEnt.h"

using namespace std;

using namespace ZGame::Entities;
/**
*\note for now the typeName is gotten from a static function. This will be changed to from database.
*
*/
PlayerEntity::PlayerEntity() : ZEntity("PLAYER_ENTITY","ninja.mesh")
//PlayerEntity::PlayerEntity(ReplicaManager &replica) : ZEntity("PLAYER_ENTITY","PLAYER_MESH"),
//_replicaManager(replica)
{
    //we should get these type information from database.
    //Ogre::StringStream types;
    //types << ZGame::PLAYER_TYPE << "TYPE";
    //_typeName = types.str(); 
}

/**
*This destroys the player entity.
*
*precondition: We assume that the CommandController will handle exceptions thrown by executing the remove node command.
*/
PlayerEntity::~PlayerEntity()
{
}

bool
PlayerEntity::onInitClient()
{
    
    return true;
}

bool
PlayerEntity::onInitServer()
{
    cout << "In PlayerEntity::onInitServer" << endl;
    return true;
}

bool
PlayerEntity::onDestroyClient()
{
    return true;
}

bool
PlayerEntity::onDestroyServer()
{
    cout << "In PlayerEntity::onDestroyServer()" << endl;
    return true;
}

/**
*This function is a call back for on send construction events. This function is coupled (interface) to
*the RakNet system. The RakNet system will call this function for on send construction events. One needs to
*write out the type of the entity. The outBitStream is left there to write custom things to the remote
*systems, using bit streams.
*
*/
bool
PlayerEntity::onSendConstruction(Ogre::String &typeStr, RakNet::BitStream* outBitStream)
{
    typeStr = "PLAYER_TYPE"; //This temporary hard-coded type value. In the future will come from database.
    return true;
}






