#ifndef _SERVICES_MANAGER_H
#define _SERVICES_MANAGER_H
/**
* \file This file defines the modules for implement Services.
*
*/
#include "CommandController.h"

namespace ZGame
{
    namespace COMMAND
    {
        //class CommandController;
        /**
        *This class defines a Manager for the concept of services. Services in COMMAND namespace are Commands. And these Commands
        *are initialized and added to the Command system through ServicesManager. What this means is you can use this as a "module" container 
        *which contains all the services you want (e.g EntitiesManager.) Since Services are initialized by the CommandController, it's services
        *will be availible as soon the CommandController is availible. ServicesManager is also a place where dependencies are figured out, all in 
        *the same place.
        */
        class ServicesManager
        {
        public:
            friend class CommandController;
	    ServicesManager();
            ~ServicesManager();
        protected:
            
        private:          
        };
    }
}

#endif
