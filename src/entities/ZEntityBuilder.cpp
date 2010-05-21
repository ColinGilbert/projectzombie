#include <iostream>
using std::cout;
using std::endl;
#include "entities/ZEntityBuilder.h"
#include "CommandController.h"
#include "command/CommandList.h"
#include "command/CreateEntCmd.h"
#include "entities/ZEntity.h"

using namespace ZGame::Entities;

bool
EntitiesBuilder::build(ZEntity* zEnt, const EntityUpdateEvent &readEvent)
{
    assert(zEnt != 0 && "ZEntity is null!");
    //We want to build the ZEntity. We sent a create character command. 
    using std::cout;
    using std::endl;
    using COMMAND::CreateEntCmd;

    CreateEntCmd createEnt;
    createEnt.setResource(*zEnt->getResource());
    createEnt.setUpdateEvent(readEvent);
    
    ZGame::CommandController::getSingleton().executeCmd(createEnt);

    return true;
}

bool
EntitiesBuilder::unbuild(ZEntity* zEnt)
{
    
    assert(zEnt != 0 && "ZEntity is null!");
    cout << "In PlayerENtity::onDestroyClient()" << endl;
    COMMAND::StringCommand nodeRemove(CommandList::NODEREMOVE);
    //nodeRemove.push_back(getEntityName()+getMeshName()+"NODE");
    //CommandController::getSingleton().executeCmd(nodeRemove);

    return true;
}