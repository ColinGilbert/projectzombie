/** \brief This file defines PlayerEntity.
*
*/
#ifndef _ZGAME_PLAYER_ENTITY_H
#define _ZGAME_PLAYER_ENTITY_H

#include <Ogre.h>

#include "ZEntity.h"
#include "ReplicaManager.h"
#include "net/ZNetEntityTypes.h"

namespace ZGame
{
    namespace Entities
    {
        class PlayerEntity : public ZEntity
        {
        public:
            /** \brief Constructor*/
            PlayerEntity();
            virtual ~PlayerEntity();

            //PlayerEntity(ReplicaManager &replica);

            //Ogre::String getTypename(){}
            /** \brief This function is called once to initialize for client.*/
            bool 
                onInitClient();
            /** \brief This function is called once to initialize for server.*/
            bool 
                onInitServer();
            /** \brief This method is called once to do destruction for client. */
            bool
                onDestroyClient();
            /** \brief This method is called once to do destruction for server. */
            bool
                onDestroyServer();
            /** \brief This function is called during send construction to remote systems.*/
            bool
                onSendConstruction(Ogre::String &typeStr, RakNet::BitStream* outBitStream);
            /** \brief This function is called on read events. */
            bool
                onRead(Ogre::Vector3 &pos, Ogre::Quaternion &orient);
            /** \brief This function is called on Write events. */
            bool
                onWrite(Ogre::Vector3 &pos, Ogre::Quaternion &orient);

        private:
            //ReplicaManager _replicaManager;
            //Ogre::String _typeName; //the type of this entity;
            //void destroyNode();
        };
    }
}

#endif