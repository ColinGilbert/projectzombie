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

#include "LifeCycleRegister.h"
#include "KeyEventRegister.h"
namespace ZGame
{

  class GameState
  {
  public:
    virtual
    ~GameState();
    void init();
    void cleanRegisters() //called when done with registers.
    {
      delete _lfcRegister;
      delete _keyRegister;
      _lfcRegister =0;
      _keyRegister = 0;
    }
    LifeCycleRegister* getLFCRegister(){ return _lfcRegister;}
    KeyEventRegister* getKeyRegister(){ return _keyRegister;}
  protected:
    GameState(); //protected constructor
    LifeCycleRegister* _lfcRegister;
    KeyEventRegister* _keyRegister;
    virtual void regLfcObsForInjection() = 0;
    virtual void regKeyObsForInjection() = 0;
  };

}

#endif /* GAMESTATE_H_ */
