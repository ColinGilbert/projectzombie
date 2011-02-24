#include <iostream>
using std::cout;
using std::endl;
#include "command/CmdException.h"
#include "command/CreateEntCmd.h"
#include "CommandController.h"
#include "entities/RenderEntitiesManager.h"
#include "entities/ZEntity.h"
#include "entities/EntitiesManager.h"
#include "delegates/EntityDelegates.h"

using namespace ZGame::COMMAND;
using namespace ZGame;
using COMMAND::CmdException;

const COMMAND_KEY CreateEntCmd::KEY("createentcmd");

CreateEntCmd::CreateEntCmd(const COMMAND_KEY &key, int numOfParams)
    : Command(key)
{
}

CreateEntCmd::~CreateEntCmd(void)
{
}

void CreateEntCmd::setEntitiesManager(Entities::EntitiesManager* entMgr)
{
    _entMgr = entMgr;
}

DelegateMemento
    CreateEntCmd::execute(const Command &cmd)
{
    cout << "In CreateEntCmd::execute" << endl;
    using ZGame::Entities::EntitiesManager;
    using Entities::ZENTITY_VEC;
    using Entities::ZENT_ITER;

    DelegateMemento NULL_MEMENTO;

    const CreateEntCmd* cmdPtr = static_cast<const CreateEntCmd*>(&cmd);
    EntitiesManager* entMgr = getEntitiesManager();
    ZENTITY_VEC* ents = entMgr->getEntities();
    
   
    return NULL_MEMENTO;
}

