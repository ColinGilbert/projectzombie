#include <iostream>


#include <boost/shared_ptr.hpp>
#include "entities/EntitiesDefs.h"

using std::shared_ptr;

#include "entities/RenderEntitiesManager.h"
#include "CommandController.h"
#include "command/CreateEntCmd.h"
#include "entities/ZEntity.h"
#include "entities/RenderEntityComp.h"
#include "ZInitPacket.h"


using namespace ZGame::Entities;

RenderEntitiesManager::RenderEntitiesManager() :
_scnMgr(0), _instancesRoot(0)
{
}

bool
    RenderEntitiesManager::onInit(ZGame::ZInitPacket *initPacket)
{
    _scnMgr = initPacket->sceneManager;

    _instancesRoot = _scnMgr->getRootSceneNode()->createChildSceneNode("RDR_ENTS_ROOT");
    if(!_instancesRoot)
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE,
        "Could not create render entities root node", "RenderEntitiesManager::onInit");

    return true;
}

RenderEntitiesManager::~RenderEntitiesManager()
{
    resetRenderEntities();
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
        }
        SceneNode::ChildNodeIterator itChild = node->getChildIterator();
        while (itChild.hasMoreElements())
        {
            SceneNode* pChildNode = static_cast<SceneNode*> (itChild.getNext());
            _removeChildObjects(pChildNode);
            node->removeChild(pChildNode);
            OGRE_DELETE pChildNode;
        }
    }
}

void
    RenderEntitiesManager::resetRenderEntities()
{
    for(auto iter = _renderComps.begin(); iter != _renderComps.end(); ++iter)
    {
        (*iter)->removeAll(_scnMgr);
        delete *iter;
    }
}

RenderEntityComp*
    RenderEntitiesManager::createComponent(const ZEntityResource* res)
{
    RenderEntityComp* rdrEntComp = new Entities::RenderEntityComp(res, _scnMgr, _instancesRoot);
    _renderComps.push_back(rdrEntComp);
    return rdrEntComp;
}
