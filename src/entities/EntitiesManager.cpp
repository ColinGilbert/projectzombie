#include <iostream>
#include <algorithm>
#include <string>
using std::ostringstream;
using std::cout;
using std::endl;

#include "entities/EntitiesManager.h"
#include "entities/ZEntity.h"
#include "entities/ZEntityBuilder.h"
#include "CommandController.h"
#include "command/CreateEntCmd.h"
#include "delegates/EntityDelegates.h"
#include "entities/ZEntityBuilder.h"
#include "entities/InstancedRdrEntitiesBuilder.h"
#include "world/WorldController.h"
#include "ZCL/ZCLController.h"
#include "world/WorldScale.h"
using namespace Ogre;

using namespace ZGame::Entities;

unsigned int EntitiesManager::_KEY = 0;

extern ZGame::World::WorldScale WSCALE;

EntitiesManager::EntitiesManager() : _numOfEnts(0)
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
  clearZEntities(); //just to be safe. 
}

void
EntitiesManager::clearZEntities()
{
  //Remove the entities.
  for (ZENT_ITER it = _zEntsVec.begin(); it != _zEntsVec.end(); ++it)
    {
      delete (*it);
    }
  _zEntsVec.clear();
}

void
    EntitiesManager::setCursor(ZENTITY_VEC::const_iterator cursor)
{
    _cursor = cursor;
}

ZENTITY_VEC::const_iterator
    EntitiesManager::getCursor()
{
    return _cursor;
}

ZENTITY_VEC::const_iterator
    EntitiesManager::getEndCursor()
{
    return _zEntsVec.cend();
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

void
    EntitiesManager::getFromPool(ZENTITY_VEC::iterator &begin, ZENTITY_VEC::iterator &end,
    size_t numFromPool)
{
    //Is numFromPool greater than total size?
    if(numFromPool > _zEntsVec.size())
    {
        //Just return;
        begin = _zEntsVec.end();
        end = _zEntsVec.end();
        //OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, "Trying to get number of entities N from a entities pool size that is less than N",
          //  "EntitiesManager::getFromPool");
        return;
    }
    //Just hack this for now.
    int newPoolSize = _poolSize - numFromPool;
    if(newPoolSize < 0)
    {
        //reset the damn thing.
        numFromPool = _poolSize;
        end = _zEntsVec.begin() + _poolSize;
        begin = end - numFromPool;
        _poolSize = _zEntsVec.size();
    }
    else
    {
        end = _zEntsVec.begin() + _poolSize;
        begin = end - numFromPool;
        _poolSize = newPoolSize;
    }
}