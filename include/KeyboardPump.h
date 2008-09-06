/*
 * KeyboardPump.h
 *
 *  Created on: Sep 5, 2008
 *      Author: bey0nd
 */

#ifndef KEYBOARDPUMP_H_
#define KEYBOARDPUMP_H_

#include "EventDelegates.h"

namespace ZGame
{
  class KeyboardPump
  {
  public:
    KeyboardPump();
    ~KeyboardPump();

    void addKeyboardObserver(ZGame::EVENT::KeyboardEvtObserver obs);
    void updateKeyUpObs(const OIS::KeyEvent &evt);
    void updateKeyDownObs(const OIS::KeyEvent &evt);
    void removeAllObs();
  protected:
    typedef vector<ZGame::EVENT::ZKeyDownEvt>::iterator KeyDownObsItr;
    typedef vector<ZGame::EVENT::ZKeyUpEvt>::iterator KeyUpObsItr;
    vector<ZGame::EVENT::ZKeyDownEvt> _onKeyDownObs;
    vector<ZGame::EVENT::ZKeyUpEvt> _onKeyUpObs;

  };
}

#endif /* KEYBOARDPUMP_H_ */
