#include <iostream>

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
using namespace Ogre;

using namespace ZGame::Entities;

int EntitiesManager::_KEY = 0;

EntitiesManager::EntitiesManager() : _numOfEnts(0), _numOfGroups(0), _numPerGroup(InstancedRdrEntitiesBuilder::MAX_PER_BATCH),
    _increaseByMultiplier(2)
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
  _ents.clear();
}

void
EntitiesManager::clearBuffers()
{
  _ents.clear();
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

/**
 *This method will convert the zEntities in this manager into ZEntitiBuffers form.
 *
 **/
void
EntitiesManager::zEntitiesToBuffer()
{
  size_t vecDim = 4;
  //Allocate the buffers for N number of elements.
  _ents.numOfEnts = _zEntsVec.size();
  if(_ents.numOfEnts < 1)
    return;
  _ents.COMPONENT_DIM = vecDim;
  cout << "Allocating buffer for ZEntities to Buffer conversion." << endl;
  _ents.worldPos = new Real[_ents.numOfEnts * vecDim]; //4D vector per entity.
  _ents.worldOrient = new Real[_ents.numOfEnts * vecDim]; //4D quaternion per entity.
  _ents.velocity = new Real[_ents.numOfEnts * vecDim]; //4D vector of velocity per entity.
  _ents.goals = new Real[_ents.numOfEnts * vecDim];
  _ents.mode = new uchar[_ents.numOfEnts]; //byte mod variable per entity.
  //Iterate through ZEntities and copy the data over for reach ZEntity
  size_t entIdx = 0;
  Vector3 initVel(0.0f, 0.0f, 0.0f); //initial velocity is zero.
  //size_t grpIdx = 0;
  //size_t endGrpIdx = _groups[grpIdx].numOfEnts;
  for (ZENT_ITER it = _zEntsVec.begin(); it != _zEntsVec.end(); ++it)
    {
      const Vector3& pos = (*it)->getPosition();
      const Quaternion& orient = (*it)->getOrientation();
      const Vector3& goal = (*it)->getGoal();

      //We use the entity velocity buffer's last element for storing thrust.
      initVel.x = Ogre::Math::RangeRandom(200.0f, 350.0f);
      _ents.worldPos[entIdx * vecDim] = pos.x;
      _ents.worldPos[entIdx * vecDim + 1] = pos.y;
      _ents.worldPos[entIdx * vecDim + 2] = pos.z;
      _ents.worldPos[entIdx * vecDim + 3] = 1.0f;
      _ents.worldOrient[entIdx * vecDim] = orient.w;
      _ents.worldOrient[entIdx * vecDim + 1] = orient.x;
      _ents.worldOrient[entIdx * vecDim + 2] = orient.y;
      _ents.worldOrient[entIdx * vecDim + 3] = orient.z;
      _ents.velocity[entIdx * vecDim] = 0.0f;
      _ents.velocity[entIdx * vecDim + 1] = 0.0f;
      _ents.velocity[entIdx * vecDim + 2] = 0.0f;
      _ents.velocity[entIdx * vecDim + 3] = initVel.x;
      _ents.mode[entIdx] = (uchar) (1);
      _ents.goals[entIdx * vecDim] = goal.x;
      _ents.goals[entIdx * vecDim + 1] = goal.y;
      _ents.goals[entIdx * vecDim + 2] = goal.z;
      _ents.goals[entIdx * vecDim + 3] = 0.0f;
      entIdx++;
    }
  cout << "Done buffer conversion." << endl;

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

EntitiesGroup*
EntitiesManager::createGroup()
{
  int id = _groups.size();
  EntitiesGroup* grp = new EntitiesGroup(id, 80);
  _groups.push_back(grp);
  return grp;
}

void
EntitiesManager::resetEntities()
{
  this->clearBuffers();
  this->clearZEntities();
  _numOfEnts = 0;
}

void
EntitiesManager::buildGroups()
{
  _numOfEnts = _getNumOfEntities();
  using Entities::EntitiesBuilder;
  EntitiesBuilder builder;
  builder.build(this);
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL,
      "ZEntityBuilder::build finished.");


}
