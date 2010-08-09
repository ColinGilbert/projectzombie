#include <iostream>

using std::cout;
using std::endl;

#include <boost/shared_ptr.hpp>

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

RenderEntitiesManager::RenderEntitiesManager() : _scnMgr(EngineView::getSingleton().getSceneManager())
{
    GetRenderEntitiesManager dlg;
    dlg.bind(this, &RenderEntitiesManager::getManager);
    CreateRenderEntCmd* cmd = new CreateRenderEntCmd();
    cmd->setRenderEntitiesManager(this);
    shared_ptr<Command> crtRdrCmd(cmd);
    crtRdrCmd->setCommandMemento(dlg.GetMemento());
    CommandController::getSingleton().addCommand(crtRdrCmd);
    _entNodesRoot = _scnMgr->getRootSceneNode()->createChildSceneNode("ENTITIES_ROOT");
}

RenderEntitiesManager::~RenderEntitiesManager()
{
}

bool
    RenderEntitiesManager::createRenderEntities(ZENTITY_VEC::const_iterator begin, ZENTITY_VEC::const_iterator end, size_t numOfEnts)
{
    using Entities::InstancedRdrEntitiesBuilder;
    InstancedRdrEntitiesBuilder builder(_scnMgr);
    builder.build(numOfEnts, _renderInstances, "razor");
    InstancedGeometry* batch = _renderInstances[0];
    InstancedGeometry::BatchInstanceIterator regIt = batch->getBatchInstanceIterator();
    size_t k = 0;
    ZENTITY_VEC::const_iterator iter = begin;
    while (regIt.hasMoreElements ())
    {

        InstancedGeometry::BatchInstance *r = regIt.getNext();

        InstancedGeometry::BatchInstance::InstancedObjectIterator bit = r->getObjectIterator();
        int j = 0;
        Vector3 pos; Quaternion orient;
        while(bit.hasMoreElements())
        {
            InstancedGeometry::InstancedObject* obj = bit.getNext();
            (*iter)->onWrite(pos, orient);
            obj->setPosition(pos);
            obj->setOrientation(orient);
            ++j;
            iter++;
            
        }
        k++;
    }
    if(iter != end)
        OGRE_EXCEPT(Exception::ERR_INVALID_STATE,"There are more zEntities than there are instanced geometries.", "RenderEntities::createRenderEntities");
    return true;
}

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

void
    RenderEntitiesManager::updateRenderEntities(const float* posBuf, const float* orientBuf)
{
    //Get all the node child;
    size_t numOfEnts = 0;
    using namespace Ogre;
    //Loop through each instanced geometry and update it's position.
    InstancedGeometry::BatchInstanceIterator iter = _renderInstances[0]->getBatchInstanceIterator();


    size_t idx = 0;
    const size_t DIM = 4;

    while(iter.hasMoreElements())
    {
        InstancedGeometry::BatchInstance* r = iter.getNext();
        InstancedGeometry::BatchInstance::InstancedObjectIterator bit = r->getObjectIterator();

        while(bit.hasMoreElements())
        {
            InstancedGeometry::InstancedObject* obj = bit.getNext();
            const Vector3 pos(posBuf[idx], posBuf[idx+1], posBuf[idx+2]);
            obj->setPosition(pos);
            const Quaternion quat(orientBuf[idx], orientBuf[idx+1], orientBuf[idx+2], orientBuf[idx+3]);
            obj->setOrientation(quat);
            idx += DIM; //advance by dimension.
        }
    }
    //cout << "NumOfEnts in updateRenderEntities: " << numOfEnts << endl;
}

