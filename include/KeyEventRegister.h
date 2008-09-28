/*
 * KeyEventRegister.h
 *
 *  Created on: Sep 27, 2008
 *      Author: bey0nd
 */

#ifndef KEYEVENTREGISTER_H_
#define KEYEVENTREGISTER_H_
#include <memory>
using namespace std;
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
    void registerKeyObs(const EVENT::KeyboardEvtObserver& obs); //utility method for register key observers.
  protected:
    typedef vector<EVENT::KeyboardEvtObserver>::iterator KeyObsIt;
    vector<EVENT::KeyboardEvtObserver> _keyObs;
    bool _registered;
  };

}

#endif /* KEYEVENTREGISTER_H_ */
