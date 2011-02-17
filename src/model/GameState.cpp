/*
* GameState.cpp
*
*  Created on: Aug 28, 2008
*      Author: bey0nd
*/

#include <Ogre.h>
#include <SdkTrays.h>
#include "GameState.h"

using namespace std;

namespace ZGame
{

    GameState::GameState()
    {
        // TODO Auto-generated constructor stub
    }

    GameState::~GameState()
    {
        cout << "In gamestate destrutor." << endl;
        // TODO Auto-generated destructor stub
    }

    void
        GameState::init(LifeCycleRegister &lfcReg, KeyEventRegister &keyReg,
        MouseEventRegister &mouseReg, OgreBites::SdkTrayManager* tray)
    {
        regLfcObsForInjection(lfcReg); //register life cycle observers for injection.
        regKeyObsForInjection(keyReg); //register key observers for injection.
        regMouseObsForInjection(mouseReg);
    }

    void
        GameState::regLfcObsForInjection(LifeCycleRegister &lfcReg)
    {
    }
    void
        GameState::regKeyObsForInjection(KeyEventRegister &keyReg)
    {
    }
    void
        GameState::regMouseObsForInjection(MouseEventRegister &mouseReg)
    {
    }


}
