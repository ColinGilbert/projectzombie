#include "entities/NetworkEntitiesManager.h"
#include "net/ZNetEntity.h"
#include "ZEntity.h"
#include "entities/PlayerEnt.h"



using namespace ZGame::Networking;
using namespace ZGame::Entities;

template<typename T>
NetworkEntitiesManager<T>::NetworkEntitiesManager()
{
}

template<typename T>
NetworkEntitiesManager<T>::~NetworkEntitiesManager()
{
}

template<typename T>
void 
NetworkEntitiesManager<T>::addEntity(T key, boost::shared_ptr<ZNetEntity> &znetEntSmart,
                                  boost::shared_ptr<ZEntity> &zEntSmart)
{
    ZENTITY_MAP::iterator curZEntIter;
    curZEntIter = _entities.find(key);
    assert(curZEntIter == _entities.end() && "Logical Assert Failed: Unique key violation failed in Net Entitiy Manager."); //assert that we are in fact inserting a new NetworkID key into the system.
    _entities[key] = zEntSmart;

    NET_ENTITY_MAP::iterator curNetEntIter;
    curNetEntIter = _netEntities.find(key);
    assert(curNetEntIter == _netEntities.end() && "Logical Assert Failed: Unique key violation failed in Net Entitiy Manager.");
    _netEntities[key] = znetEntSmart;

    //load the actual thing.

}

/**
*This method will clear the composite Network Entities.
*
*\precondition It is assumed the clear process will be deterministic. Meaning, when we clear it, nothing else will try to interact in a undetermined and unsafe manner. (Because we are using delegates.)
*\postcondition The composite Network Entity specified with key will be erased from the map.
*/
template<typename T>
void
NetworkEntitiesManager<T>::clearEntity(T key)
{
    _netEntities.erase(key);
    _entities.erase(key);

}

/**
*
*\precondition: It is asssumed that we are calling at the logical point in the program where it is correct to clear all entities. IE. disconnecting from the server; and 
*that clearing will be deterministic. By deterministic we mean that no one will try to interact with these objects after they have been cleared.
*\postcondition: The two composite maps have been cleared. Do not expect to interact with the entities any longer.
*
*\note there should not be any thread unsafe behavior when calling this function. That is, nothing should be reading/writing to these delete buffers afterwards.
*
*/
template<typename T>
void
NetworkEntitiesManager<T>::clearAll()
{
    //clear the netEntities. 
    _netEntities.clear();
    _entities.clear();
}