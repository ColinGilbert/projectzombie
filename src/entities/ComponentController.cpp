#include "entities/ComponentController.h"
#include "ZInitPacket.h"
#include "entities/EntitiesManager.h"
#include "entities/RenderEntitiesManager.h"
#include "entities/ComponentEntityFactory.h"

using namespace ZGame::Entities;

ComponentController::ComponentController(std::auto_ptr<EntitiesManager> entsMgr, 
    std::auto_ptr<RenderEntitiesManager> renderEntsMgr) : _entsMgr(entsMgr), _renderEntsMgr(renderEntsMgr)
{
}

ComponentController::~ComponentController()
{
}

bool
    ComponentController::onInit(ZGame::ZInitPacket* packet)
{
    _renderEntsMgr->onInit(packet);
    ComponentEntityFactory compFactory;

    std::vector<Ogre::Any> anyComps;

    anyComps.push_back(Ogre::Any(_entsMgr.get()));
    anyComps.push_back(Ogre::Any(_renderEntsMgr.get()));

    compFactory.create("ZENTITIES", anyComps, 500);

    return true;
}

bool
    ComponentController::onDestroy()
{
    return true;
}

bool
    ComponentController::onUpdate(const Ogre::FrameEvent &evt)
{
    return true;
}

