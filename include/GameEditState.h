/*
 * GameEditState.h
 *
 *  Created on: Aug 28, 2008
 *      Author: bey0nd
 */

#ifndef GAMEEDITSTATE_H_
#define GAMEEDITSTATE_H_

#include <memory>
using namespace std;
#include <Ogre.h>
#include <OIS/OIS.h>
#include "GameState.h"


namespace ZGame
{
class GameEditView;
class LifeCycleRegister;
class KeyEventRegister;
class GameEditState : public ZGame::GameState
{
public:
  GameEditState();
  virtual
  ~GameEditState();

   //life cycle methods
  bool onUpdate(const Ogre::FrameEvent& evt);
  bool onInit();
  bool onDestroy();

  //control methods
  bool onKeyUp(const OIS::KeyEvent &evt);
  bool onKeyDown(const OIS::KeyEvent &evt);

protected:
  auto_ptr<GameEditView> _editView;

  virtual void regLfcObsForInjection(LifeCycleRegister &lfcReg);
  virtual void regKeyObsForInjection(KeyEventRegister &keyReg);
};
}
#endif /* GAMEEDITSTATE_H_ */
