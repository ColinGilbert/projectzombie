#ifndef _ZENTITIES_MANAGER_H
#define _ZENTITIES_MANAGER_H

#include <vector>
#include <Ogre.h>
#include <boost/shared_ptr.hpp>
//#include "utilities/cl.hpp"
#include "entities/EntitiesDefs.h"
#include "net/ZNetEntity.h"
#include "ZException.h"

using std::vector;

namespace ZGame
{

    namespace Entities
    {
        class ZEntity;
        /**
        *This structure is used to represent ZEntities in the form of buffers. ZEntities as buffers is more succinct as a form for efficient processing.
        *WIP DO NOT USE... class...
        */
        struct ZEntityBuffers
        {
            ZEntityBuffers() :
        numOfEnts(0), worldPos(0), mode(0), worldOrient(0),goals(0),storeone(0),
            density(0)
        {
        }
        size_t numOfEnts;
        size_t COMPONENT_DIM;
        Ogre::Real* worldPos;
        Ogre::Real* worldOrient;
        Ogre::Real* velocity;
        Ogre::Real* goals;
        Ogre::Real* storeone;
        Ogre::Real* density;


        Ogre::uchar* mode;
        void
            clear()
        {
            if(worldPos)
                delete[] worldPos;
            worldPos = 0;
            if(worldOrient)
                delete[] worldOrient;
            worldOrient = 0;
            if(mode)
                delete[] mode;
            mode = 0;
            if(goals)
                delete[] goals;
            goals = 0;

            if(storeone)
                delete[] storeone;
            storeone = 0;

            if(density)
                delete[] density;
            density = 0;
            numOfEnts = 0;
        }
        };

        class EntitiesGroup
        {
        public:
            EntitiesGroup(int groupId, int num, Ogre::Vector3 cen = Ogre::Vector3::ZERO, Ogre::Real r = Ogre::Real(200.0f)) :
              id(groupId), numOfEnts(num), center(cen), radius(r), goal(cen)
              {
              }
              int id;
              int numOfEnts;
              Ogre::Vector3 center;
              Ogre::Real radius;
              Ogre::Vector3 goal;

        };

        /** This class defines an ZEntity Components Manager. **/
        class EntitiesManager
        {
        public:
            EntitiesManager();
            virtual
                ~EntitiesManager();
            /** \brief This method will create an ZEntity in the system.*/
            ZEntity*
                createZEntity();
            /** \brief This method will remove an ZEntity from the system.*/
            void
                removeZEntity();
            /** \brief This method will clear all ZEntities from the system.**/
            void
                clearZEntities();

            ZENTITY_VEC*
                getEntities()
            {
                return &_zEntsVec;
            }

            size_t
                getNumOfEntities()
            {
                return _numOfEnts;
            }

            void
                setPoolSize(int poolSize)
            {
                _poolSize = poolSize;
            }

            void
                getFromPool(ZENTITY_VEC::iterator &begin, 
                ZENTITY_VEC::iterator &end, size_t numFromPool);

            EntitiesManager*
                getManager()
            {
                return this;
            }

            void
                setCursor(ZENTITY_VEC::const_iterator cursor);
            ZENTITY_VEC::const_iterator
                getCursor();
            ZENTITY_VEC::const_iterator
                getEndCursor();
        protected:
        private:
            static unsigned int _KEY;
            ZENTITY_VEC _zEntsVec;
            size_t _numOfEnts;
            int _poolSize;
            void
                _getNewKey(Entities::ZENT_KEY &key);
            ZENTITY_VEC::const_iterator _cursor;
        };

        using ZGame::Networking::ZNetEntity;
        //static methods.
        /*\brief This templated static method will construct the composite ZNetEntity given EntityClass.
        *
        *\precondition: We expect EntityClass dervies from ZEntity, and thus we will not do any type checking here.
        *\postcondition: the composite entity is created.
        *\note: For server you need to pass in the NetworkIDManager. It seems that if you call GetNetworkID() before calling construction (or DisableInterface) of replica it will crash.
        */
        ///////////template methods//////////////////
        template<typename EntityClass>
        void
            constructCompositeZNetEntity(boost::shared_ptr<Entities::ZEntity> &entityShrPtr, boost::shared_ptr<ZNetEntity> &znetEntityShrPtr, EntityClass* entPtr,
            ReplicaManager* replicaManager, SystemAddress creatorAddress, NetworkID* netId, bool isServer)
        {
            /*
            using namespace Entities;
            cout << "In ZNetEntity construct composite ZNetEntity." << endl;

            //create ZNetEntity.
            entPtr = new EntityClass();
            znetEntityShrPtr.reset(new ZNetEntity(*replicaManager, creatorAddress, isServer));
            entityShrPtr.reset(static_cast<ZEntity*>(entPtr));


            if(isServer)
            {
            //Note: We need to call this in order to get around a bug in Raknet (crashes if we don't call.) If we manually set the NetworkIDManager right after creating the replica, it will work.
            //Instead, let's just call disable replica interface. One can call this function elsewhere after the composite object has been created. It won't affect anything.
            replicaManager->DisableReplicaInterfaces(znetEntityShrPtr.get(),
            REPLICA_RECEIVE_DESTRUCTION | REPLICA_RECEIVE_SCOPE_CHANGE);
            NetworkID nid;
            nid = znetEntityShrPtr->GetNetworkID();
            ostringstream oss;
            oss << "Entity" << nid.localSystemAddress;
            entPtr->setEntityName(oss.str());
            }
            else
            {
            replicaManager->DisableReplicaInterfaces(znetEntityShrPtr.get(), REPLICA_SEND_CONSTRUCTION | REPLICA_SEND_DESTRUCTION | REPLICA_SEND_SCOPE_CHANGE);
            znetEntityShrPtr->SetNetworkID(*netId);
            ostringstream oss;
            oss << "Entity" << netId->localSystemAddress;
            entPtr->setEntityName(oss.str());

            }
            //////////////We need to setup server/client specific entity aspects.//////////////////
            Entities::EntityAspects entAspects;
            Entities::bindEntityAspects(entAspects,*entPtr,isServer);
            if(isServer)
            entPtr->onInitServer();
            else
            entPtr->onInitClient();
            znetEntityShrPtr->setEntityAspects(entAspects); //DO NOT forget to set, else exception will be thrown.
            */
        }
    }
}
#endif
