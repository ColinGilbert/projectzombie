#ifndef _NETWORK_CLIENT_STATE_H #define _NETWORK_CLIENT_STATE_H #include <Ogre.h>  #include "NetController.h" #include "EventDelegates.h" #include "LifeCycleDelegates.h" #include "Controller.h"  namespace ZGame {     namespace Networking     {         class NetworkClientController : public NetController, public Controller         {         public:             NetworkClientController();             virtual ~NetworkClientController();              //implemeting methods from interface             virtual int                  execute(ZGame::Command) {return 0;}              bool                 connect();             bool                 disconnect();             void                  shutdown();              //LifeCycle functions             virtual bool                  onInit();             virtual bool                  onUpdate(const Ogre::FrameEvent &evt);             virtual bool                  onDestroy();             //void             //regLfcObs(LifeCycleRegister &lfcReg);         protected:             //unsigned char getPacketIdentifer(Packet* p);         private:             bool _isConnected;             SystemAddress _serverSysAddress;              bool                 initClient();              void handlePacket();              //unsigned char getPacketIdentifier(Packet* p);             void printPacketId(unsigned char id);         };     } } #endif