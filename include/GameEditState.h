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

namespace ZGame
{
class GameEditView;
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
  GameEditView* _editView;

  virtual void regLfcObsForInjection();
  virtual void regKeyObsForInjection();
};
}
#endif /* GAMEEDITSTATE_H_ */
