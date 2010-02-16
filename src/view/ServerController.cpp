#include <iostream>

#include "ServerController.h"
#include "MessageIdentifiers.h"


namespace ZGame
{
  ServerController::ServerController()
  {
  }

  ServerController::~ServerController()
  {

  }

  bool
  ServerController::onInit()
  {
    using namespace Ogre;
    _root.reset(new Ogre::Root("plugins.cfg","pchaos_server_display.cfg","Pchaos_server.log"));
    if(_root->showConfigDialog())
    {
      _root->initialise(false);
    }
    else
      return false;

    Ogre::LogManager::getSingleton().setLogDetail(Ogre::LL_BOREME);


    if(!initServer())
      return false;

    std::cout << "Starting server!" << std::endl;
    const int MAX_CONNECT = 30;
    const int SERVER_PORT = 6666;
    peer->Startup(MAX_CONNECT,30,&SocketDescriptor(SERVER_PORT,0),1);
    peer->SetMaximumIncomingConnections(30);

    return true;

  }

  void
  ServerController::run()
  {
    //Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL,"In ServerController run.");
    while(1) //run forever.
    {
      handlePacket();
    }
  }

  bool
    ServerController::handlePacket()
  {
    unsigned char packetId;

    Packet* packet = peer->Receive();
  
    while(packet)
    {
    if(packet)
    {
      packetId = getPacketIdentifer(packet);
      printPacketId(packetId);
      peer->DeallocatePacket(packet);
     
    }
    packet = peer->Receive();

    }
    
    return true;
  }

  void
    ServerController::printPacketId(unsigned char id)
  {
    using namespace std;
    switch(id)
     {
      case ID_REMOTE_DISCONNECTION_NOTIFICATION:
        cout << "Another client has disconnected" << endl;
        break;
      case ID_REMOTE_CONNECTION_LOST:
        cout << "Another client has lost connection" << endl;
        break;
  
      case ID_REMOTE_NEW_INCOMING_CONNECTION:
        cout << "Another client has connected" << endl;

        break;
      case ID_CONNECTION_REQUEST_ACCEPTED:
        cout << "Client request accepted" << endl;
        break;

      case ID_NEW_INCOMING_CONNECTION:
        cout << "A connection is incoming." << endl;
        break;

      case ID_DISCONNECTION_NOTIFICATION:
        cout << "A client has disconnected" << endl;
        break;

      case ID_CONNECTION_LOST:

        cout << "A client has lost connect." << endl;
        break;
      default:
        break;
    }
    
  }

  unsigned char 
    ServerController::getPacketIdentifer(Packet* p)
  {
    if((unsigned char)p->data[0] == ID_TIMESTAMP)
      return (unsigned char)p->data[sizeof(unsigned char)+sizeof(unsigned long)];
    else
      return (unsigned char)p->data[0];
  }





  void
  ServerController::onDestroy()
  {
    shutDownServer();
    Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL,"Shutting down server.");
  }

  /**
  *This method will initilize the server, by handling the apporiate 
  *Raknet startup sequences.
  *
  **/
  bool
  ServerController::initServer()
  {
    using namespace std;
    cout << "Server initialized!" << endl;
    peer.reset(RakNetworkFactory::GetRakPeerInterface());
    if(peer.get() != 0)
      return true;
    return false;
  }

  /**
  *This method will handle the RakNet shutdown sequences.
  **/
  bool
  ServerController::shutDownServer()
  {
    using namespace std;
    cout << "Shutting down server!" << endl;
    peer->Shutdown(300);
    RakNetworkFactory::DestroyRakPeerInterface(peer.release());
    return true;
  }

  
}
