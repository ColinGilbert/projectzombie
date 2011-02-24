#include "entities/RenderEntityComp.h"
#include "entities/ZEntityResource.h"
#include "world/WorldScale.h"
using namespace ZGame::Entities;

RenderEntityComp::RenderEntityComp(const ZEntityResource* res, Ogre::SceneManager* scnMgr, Ogre::SceneNode* root)
    : Component()
{
    if(!res || !scnMgr || !root)
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Null pointer passed in.", 
        "RenderEntityComp::RenderEntityComp");
    _create(res, scnMgr, root);
}

RenderEntityComp::~RenderEntityComp()
{
    _node->getParent()->removeChild(_node);
}

void
    RenderEntityComp::removeAll(Ogre::SceneManager* scnMgr)
{
    auto objIter = _node->getAttachedObjectIterator();
    while(objIter.hasMoreElements())
    {
        Ogre::MovableObject* movable = static_cast<Ogre::MovableObject*>(objIter.getNext());
        _node->detachObject(movable);
        scnMgr->destroyMovableObject(movable);
    }
     
}

const fastdelegate::DelegateMemento
    RenderEntityComp::getInputDM(const Ogre::String &memName)
{
    fastdelegate::DelegateMemento retMem;
    if(memName == "OnReadInput")
    {
        Entities::ConstEntityUpdateEvent ev(this, &RenderEntityComp::onRead);
        retMem = ev.GetMemento();
    }
    else if(memName == "OnEventInput")
    {
        Entities::EntityOnEventEvent ev(this, &RenderEntityComp::onEvent);
        retMem = ev.GetMemento();
    }
    else
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Invalid memento string", 
        "RenderEntity::getInputDM");

    return retMem;
}

void
    RenderEntityComp::addOutputDM(const Ogre::String& memName, const fastdelegate::DelegateMemento dm)
{
}

void
    RenderEntityComp::deleteOutputDM(const Ogre::String& memName, const fastdelegate::DelegateMemento dm)
{

}

void
    RenderEntityComp::onEvent(unsigned int event)
{
    if(event == ONBUILD)
    {
        _node->setVisible(true);
    }
}

bool
    RenderEntityComp::onRead(const Ogre::Vector3 &pos, const Ogre::Quaternion &orient)
{
    _node->setPosition(pos);
    _node->setOrientation(orient); //Note this will have to changed,  orientation and position should be decoupled.
    return true;
}

void
    RenderEntityComp::_create(const ZEntityResource* res, Ogre::SceneManager* scnMgr, Ogre::SceneNode* node)
{
    Ogre::Entity* ent = scnMgr->createEntity(res->getKey(), res->getResourceName());
    ent->setMaterialName("PRJZ/MinecraftCharacter");
    auto scale = World::WorldScale::computeAWorldScale();
    if(!ent)
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Failed to create Entity", "RenderEntityComp::_create");
    //Assume the center is is at units / 2.0
    _node = node->createChildSceneNode();
    _node->attachObject(ent);
    _node->setScale(scale.metersPerUnit, scale.metersPerUnit, scale.metersPerUnit);
   // childNode->translate(0.0f, -scale.unitsPerMeter / 2.0, 0.0f);
    _node->setVisible(false);
}
