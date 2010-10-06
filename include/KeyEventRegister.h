/*
 * KeyEventRegister.h
 *
 *  Created on: Sep 27, 2008
 *      Author: bey0nd
 */

#ifndef KEYEVENTREGISTER_H_
#define KEYEVENTREGISTER_H_
#include <memory>
#include <vector>
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
    void registerKeyObs(EVENT::KeyboardEvtObserver& obs); //utility method for register key observers.
  protected:
	typedef std::vector<EVENT::KeyboardEvtObserver>::iterator KeyObsIt;
	std::vector<EVENT::KeyboardEvtObserver> _keyObs;
    bool _registered;
  };

}

#endif /* KEYEVENTREGISTER_H_ */
