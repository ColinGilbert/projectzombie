/*
 * MouseEventRegister.h
 *
 *  Created on: Sep 29, 2008
 *      Author: bey0nd
 */

#ifndef MOUSEEVENTREGISTER_H_
#define MOUSEEVENTREGISTER_H_
#include <memory>
using namespace std;
#include "EventDelegates.h"
namespace ZGame
{

  class MouseEventRegister
  {
  public:
    MouseEventRegister();
    virtual
    ~MouseEventRegister();

    void injectMouseSubj(const EVENT::MouseEvtSubject &subj);
    void registerMouseObs(const EVENT::MouseEvtObserver &obs);
  protected:
    typedef vector<EVENT::MouseEvtObserver>::iterator ObsItr;
    vector<EVENT::MouseEvtObserver> _obs;

  };

}

#endif /* MOUSEEVENTREGISTER_H_ */
