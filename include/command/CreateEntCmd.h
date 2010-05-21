#ifndef _CREATE_ENT_CMD
#define _CREATE_ENT_CMD

#include "command/Command.h"
#include "delegates/EntityDelegates.h"
#include "entities/ZEntityResource.h"


using ZGame::Entities::ZEntityResource;
using ZGame::Entities::EntityUpdateEvent;

namespace ZGame
{
    namespace COMMAND
    {
        
        class CreateEntCmd :
            public Command
        {
        public:
            static const COMMAND_KEY KEY;
            CreateEntCmd(const COMMAND_KEY &key, int numOfParams=1);
            virtual 
                ~CreateEntCmd(void);
            virtual DelegateMemento
                execute(const Command &cmd);
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
            }
            
        protected:
            
        private:
            ZEntityResource _res;
            EntityUpdateEvent _updateEvt;
        };

        class CreateRenderEntCmd : public CreateEntCmd
        {
        public:
            static const COMMAND_KEY KEY;
            CreateRenderEntCmd() : CreateEntCmd(KEY) {}
            CreateRenderEntCmd(const CreateEntCmd &createEntCmd);
            //CreateRenderEntCmd(const COMMAND::COMMAND_KEY &key, int numOfParams=1);

            virtual DelegateMemento
                execute(const Command &cmd);

        protected:
        private:
        };
    }
}

#endif