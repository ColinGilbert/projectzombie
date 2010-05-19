#include "entities/ZEntityBuilder.h"
#include "CommandController.h"
#include "command/CommandList.h"

using namespace ZGame::Entities;

bool
EntitiesBuilder::build(ZEntity* zEnt, const EntityUpateEvent &readEvent)
{
    assert(zEnt != 0 && "ZEntity is null!");
    //We want to build the ZEntity. This means we need to use message to call the various systems to inform them about this.
    //Build the character creation command.
    using std::cout;
    using std::endl;
    using COMMAND::StringCommand;
    cout << "In PlayerEntity::onInitClient" << endl;
    //Let's create a character.
    StringCommand charCreate(CommandList::CHARCREATE);
    //push the parameters.
    charCreate.push_back(getMeshName());
    charCreate.push_back(getEntityName()+getMeshName()+"NODE");
    CommandController::getSingleton().executeCmd(charCreate);

    return true;
}

bool
EntitiesBuilder::unbuild(ZEntity* zEnt)
{
    assert(zEnt != 0 && "ZEntity is null!");
    cout << "In PlayerENtity::onDestroyClient()" << endl;
    COMMAND::StringCommand nodeRemove(CommandList::NODEREMOVE);
    nodeRemove.push_back(getEntityName()+getMeshName()+"NODE");
    CommandController::getSingleton().executeCmd(nodeRemove);

    return true;
}