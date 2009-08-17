#include "NetworkClientState.h"
#include "LifeCycleRegister.h"

namespace ZGame
{
  namespace Networking
  {
    NetworkClientState::NetworkClientState() : _isConnected(false)
    {
    }

    NetworkClientState::~NetworkClientState()
    {
    }

    void NetworkClientState::init()
    {
      if(!initClient())
      {
        cout << "initializing network client failed!." << endl;
      }
    }

    void NetworkClientState::regLfcObs(LifeCycleRegister &lfcReg)
    {
      LifeCycle::LifeCycleObserver lfcObs;
      lfcObs.onInit.bind(&NetworkClientState::onInit, this);
      lfcObs.onUpdate.bind(&NetworkClientState::onUpdate,this);
      lfcObs.onDestroy.bind(&NetworkClientState:;onDestroy,this);
    }

    bool NetworkClientState::initClient()
    {
      _peer.reset(RakNetworkFactory::GetRakPeerInterface());
      if(_peer.get() == 0)
        return false;
      peer->Startup(1,30,&SocketDescriptor(),1);
      return true;
    }

    void NetworkClientState::connect()
    {
      if(_isConnected)
        return;
      _isConnected = true;
      cout << "Trying to connect. " << endl;
      peer->Connect("127.0.0.1",6666,0,0);
    }


  }
}
