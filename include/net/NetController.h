/** \file
* \brief Contains networking based controllers and related class.
*
*/

#ifndef _ZGAME_NETCONTROLLER_H
#define _ZGAME_NETCONTROLLER_H

#include <vector>

#include <boost/shared_ptr.hpp>


#include <Ogre.h>

#include "StringTable.h"
#include "RakNetTypes.h"
#include "RakNetworkFactory.h"
#include "RakPeerInterface.h"
//#include "ReplicaMember.h"
#include "BitStream.h"
#include "MessageIdentifiers.h"
#include "ReplicaEnums.h"
#include "Replica.h"
#include "ReplicaManager.h"
#include "NetworkIDManager.h"
#include "net/ZNetEntity.h"
#include "ZEntity.h"

namespace ZGame
{
    namespace Networking
    {
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

            typedef map<T, boost::shared_ptr<ZNetEntity> > NET_ENTITY_MAP;
            typedef map<T, boost::shared_ptr<ZEntity> > ZENTITY_MAP;

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

            }
            /**
            *This method will clear the composite Network Entities.
            *
            *\precondition It is assumed the clear process will be deterministic. Meaning, when we clear it, nothing else will try to interact in a undetermined and unsafe manner. (Because we are using delegates.)
            *\postcondition The composite Network Entity specified with key will be erased from the map.
            */
            void
                clearEntity(T key)
            {
                _netEntities.erase(key);
                _entities.erase(key);
                
            }
            /**
            *This method will clear everything for the two composite entity maps. 
            *\precondition: It is asssumed that we are calling at the logical point in the program where it is correct to clear all entities. IE. disconnecting from the server; and 
            *that clearing will be deterministic. By deterministic we mean that no one will try to interact with these objects after they have been cleared.
            *\postcondition: The two composite maps have been cleared. Do not expect to interact with the entities any longer.
            *
            *\note there should not be any thread unsafe behavior when calling this function. That is, nothing should be reading/writing to these delete buffers afterwards.
            *
            */
            //template<typename T>
            void
                clearAll()
            {
                //clear the netEntities. 
                _netEntities.clear();
                _entities.clear();
            }
        private:
            NET_ENTITY_MAP _netEntities;
            ZENTITY_MAP _entities;
        };

        /** \brief This class is the super class for networking based controllers.
        *
        *This is the super class for network controller. It encapsulates the usage of ReplicaManager. See RakNet ReplicaManager examples.
        */
        class NetController
        {
        public:
            //static NetworkEntitiesManager netEntManager;

            virtual ~NetController();
            //LifeCycle functions
            virtual bool
                onInit();
            virtual bool
                onUpdate();
            virtual bool
                onUpdate(const Ogre::FrameEvent &evt);
            virtual bool
                onDestroy(); 

        protected:



            bool _isServer;

            /** \brief Default constructor. */
            NetController(bool isServer=true);

            /** \brief pure virtual function that will return the ReceiveConstruction call back.*/
            virtual ReceiveConstructionInterface*
                getConstructionCB() = 0;
            /** \brief Pure virtual function that will return SendDownloadComplete call back.*/
            virtual SendDownloadCompleteInterface*
                getSetDownloadCompleteCB() = 0;
            /** \brief Pure virtual function that will return ReceiveDownloadComplete call back.*/
            virtual ReceiveDownloadCompleteInterface*
                getReceiveDownloadCompleteCB() = 0;

            RakPeerInterface* getRakPeerInterface() { return _rakPeer;}
            NetworkIDManager* getNetworkIDManager() { return &_networkIDManager;}
            ReplicaManager* getReplicaManager() { return &_replicaManager;}


            unsigned char
                getPackIdentifer(Packet* p)
            {
                if((unsigned char)p->data[0] == ID_TIMESTAMP)
                    return (unsigned char)p->data[sizeof(unsigned char)+sizeof(unsigned long)];
                else
                    return (unsigned char)p->data[0];
            }

        private:
            /** \brief The RakPeer interface. */
            RakPeerInterface* _rakPeer;
            /** \brief Stores a list of network object pointers, and indices to look them up by. 
            *
            *This class needs to registered with both RakPeer and with each created NetworkIDObject.
            */
            NetworkIDManager _networkIDManager;

            /** \brief Our Replica Manager. */
            ReplicaManager _replicaManager;

        };
    }
}

#endif