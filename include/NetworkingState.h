#ifndef _NETWORKING_STATE_H_
#define _NETWORKING_STATE_H_
#include <string>
#include <memory>
#include <vector>
#include <Ogre.h>

#include "RakNetworkFactory.h"
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"

namespace ZGame
{
  namespace Networking
  {
  class NetworkingState
  {
  public:
    NetworkingState(){}
    virtual
      ~NetworkingState() = 0; //pure virtual

  protected:
    std::auto_ptr<RakPeerInterface> peer;

    virtual bool initNetwork() = 0; //pure virtual
    virtual bool shutDownNetwork() = 0; //pure virtual
    virtual void handlePacket() = 0;
    void printPacketId(unsigned char packetId);
    

    
  };
  }
}



#endif