/**
* ServerController.h

* Created on: May 21, 2008
* Author: bey0nd

* This file defines the controller for the server.
*
*
**/

#ifndef _SERVERCONTROLLER_H_
#define _SERVERCONTROLLER_H_

//#include <string>
#include <memory>
#include <vector>
#include <Ogre.h> //Note: We want to use Ogre, such as it's vector etc, but for server there is no need for a screen, just the root will be fine.

#include "RakNetworkFactory.h"
#include "RakPeerInterface.h"
#include "MainController.h"

namespace ZGame
{
    class ServerController : public MainController
    {
    public:
        ServerController();
        virtual
            ~ServerController();

        //implements virtual method from MainController
        bool onInit();
        void run();
        void onDestroy();

    private:
        std::auto_ptr<Ogre::Root> _root;
        std::auto_ptr<RakPeerInterface> peer;
        bool initServer();
        bool shutDownServer();
        bool handlePacket(); //initial implementing handle packets.

        unsigned char getPacketIdentifer(Packet* p);
        void printPacketId(unsigned char id);
    };
}


#endif