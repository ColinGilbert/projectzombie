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

#include "LifeCycleDelegates.h"
#include "EventDelegates.h"
#include "LifeCycleRegister.h"
#include "KeyEventRegister.h"
namespace ZGame
{

  class GameState : public LifeCycleRegister, public KeyEventRegister
  {
  public:
    virtual
    ~GameState();
    void init();
  protected:
    GameState(); //protected constructor
  };

}

#endif /* GAMESTATE_H_ */
