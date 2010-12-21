/** \file This file defines the controller for the server.
* ServerController.h
*
* Created on: May 21, 2008
* Author: bey0nd
*
* 
*
*
**/

#ifndef _SERVERCONTROLLER_H_
#define _SERVERCONTROLLER_H_

//#include <string>
#include <memory>
#include <vector>
#include <Ogre.h> //Note: We want to use Ogre, such as it's vector etc, but for server there is no need for a screen, just the root will be fine.

#include "MainController.h"
#include "net/NetServerController.h"

namespace ZGame
{
    class ServerController : public MainController
    {
    public:
        ServerController();
        virtual
            ~ServerController();
        bool
            loadStartStates() {return true;}
        //implements virtual method from MainController
        bool onInit();
        void run();
        void onDestroy();

    private:
        std::auto_ptr<Ogre::Root> _root;
        ZGame::Networking::NetServerController _serverController;
        //std::auto_ptr<RakPeerInterface> peer;
        //bool initServer();
        //bool shutDownServer();
        //bool handlePacket(); //initial implementing handle packets.

        //unsigned char getPacketIdentifer(Packet* p);
        void printPacketId(unsigned char id);
    };
}


#endif