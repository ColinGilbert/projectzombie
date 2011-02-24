#include "world/GameController.h"
#include "ZWorkspace.h"
#include "ZWorkspaceController.h"
#include "toolset/ToolsetController.h"
#include "world/WorldController.h"
#include "entities/ComponentController.h"
#include "CommandController.h"
#include "command/CommandList.h"
#include "command/CreateEntCmd.h"

#include "entities/ZEntityBuilder.h"

using namespace ZGame::World;

GameController::GameController()
    : _requireWorkController(false)
{
}

GameController::~GameController()
{
}


void
    GameController::setWorkspaceController(ZGame::ZWorkspaceController* workspaceCtrl)
{
    _workspaceCtrl = workspaceCtrl;
}

void
    GameController::onBuildComponentEntity()
{
    _workspaceCtrl->onEnterSelectionMode();
    _requireWorkController = true;
    _deferredBuildComponent = true;
}

void
    GameController::onBuildTriggers()
{
}

void
    GameController::onBuildActionQueue()
{
}

void
    GameController::onWorkspace(ZGame::ZWorkspace* workspace)
{
    if(_deferredBuildComponent)
    {
        Ogre::AxisAlignedBox fillBox = workspace->getToolsetController()->getSelectionBox();
        std::vector<Ogre::Vector3> positions;
        workspace->getWorldController()->getVolumeRegionPositionsHeightField(fillBox, positions);
        std::cout << "GameController::onWorkspace" << std::endl;
        Entities::EntitiesBuilder builder;
        builder.build(positions, workspace->getComponentController()->getEntitiesManager());
    }
    _requireWorkController = false;
    _deferredBuildComponent = false;
}