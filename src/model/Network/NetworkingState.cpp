#include "NetworkingState.h"
#include "MessageIdentifiers.h"

namespace ZGame
{
  namespace Networking
  {
    void NetworkingState::printPacketId(unsigned char packetId)
    {
    
      using namespace std;
      switch(packetId)
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
        //cout << "Got an id: " << id << endl;
        break;
      }

    }
  }
}