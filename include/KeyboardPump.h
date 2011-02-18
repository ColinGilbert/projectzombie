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
        void addModifierStateObserver(const ZGame::EVENT::ZModifierStateEvt &obs);
        void updateKeyUpObs(const OIS::KeyEvent &evt);
        void updateKeyDownObs(const OIS::KeyEvent &evt);
        void updateModifierStateObs(const unsigned int modifierState);
        void removeAllObs();
    protected:
        typedef std::vector<ZGame::EVENT::ZKeyDownEvt>::iterator KeyDownObsItr;
        typedef std::vector<ZGame::EVENT::ZKeyUpEvt>::iterator KeyUpObsItr;
        typedef std::vector<ZGame::EVENT::ZModifierStateEvt>::iterator ModifierStateObsItr;
        std::vector<ZGame::EVENT::ZKeyDownEvt> _onKeyDownObs;
        std::vector<ZGame::EVENT::ZKeyUpEvt> _onKeyUpObs;
        std::vector<ZGame::EVENT::ZModifierStateEvt> _onModifierStateObs;

    };
}

#endif /* KEYBOARDPUMP_H_ */
