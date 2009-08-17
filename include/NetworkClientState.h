#ifndef _NETWORK_CLIENT_STATE_H
#define _NETWORK_CLIENT_STATE_H
#include "RakNetworkFactory.h"
#include "RakPeerInterface.h"

namespace ZGame
{
  class LifeCycleRegister;

  namespace Networking
  {
  class NetworkClientState
  {
  public:
    NetworkClientState();
    virtual ~NetworkClientState();

    void
    init();

    //LifeCycle functions
    bool 
    onInit();
    bool onUpdate(const Ogre::FrameEvent &evt);
    bool onDestroy();

    void
    regLfcObs(LifeCycleRegister &lfcReg);
  protected:

  private:

    std::auto_ptr<RakPeerInterface> _peer;
    bool _isConnected;

    bool
    initClient();
    bool
    connect();
    bool
    disconnect();

    void handlePacket();

    unsigned char getPacketIdentifier(Packet* p);
    void printPacketId(unsigned char id);


    

  };
  }
}
#endif