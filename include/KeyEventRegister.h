/*
* KeyEventRegister.h
*
*  Created on: Sep 27, 2008
*      Author: bey0nd
*/

#ifndef KEYEVENTREGISTER_H_
#define KEYEVENTREGISTER_H_
#include "ZPrerequisites.h"
#include "EventDelegates.h"

namespace ZGame
{


    class KeyEventRegister
    {
    public:
       
        KeyEventRegister();
        virtual
            ~KeyEventRegister();

        void injectKeySubj(const EVENT::KeyEvtSubject &subj);
        void registerKeyObs(EVENT::KeyboardEvtObserver& obs,
            REGISTER_ORDER order); //utility method for register key observers.
    protected:
        typedef std::list<EVENT::KeyboardEvtObserver>::iterator KeyObsIt;
        std::list<EVENT::KeyboardEvtObserver> _keyObs;
        bool _registered;
    };

}

#endif /* KEYEVENTREGISTER_H_ */
