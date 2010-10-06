/*
 * MouseEventRegister.h
 *
 *  Created on: Sep 29, 2008
 *      Author: bey0nd
 */

#ifndef MOUSEEVENTREGISTER_H_
#define MOUSEEVENTREGISTER_H_
#include <memory>
#include <vector>
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
    void registerMouseObs(EVENT::MouseEvtObserver &obs);
  protected:
	typedef std::vector<EVENT::MouseEvtObserver>::iterator ObsItr;
	std::vector<EVENT::MouseEvtObserver> _obs;

  };

}

#endif /* MOUSEEVENTREGISTER_H_ */
