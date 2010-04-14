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

        void addKeyboardObserver(const ZGame::EVENT::KeyboardEvtObserver &obs);
        void updateKeyUpObs(const OIS::KeyEvent &evt);
        void updateKeyDownObs(const OIS::KeyEvent &evt);
        void removeAllObs();
    protected:
        typedef std::vector<ZGame::EVENT::ZKeyDownEvt>::iterator KeyDownObsItr;
        typedef std::vector<ZGame::EVENT::ZKeyUpEvt>::iterator KeyUpObsItr;
        std::vector<ZGame::EVENT::ZKeyDownEvt> _onKeyDownObs;
        std::vector<ZGame::EVENT::ZKeyUpEvt> _onKeyUpObs;

    };
}

#endif /* KEYBOARDPUMP_H_ */
