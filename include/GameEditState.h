/*
 * GameEditState.h
 *
 *  Created on: Aug 28, 2008
 *      Author: bey0nd
 */

#ifndef GAMEEDITSTATE_H_
#define GAMEEDITSTATE_H_

#include <Ogre.h>

#include "GameState.h"
#include "GameEditView.h"
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
  void injectKeyEvtSubject(ZGame::EVENT::KeyEvtSubject &subject);

  //life cycle methods
  bool onUpdate();
  bool onInit();
  bool onDestroy();

  //control methods
  bool onKeyUp(const OIS::KeyEvent &evt);
  bool onKeyDown(const OIS::KeyEvent &evt);

protected:
  GameEditView _editView;

};
}
#endif /* GAMEEDITSTATE_H_ */
