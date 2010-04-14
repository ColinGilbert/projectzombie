#include "CommandController.h"
#include "CommandList.h"
#include "net\ZNetEntityTypes.h"
#include "entities\PlayerEnt.h"

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
    cout << "In PlayerEntity::onInitClient" << endl;
    COMMAND::CommandList* cmdList = CommandController::getSingleton().getCommandList();
    //Let's create a character.
    Ogre::StringVector cmd; 
    //const string commands are found in CommandController.
    cmd.push_back(cmdList->CHARCREATE);
    cmd.push_back(getMeshName());
    cmd.push_back(getEntityName()+getMeshName()+"NODE"); //the node parameter.
    CommandController::getSingleton().executeCmd(cmd);
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
    COMMAND::CommandList* cmdList = CommandController::getSingleton().getCommandList();
    cout << "In PlayerENtity::onDestroyClient()" << endl;
    Ogre::StringVector cmd;
    cmd.push_back(cmdList->NODEREMOVE);
    cmd.push_back(getEntityName()+getMeshName()+"NODE");
    CommandController::getSingleton().executeCmd(cmd);
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
/**
*This function is called for on read events--meaning, that 
*
*/
bool
PlayerEntity::onRead(Ogre::Vector3 &pos, Ogre::Quaternion &orient)
{
    return true;
}

/**
*This function is called for on write events--meaning,
*
*/
bool
PlayerEntity::onWrite(Ogre::Vector3 &pos, Ogre::Quaternion &orient)
{
    return true;
}





