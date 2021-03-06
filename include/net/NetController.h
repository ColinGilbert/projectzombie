/** \file
* \brief Contains networking based controllers and related class.
*
*/

#ifndef _ZGAME_NETCONTROLLER_H
#define _ZGAME_NETCONTROLLER_H

#include <vector>
//#include <tr1/unordered_map>




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

namespace ZGame
{
    namespace Networking
    {
       

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

	    /** \brief The RakPeer interface. */
            RakPeerInterface* _rakPeer;


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
