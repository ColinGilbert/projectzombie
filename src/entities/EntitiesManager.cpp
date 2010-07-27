#include <iostream>

using std::ostringstream;
using std::endl;

#include "entities/EntitiesManager.h"
#include "entities/ZEntity.h"
#include "entities/ZEntityBuilder.h"
#include "CommandController.h"
#include "command/CreateEntCmd.h"
#include "delegates/EntityDelegates.h"

using namespace ZGame::Entities;

int EntitiesManager::_KEY = 0;

EntitiesManager::EntitiesManager()
{
    using COMMAND::CreateEntCmd;
    GetEntitiesManager dlg;
    dlg.bind(this, &EntitiesManager::getManager);
    CreateEntCmd* cmd = new CreateEntCmd();
    cmd->setEntitiesManager(this);
    shared_ptr<Command> createEntCmd(cmd);
    createEntCmd->setCommandMemento(dlg.GetMemento());
    CommandController::getSingleton().addCommand(createEntCmd);
}

EntitiesManager::~EntitiesManager()
{
    //Remove the entities.
    for(ZENT_ITER it=_zEntsVec.begin(); it!=_zEntsVec.end(); ++it)
    {
        delete(*it);
    }
}
/**
*This method will create a ZEntity and add it to the manager for mangement. The build phase is seperated from the creation phase. This way, you can load all the entities before thread creation; so to
*reuse the Ogre resource system as much as possible. 
*
*/
ZEntity*
EntitiesManager::createZEntity()
{
    Entities::ZENT_KEY key;
    _getNewKey(key);
    ZEntityResource res(key, "ninja.mesh");
    ZEntity* zEnt = new ZEntity(res);
    _zEntsVec.push_back(zEnt);
    return zEnt;
}

void
EntitiesManager::removeZEntity()
{
}

void
EntitiesManager::_getNewKey(ZENT_KEY &key)
{
    ostringstream oss;
    oss << "ZENT_" << ++_KEY;
    key = oss.str();
}