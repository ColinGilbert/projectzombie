#ifndef _CREATE_ENT_CMD
#define _CREATE_ENT_CMD

#include "command/Command.h"
#include "delegates/EntityDelegates.h"
#include "entities/ZEntityResource.h"
#include "entities/EntitiesDefs.h"
using ZGame::Entities::ZEntityResource;
using ZGame::Entities::EntityUpdateEvent;

namespace ZGame
{
    namespace Entities
    {
        class EntitiesManager;
        class RenderEntitiesManager;
        class ZEntity;
    }
    namespace COMMAND
    {
        /**
        *This class defines a Create Entity Command. For usage, it is assume that one using this command will allocate the command on the stat, or that they will be responsible for
        *deleting the command. For command implementation, it is assumed that the Command and Control system will manage this command resource. See Command and Control system.
        *
        */
        class CreateEntCmd :
            public Command
        {
        public:
            static const COMMAND_KEY KEY;
            CreateEntCmd() : Command(KEY) {}
            CreateEntCmd(const COMMAND_KEY &key, int numOfParams=1);
            virtual 
                ~CreateEntCmd(void);
            virtual DelegateMemento
                execute(const Command &cmd);
            void setEntitiesManager(Entities::EntitiesManager* entMgr);
            Entities::EntitiesManager* getEntitiesManager() const
            {
                return _entMgr;
            }
            

            /*
            void setResource(const ZEntityResource &res)
            {
            _res = res;
            }
            void setUpdateEvent(const EntityUpdateEvent &update)
            {
            _updateEvt = update;
            }
            const ZEntityResource* const getResource() const
            {
            return &_res;
            }
            const EntityUpdateEvent* const getUpdateEvent() const
            {
            return &_updateEvt;
            }*/

        protected:

        private:
            Entities::EntitiesManager* _entMgr;
            //ZEntityResource _res;
            //EntityUpdateEvent _updateEvt;
        };


        class CreateRenderEntCmd : public Command
        {
        public:
            static const COMMAND_KEY KEY;
            CreateRenderEntCmd() : Command(KEY) {}
            CreateRenderEntCmd(const CreateEntCmd &createEntCmd);
            CreateRenderEntCmd(Entities::ZEntity const* ent);

            //CreateRenderEntCmd(const COMMAND::COMMAND_KEY &key, int numOfParams=1);
            virtual DelegateMemento
                execute(const Command &cmd);

            void setRenderEntitiesManager(Entities::RenderEntitiesManager* rdrEntMgr);
            Entities::RenderEntitiesManager* getRenderEntitiesManager() const
            {
                return _rdrEntMgr;
            }
            Entities::ZEntity const* getZEntity() const
            {
                return _ent;
            }

        protected:
            /**
            virtual const
            fastdelegate::DelegateMemento*
            getCommandMemento(size_t index) const
            {
            assert(_mementos.size() == _NUM_OF_MEMENTOS && "Class invariant invalidated. The number of mementos for this command should be exactly two!");
            assert(index < _mementos.size() && "Class invariant invalidated. Memento get index is out-of-range!");
            return *_mementos.at(index);
            }**/
        private:
            Entities::RenderEntitiesManager* _rdrEntMgr;
            Entities::ZEntity const* _ent;
        };
    }
}

#endif