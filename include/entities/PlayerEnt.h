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

            //PlayerEntity(ReplicaManager &replica);

            //Ogre::String getTypename(){}
            /** \brief This function is called once to initialize for client.*/
            virtual bool 
                onInitClient();
            /** \brief This function is called once to initialize for server.*/
            virtual bool 
                onInitServer();
            /** \brief This function is called during send construction to remote systems.*/
            virtual bool
                onSendConstruction(Ogre::String &typeStr, RakNet::BitStream* outBitStream);
            /** \brief This function is called on read events. */
            virtual bool
                onRead(Ogre::Vector3 &pos, Ogre::Quaternion &orient);
            /** \brief This function is called on Write events. */
            virtual bool
                onWrite(Ogre::Vector3 &pos, Ogre::Quaternion &orient);

        private:
            //ReplicaManager _replicaManager;
            //Ogre::String _typeName; //the type of this entity;
        };
    }
}

#endif