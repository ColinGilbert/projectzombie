#ifndef _CREATE_ENT_CMD
#define _CREATE_ENT_CMD

#include "command/Command.h"
#include "delegates/EntityDelegates.h"
#include "entities/ZEntityResource.h"
#include "entities/EntitiesDefs.h"
using ZGame::Entities::ZEntityResource;
using ZGame::Entities::EntityUpdateEvent;
using ZGame::Entities::ZENTITY_VEC;

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
        *NOTE: THIS IS not point in this class anymore.
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
            
            void
                setPositions(std::vector<Ogre::Vector3>* positions)
            {
                _positions = positions;
            }

        protected:

        private:
            Entities::EntitiesManager* _entMgr;
            std::vector<Ogre::Vector3>* _positions;
        };
    }
}

#endif