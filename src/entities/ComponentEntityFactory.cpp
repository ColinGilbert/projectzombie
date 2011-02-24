#include "entities/ComponentEntityFactory.h"
#include "entities/EntitiesManager.h"
#include "entities/RenderEntitiesManager.h"
#include "entities/ZEntity.h"
#include "entities/RenderEntityComp.h"

using namespace ZGame::Entities;

ComponentEntityFactory::ComponentEntityFactory()
{
}

ComponentEntityFactory::~ComponentEntityFactory()
{
}

/**
*
* \note This is just a prototype method for creating stuff.
**/
void
    ComponentEntityFactory::create(const Ogre::String& typeName, 
    std::vector<Ogre::Any> &componentAnys, size_t numsToCreate)
{
    if(typeName == "ZENTITIES")
    {
        _createZEntities(componentAnys, numsToCreate);
    }
    else
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Invalid component entity type",
        "ComponentEntityFactory::createZEntity");
}

void
    ComponentEntityFactory::_createZEntities(std::vector<Ogre::Any> &componentAnys, size_t nums)
{
    using Ogre::any_cast;
    using Ogre::Any;
    if(componentAnys.size() != 2)
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, "Number of component manager is not right.",
        "ComponentEntityFactory::_createZEntities");
    EntitiesManager* entMgr = any_cast<EntitiesManager*>(componentAnys[0]);
    RenderEntitiesManager* rdrEntMgr = any_cast<RenderEntitiesManager*>(componentAnys[1]);
    for(size_t i = 0; i < nums; ++i)
    {
        //The ZENTITIES component-based entity is dependent on ZEntity component, and RenderEntities component.
        //We assume the order is in the order stated above.
        ZEntity* zent = entMgr->createZEntity();
        RenderEntityComp* rdrComp = rdrEntMgr->createComponent(zent->getResource());
        _makeConnections(rdrComp, zent, "OnReadInput", "OnWriteOutput");
        _makeConnections(rdrComp, zent, "OnEventInput", "OnEventOutput");
    }

    entMgr->setPoolSize(nums);
}

void
    ComponentEntityFactory::_makeConnections(Component* inComp, Component* outComp,
    const Ogre::String& inPort, const Ogre::String& outPort)
{
    fastdelegate::DelegateMemento input = inComp->getInputDM(inPort);
    outComp->addOutputDM(outPort, input);
}