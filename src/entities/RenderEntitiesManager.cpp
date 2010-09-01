#include <iostream>

//#define VTFINST
//#define CROWD 1
using std::cout;
using std::endl;

#include <boost/shared_ptr.hpp>
#include "entities/EntitiesDefs.h"
#ifdef VTFINST
#include <OgreInstanceManager.h>
#include <OgreInstancedEntity.h>
#endif
using boost::shared_ptr;

#include "entities/RenderEntitiesManager.h"
#include "delegates/EntityDelegates.h"
#include "CommandController.h"
//#include "command/CommandList.h"
#include "command/CreateEntCmd.h"
#include "EngineView.h"
#include "entities/ZEntity.h"
#include "entities/InstancedRdrEntitiesBuilder.h"
using ZGame::COMMAND::CreateRenderEntCmd;
using namespace ZGame::Entities;

RenderEntitiesManager::RenderEntitiesManager() :
  _scnMgr(EngineView::getSingleton().getSceneManager())
{
  GetRenderEntitiesManager dlg;
  dlg.bind(this, &RenderEntitiesManager::getManager);
  CreateRenderEntCmd* cmd = new CreateRenderEntCmd();
  cmd->setRenderEntitiesManager(this);
  shared_ptr<Command> crtRdrCmd(cmd);
  crtRdrCmd->setCommandMemento(dlg.GetMemento());
  CommandController::getSingleton().addCommand(crtRdrCmd);
  _entNodesRoot = _scnMgr->getRootSceneNode()->createChildSceneNode("ENTITIES_ROOT");
#ifdef VTFINST
  _instMgr = 0;
#endif
}

RenderEntitiesManager::~RenderEntitiesManager()
{
}

void
RenderEntitiesManager::_removeChildObjects(Ogre::SceneNode* node)
{
  using namespace Ogre;
  if (node)
    {
      SceneNode::ObjectIterator itObject = node->getAttachedObjectIterator();
      while (itObject.hasMoreElements())
        {
          MovableObject* pObject = static_cast<MovableObject*> (itObject.getNext());
          node->detachObject(pObject);
	  //_instMgr->destroyInstancedEntity(pObject);
	  //node->getCreator()->destroyMovableObject(pObject);
        }
      SceneNode::ChildNodeIterator itChild = node->getChildIterator();
      while (itChild.hasMoreElements())
        {
          SceneNode* pChildNode = static_cast<SceneNode*> (itChild.getNext());
          _removeChildObjects(pChildNode);
        }
    }
}

void
RenderEntitiesManager::resetRenderEntities()
{

#ifdef VTFINST

  _removeChildObjects(_instancesRoot);
  _instancesRoot->removeAllChildren();
  _scnMgr->destroySceneNode(_instancesRoot);
  _scnMgr->destroyInstanceManager(_instMgr);
  _instMgr = 0;
  _instancesRoot = 0;

#else

   for (int i = 0; i < _renderInstances.size(); ++i)
   {
   _scnMgr->destroyInstancedGeometry(_renderInstances[i]);
   //_renderInstances[i] = 0;
   }

  _renderInstances.clear();
#endif
}

