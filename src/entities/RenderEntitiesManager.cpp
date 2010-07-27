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

