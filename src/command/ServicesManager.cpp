#include <boost/shared_ptr.hpp>
using boost::shared_ptr;
#include "command/ServicesManager.h"
#include "CommandController.h"
using ZGame::CommandController;
#include "command/CreateEntCmd.h"

using namespace ZGame::COMMAND;

ServicesManager::ServicesManager()
{
    //Add the CreateEntCmd to CommandController.
    shared_ptr<CreateEntCmd> createEntCmd(new CreateEntCmd());
    ZGame::CommandController::getSingleton().addCommand(createEntCmd);
}

ServicesManager::~ServicesManager()
{
}

