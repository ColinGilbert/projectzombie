/*
 * MouseEventRegister.h
 *
 *  Created on: Sep 29, 2008
 *      Author: bey0nd
 */

#ifndef MOUSEEVENTREGISTER_H_
#define MOUSEEVENTREGISTER_H_
#include "ZPrerequisites.h"
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
    void registerMouseObs(EVENT::MouseEvtObserver &obs, REGISTER_ORDER order);
  protected:
	typedef std::list<EVENT::MouseEvtObserver>::iterator ObsItr;
	std::list<EVENT::MouseEvtObserver> _obs;

  };

}

#endif /* MOUSEEVENTREGISTER_H_ */
