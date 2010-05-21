#include <boost/shared_ptr.hpp>

using boost::shared_ptr;

#include "entities/RenderEntitiesManager.h"
#include "delegates/EntityDelegates.h"
#include "CommandController.h"
//#include "command/CommandList.h"
#include "command/CreateEntCmd.h"
using ZGame::COMMAND::CreateRenderEntCmd;
using namespace ZGame::Entities;

RenderEntitiesManager::RenderEntitiesManager()
{
    CreateRenderEntDlg dlg;
    dlg.bind(this, &RenderEntitiesManager::createRenderEntity);
    shared_ptr<Command> crtRdrCmd(new CreateRenderEntCmd());
    crtRdrCmd->setCommandMemento(dlg.GetMemento());
    CommandController::getSingleton().addCommand(crtRdrCmd);
}

RenderEntitiesManager::~RenderEntitiesManager()
{
}

bool
RenderEntitiesManager::createRenderEntity(const ZEntityResource* const res, const EntityUpdateEvent* const read)
{
    assert(res && "ZEnityResource pointer is null.");
    assert(read && "EntityUpdateEvent read pointer is null");
    return true;
}