#include "command\CreateEntCmd.h"
#include "CommandController.h"

using namespace ZGame::COMMAND;

const COMMAND_KEY CreateEntCmd::KEY("createentcmd");
const COMMAND_KEY CreateRenderEntCmd::KEY("createrendercmd");

CreateEntCmd::CreateEntCmd(const COMMAND_KEY &key, int numOfParams)
: Command(key)
{
}

CreateEntCmd::~CreateEntCmd(void)
{
}

DelegateMemento
CreateEntCmd::execute(const Command &cmd)
{
    DelegateMemento NULL_MEMENTO;
    const CreateEntCmd* cmdPtr = static_cast<const CreateEntCmd*>(&cmd);

    const ZEntityResource* const res = cmdPtr->getResource();
    const EntityUpdateEvent* const update = cmdPtr->getUpdateEvent();

    CreateRenderEntCmd crtRdrCmd(*cmdPtr);
    //Call CreateRenderEntity
    CommandController::getSingleton().executeCmd(crtRdrCmd);
    //Call other commands relating to Entity creation.

    return NULL_MEMENTO;
}

/**
*A copy constructor which will construct this command from a CreateEntCmd.
*/
CreateRenderEntCmd::CreateRenderEntCmd(const CreateEntCmd &createEntCmd) : CreateEntCmd(CreateRenderEntCmd::KEY)
{
    setResource(*createEntCmd.getResource());
    setUpdateEvent(*createEntCmd.getUpdateEvent());
}

/**
*This method will execute this command and invoking the memento. It's defined recursively (see StringCommand.)
*
*/
DelegateMemento
CreateRenderEntCmd::execute(const Command &cmd)
{
    using Entities::CreateRenderEntDlg;
    DelegateMemento NULL_MEMENTO;
    const CreateRenderEntCmd* cmdPtr = static_cast<const CreateRenderEntCmd*>(&cmd);
   
    const ZEntityResource* const res = cmdPtr->getResource();
    const EntityUpdateEvent* const updateEvt = cmdPtr->getUpdateEvent();
    //Get the parameters.
    CreateRenderEntDlg rdrDlg;
    rdrDlg.SetMemento(*getCommandMemento());
    rdrDlg(res, updateEvt);

    return NULL_MEMENTO;
}