#ifdef VTFINST
bool
RenderEntitiesManager::createRenderEntities(ZENTITY_VEC::const_iterator begin, ZENTITY_VEC::const_iterator end, size_t numOfEnts)
{
  using namespace Ogre;
  using Entities::InstancedRdrEntitiesBuilder;
  InstancedRdrEntitiesBuilder build(_scnMgr);

  std::string meshName = "robot";

  _instancesRoot = _scnMgr->getRootSceneNode()->createChildSceneNode("RDR_ENTITIES_ROOT");
  if(_instMgr == 0)
    _instMgr = _scnMgr->createInstanceManager("MyInstanceMgr", "robot.mesh", ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME,
      Ogre::InstanceManager::TextureVTF, 300, IM_USEALL);
  //_instMgr = _scnMgr->createInstanceManager("MyInstanceMgr", "robot.mesh", ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME,
  //Ogre::InstanceManager::ShaderBased, 80);

  cout << "Creating instanced entities" << endl;
  cout << "Num of ents: " << numOfEnts << endl;
  ZENTITY_VEC::const_iterator iter = begin;
  for (size_t i = 0; i < numOfEnts; ++i)
    {
      InstancedEntity *ent = _instMgr->createInstancedEntity("BlueVTF");
      SceneNode* sceneNode = _instancesRoot->createChildSceneNode();
      sceneNode->attachObject(ent);
      Vector3 pos;
      Quaternion orient;
      (*iter)->onWrite(pos, orient);
      sceneNode->setPosition(pos);
      sceneNode->setOrientation(orient);
      sceneNode->setScale(Ogre::Vector3(2.0f, 2.0f, 2.0f));
      AnimationState* anim = ent->getAnimationState("Walk");
      _animations.push_back(anim);
      anim->setTimePosition(Ogre::Math::RangeRandom(0.0f, 30.0f));
      anim->setEnabled(true);
      iter++; //Let's just refactor and pass in the vector so we don't have to do this.
    }

  cout << "Finished creating instanced entities" << endl;

}
#else

 bool
 RenderEntitiesManager::createRenderEntities(ZENTITY_VEC::const_iterator begin, ZENTITY_VEC::const_iterator end, size_t numOfEnts)
 {
 using Entities::InstancedRdrEntitiesBuilder;
 InstancedRdrEntitiesBuilder builder(_scnMgr);
 std::string meshName = "Barrel";
 #ifdef CROWD
 meshName = "robot";
 #endif

 builder.build(numOfEnts, _renderInstances, meshName);

 InstancedGeometry* batch = _renderInstances[0];

 InstancedGeometry::BatchInstanceIterator regIt = batch->getBatchInstanceIterator();
 size_t k = 0;
 ZENTITY_VEC::const_iterator iter = begin;

 size_t batchInstanceIdx = 0;

 while (regIt.hasMoreElements ())
 {

 InstancedGeometry::BatchInstance *r = regIt.getNext();
 cout << "Batch Indstance Idx: " << batchInstanceIdx++ << endl;
 InstancedGeometry::BatchInstance::InstancedObjectIterator bit = r->getObjectIterator();
 int j = 0;
 Vector3 pos; Quaternion orient;

 while(bit.hasMoreElements())
 {
 cout << "Batch Instance has InstancedObjects" << endl;
 InstancedGeometry::InstancedObject* obj = bit.getNext();
 (*iter)->onWrite(pos, orient);
 obj->setPosition(pos);
 obj->setOrientation(orient);
 #ifdef CROWD
 obj->setScale(Ogre::Vector3(2.0f, 2.0f, 2.0f));
 AnimationState* anim = obj->getAnimationState("Walk");
 if(anim)
 {
 _animations.push_back(anim);
 anim->setTimePosition(Ogre::Math::RangeRandom(0.0f, 30.0f));
 anim->setEnabled(true);
 }
 #else
 obj->setScale(Ogre::Vector3(10.0f, 25.0f, 10.0f));
 #endif

 ++j;
 iter++;
 k++;
 cout << "Number of InstancedObject: " << k << endl;


 }

 }
 //if(iter != end)
 //OGRE_EXCEPT(Exception::ERR_INVALID_STATE,"There are more zEntities than there are instanced geometries.", "RenderEntities::createRenderEntities");

 return true;
 }
#endif

/**This method will build the scene nodes.
 *
 *\note: current method is to simply create a bunch of nodes directly off root node of the scene. In the future we will have to refactor such that we
 *build a tree off a node which we create from the root. Doing it this way, we only have to store the root node of the entities tree and thus skipping the
 *need to store a vector of scene nodes. (e.g: we can iterate through the scene nodes using Ogre's build-in node traversal facilities.)
 **/
