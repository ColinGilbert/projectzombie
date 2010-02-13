#ifndef _ZGAME_NETCONTROLLER_HEADER
#define _ZGAME_NETCONTROLLER_HEADER

#include <Ogre.h>

#include <RakNetworkFactory.h>
#include <RakPeerInterface.h>

namespace ZGame
{
  namespace Net
  {
    class NetController 
    {
    public:
      virtual
        ~NetController(){}
    protected:
      NetController(){}

      unsigned char getPacketIdentifer(Packet* p);
      void printPacketId(unsigned char id);


    }
  }
}



#endif