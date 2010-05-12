#ifndef _ZGAME_NETWORK_ENTITIES_MANAGER_H
#define _ZGAME_NETWORK_ENTITIES_MANAGER_H

#include <boost/shared_ptr.hpp>
#include "net/ZNetEntity.h"
#include "ZEntity.h"

namespace ZGame
{

    namespace Entities
    {
        class ZEntity;
    }
    using Entities::ZEntity;
    namespace Networking
    {
        class ZNetEntity;
        /**
        *This Initial implementation of NetworkEntitiesManager.
        *
        */
        template<typename T>
        class NetworkEntitiesManager
        {
        public:
            NetworkEntitiesManager(){}
            virtual ~NetworkEntitiesManager(){}

            void
                addEntity(T key, boost::shared_ptr<ZNetEntity> &znetEntSmart,
                boost::shared_ptr<ZEntity> &zEntSmart)
            {
                ZENTITY_MAP::iterator curZEntIter;
                curZEntIter = _entities.find(key);
                assert(curZEntIter == _entities.end() && "Logical Assert Failed: Unique key violation failed in Net Entitiy Manager."); //assert that we are in fact inserting a new NetworkID key into the system.
                _entities[key] = zEntSmart;

                NET_ENTITY_MAP::iterator curNetEntIter;
                curNetEntIter = _netEntities.find(key);
                assert(curNetEntIter == _netEntities.end() && "Logical Assert Failed: Unique key violation failed in Net Entitiy Manager.");
                _netEntities[key] = znetEntSmart;

                //load the actual thing.

            }
            /** \brief This method will clear the composite Network Entities.*/
            void
                clearEntity(T key)
            {
                _netEntities.erase(key);
                _entities.erase(key);

            }
            /** \brief This method will clear everything for the two composite entity maps.*/
            void
                clearAll()
            {
                //clear the netEntities. 
                _netEntities.clear();
                _entities.clear();
            }

        private:
            typedef std::map<T, boost::shared_ptr<ZNetEntity> > NET_ENTITY_MAP;
            typedef std::map<T, boost::shared_ptr<Entities::ZEntity> > ZENTITY_MAP;

            NET_ENTITY_MAP _netEntities;
            ZENTITY_MAP _entities;
        };
    }
}

#endif