bool
RenderEntitiesManager::createRenderEntity(ZEntity const* ent)
{
  //cout << "In RenderEntitieManager::createRenderEntity" << endl;
  using namespace Ogre;
  using Entities::ZEntityResource;
  using Entities::ZEntity;

  ZEntityResource const* res = ent->getResource();
  assert(res && "ZEnityResource pointer is null.");

  Entity* ogreEnt = _scnMgr->createEntity(ent->getEntityName(), "ninja.mesh");//res->getResourceName());
  SceneNode* node = _entNodesRoot->createChildSceneNode(ent->getEntityName());
  //SceneNode* node = _scnMgr->getRootSceneNode()->createChildSceneNode(ent->getEntityName());
  SceneNode* nodeChild = node->createChildSceneNode();
  nodeChild->attachObject(ogreEnt);
  //node->setScale(0.05f, 0.05f, 0.05f);
  node->setPosition(ent->getPosition());
  node->setOrientation(ent->getOrientation());
  //_entNodes.push_back(node);

  //assert(read && "EntityUpdateEvent read pointer is null");
  return true;
}


#ifdef VTFINST
void
RenderEntitiesManager::updateRenderEntities(const float* posBuf, const float* orientBuf, const float* velocityBuf, const float &dt)
{
  size_t numOfEnts = 0;
  using namespace Ogre;
  SceneNode::ChildNodeIterator itChild = _instancesRoot->getChildIterator();
  size_t idx = 0;
  const size_t DIM = 4;
  const Quaternion initialOrient(Ogre::Radian(Ogre::Degree(-90.0f).valueRadians()), Ogre::Vector3::UNIT_Y);
  //const Quaternion initialOrient;
  Ogre::vector<AnimationState*>::type::iterator it;
  //it=_animations.begin();
  //Vector3 pos;
  //Quaternion quat;
  while(itChild.hasMoreElements())
    {
      //cout << "Updating child nodes!" << endl;
      SceneNode* pChildNode = static_cast<SceneNode*> (itChild.getNext());
      Vector3 pos(posBuf[idx], posBuf[idx + 1], posBuf[idx + 2]);
      //cout << "Pos: " << pos << endl;
      pos.y = 0.0f;
      pChildNode->setPosition(pos);
      Quaternion quat(orientBuf[idx], orientBuf[idx + 1], orientBuf[idx + 2], orientBuf[idx + 3]);
      quat = quat * initialOrient;
      pChildNode->setOrientation(quat);
      
      //(*it)->addTime(dt*1.0f);
      //(*it)->addTime(dt*velocityBuf[idx]);
      //it++;
      idx += DIM;
    }
}
#else



void
RenderEntitiesManager::updateRenderEntities(const float* posBuf, const float* orientBuf, const float* velocityBuf, const float &dt)
{
  //Get all the node child;
  size_t numOfEnts = 0;
  using namespace Ogre;
  //Loop through each instanced geometry and update it's position.
  InstancedGeometry::BatchInstanceIterator iter = _renderInstances[0]->getBatchInstanceIterator();

  size_t idx = 0;
  const size_t DIM = 4;
  const Ogre::Quaternion initialOrient(Ogre::Radian(Ogre::Degree(-90.0f).valueRadians()), Ogre::Vector3::UNIT_Y);
  //Vector3 pos;
#ifdef CROWD
  Ogre::vector<AnimationState*>::type::iterator it;
  it=_animations.begin();

#endif
  //const Ogre::Quaternion initialOrient;
  while (iter.hasMoreElements())
    {
      InstancedGeometry::BatchInstance* r = iter.getNext();
      InstancedGeometry::BatchInstance::InstancedObjectIterator bit = r->getObjectIterator();

      while (bit.hasMoreElements())
        {
          InstancedGeometry::InstancedObject* obj = bit.getNext();
          //cout << "Pos buffer: " << posBuf[idx + 1] << endl;
          Vector3 pos(posBuf[idx], posBuf[idx + 1], posBuf[idx + 2]);
          pos.y = 0.0f;
          //cout << "Postion for rdr ent: " << pos << endl;
          obj->setPosition(pos);
          Quaternion quat(orientBuf[idx], orientBuf[idx + 1], orientBuf[idx + 2], orientBuf[idx + 3]);
          quat = quat * initialOrient;
          obj->setOrientation(quat);
#ifdef CROWD

          (*it)->addTime(dt*velocityBuf[idx]);
          it++;
#endif
          idx += DIM; //advance by dimension.
        }
    }

  //cout << "NumOfEnts in updateRenderEntities: " << numOfEnts << endl;
}
#endif
