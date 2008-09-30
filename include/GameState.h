/*
 * GameState.h
 *
 *  Created on: Aug 28, 2008
 *      Author: bey0nd
 */

#ifndef GAMESTATE_H_
#define GAMESTATE_H_

#include <vector>

using namespace std;

namespace ZGame
{
  class LifeCycleRegister;
  class KeyEventRegister;
  class MouseEventRegister;
  class GameState
  {
  public:
    virtual
    ~GameState();
    void init(LifeCycleRegister &lfcReg, KeyEventRegister &keyReg, MouseEventRegister &mouseReg);
  protected:
    GameState(); //protected constructor
    virtual void regLfcObsForInjection(LifeCycleRegister &lfcReg){}
    virtual void regKeyObsForInjection(KeyEventRegister &keyReg){}
    virtual void regMouseObsForInjection(MouseEventRegister &mouseReg){}
  };

}

#endif /* GAMESTATE_H_ */
