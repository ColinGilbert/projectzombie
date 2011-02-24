#include "entities/ZEntityBuilder.h"
#include "entities/ZEntity.h"
#include "delegates/EntityDelegates.h"
#include "entities/EntitiesManager.h"
using namespace ZGame::Entities;

/**
* This method will build entities. There should be a poll of entities of the entity in question. If the requested build amount overflows more than N entities in a pool,
*then expect wrap around behavior. However, if N is greater than the total pool size, then expect undefine behavior.
**/
bool 
    EntitiesBuilder::build(const std::vector<Ogre::Vector3> &positions, 
    EntitiesManager* entMgr)
{
    using namespace Ogre;
    
    
    if(!entMgr)
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, "NULL pointer for Entities Components Manager."
        "EntitiesBuilder::build", "Line: 23");

    int poolNum = positions.size();
    ZENTITY_VEC::iterator begin; 
    ZENTITY_VEC::iterator end;
    entMgr->getFromPool(begin, end, poolNum);
    size_t i = 0;
    for(auto iter = begin; iter != end; ++iter)
    {
        Ogre::Quaternion quat;
        (*iter)->onEvent(ONBUILD);
        (*iter)->onRead(positions[i++] - Ogre::Vector3(0.0f, 0.5f, 0.0f), quat);
    }

    Ogre::LogManager::getSingleton().getLog("App.log")->logMessage("Building ZEntities");
    return true;
}
