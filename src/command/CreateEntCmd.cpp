#include <iostream>
using std::cout;
using std::endl;
#include "command/CmdException.h"
#include "command/CreateEntCmd.h"
#include "CommandController.h"
#include "entities/RenderEntitiesManager.h"
#include "entities/ZEntity.h"
#include "entities/EntitiesManager.h"
#include "delegates/EntityDelegates.h"

using namespace ZGame::COMMAND;
using namespace ZGame;
using COMMAND::CmdException;

const COMMAND_KEY CreateEntCmd::KEY("createentcmd");
const COMMAND_KEY CreateRenderEntCmd::KEY("createrendercmd");

CreateEntCmd::CreateEntCmd(const COMMAND_KEY &key, int numOfParams)
: Command(key)
{
}

CreateEntCmd::~CreateEntCmd(void)
{
}

void CreateEntCmd::setEntitiesManager(Entities::EntitiesManager* entMgr)
{
    _entMgr = entMgr;
}

DelegateMemento
CreateEntCmd::execute(const Command &cmd)
{
    cout << "In CreateEntCmd::execute" << endl;
    using ZGame::Entities::EntitiesManager;
    using Entities::ZENTITY_VEC;
    using Entities::ZENT_ITER;

    DelegateMemento NULL_MEMENTO;
    
    const CreateEntCmd* cmdPtr = static_cast<const CreateEntCmd*>(&cmd);
    EntitiesManager* entMgr = getEntitiesManager();
    ZENTITY_VEC* ents = entMgr->getEntities();
    cout << "Entity vector in entity manager size is: " << ents->size() << endl;
    
    try
    {
    CreateRenderEntCmd crtRdrCmd;
    //We iterate through the entities and pass each entity to the create render entity command.
    for(ZENTITY_VEC::iterator iter = ents->begin(); iter != ents->end(); ++iter)
    {      
        CreateRenderEntCmd crtRdrCmd(*iter);
        //Call CreateRenderEntity
        CommandController::getSingleton().executeCmd(crtRdrCmd);
        //Call other commands relating to Entity creation.
    }
    }catch(CmdException e)
    {
        std::string error("There was an exception during CreateRenderEntCmd");
        error += e.what();
        //For this command missing this command is pretty severe, we should throw it.
        throw CmdException(error);
    }
    //Finally, convert the ZEntities into buffer format. 
    cout << "Converting ZEntities to buffers. " << endl;
    entMgr->zEntitiesToBuffer();
    entMgr->clearZEntities();

    return NULL_MEMENTO;
}



/**
*A copy constructor which will construct this command from a CreateEntCmd.

CreateRenderEntCmd::CreateRenderEntCmd(const CreateEntCmd &createEntCmd) : CreateEntCmd(CreateRenderEntCmd::KEY)
{
    setResource(*createEntCmd.getResource());
    setUpdateEvent(*createEntCmd.getUpdateEvent());
}
**/
CreateRenderEntCmd::CreateRenderEntCmd(Entities::ZEntity const* ent) : Command(CreateRenderEntCmd::KEY), _ent(ent)
{

    //setResource(ent.getResource());
}

void CreateRenderEntCmd::setRenderEntitiesManager(Entities::RenderEntitiesManager* rdrEntMgr)
{
    _rdrEntMgr = rdrEntMgr;
}

/**
*This method will execute this command and invoking the memento. It's defined recursively (see StringCommand.)
*
*/
DelegateMemento
CreateRenderEntCmd::execute(const Command &cmd)
{
    //using Entities::GetRenderEntitiesManager;
    using Entities::RenderEntitiesManager;
    DelegateMemento NULL_MEMENTO;
    //GetRenderEntitiesManager getRdrMgr;

    const CreateRenderEntCmd* cmdPtr = static_cast<const CreateRenderEntCmd*>(&cmd);
    //getRdrMgr.SetMemento(*getCommandMemento());
    RenderEntitiesManager* rdrEntMgr = getRenderEntitiesManager();

    rdrEntMgr->createRenderEntity(cmdPtr->getZEntity());

    //const ZEntityResource* const res = cmdPtr->getResource();
    //const EntityUpdateEvent* const updateEvt = cmdPtr->getUpdateEvent();
    //Get the parameters.
    //CreateRenderEntDlg rdrDlg;
    //rdrDlg.SetMemento(*getCommandMemento(_CREATE_RENDER_ENT_IDX));
    //rdrDlg(res, updateEvt);
    //rdrDlg.SetMemento(*getCommandMemento(_GET_RENDER_ENT_PROPS));
    //rdrDlg();

    return NULL_MEMENTO;
}

/*
void
CreateRenderEntCmd::setRenderEntityManager(const Entities::RenderEntitiesManager *entMgr)
{
    _entMgr = entMgr;
}
*/