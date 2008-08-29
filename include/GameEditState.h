/*
 * GameEditState.h
 *
 *  Created on: Aug 28, 2008
 *      Author: bey0nd
 */

#ifndef GAMEEDITSTATE_H_
#define GAMEEDITSTATE_H_

#include "GameState.h"

namespace ZGame
{

class GameEditState : public ZGame::GameState
{
public:
  GameEditState();
  virtual
  ~GameEditState();

  virtual void initialize();
  void injectLifeCycleSubject(ZGame::LifeCycle::LifeCycleSubject &subject);

  //life cycle methods
  bool onUpdate();
  bool onInit();
  bool onDestroy();


};
}
#endif /* GAMEEDITSTATE_H_ */
