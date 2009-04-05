/*
 * LifeCycleRegister.h
 *
 *  Created on: Sep 27, 2008
 *      Author: bey0nd
 */

#ifndef LIFECYCLEREGISTER_H_
#define LIFECYCLEREGISTER_H_

#include <memory>
#include <vector>
#include "LifeCycleDelegates.h"


namespace ZGame
{
  
  class LifeCycleRegister
  {
  public:
    LifeCycleRegister();
    virtual
    ~LifeCycleRegister();

    void injectLfcSubj(const LifeCycle::LifeCycleSubject &subj);
    void registerLfcObs(const LifeCycle::LifeCycleObserver& obs); //helper function to do the actual registration.

  protected:

    
	typedef std::vector<LifeCycle::LifeCycleObserver>::iterator LfcObsIt;
	std::vector<LifeCycle::LifeCycleObserver> _lfcObs;
    bool _isRegistered;

  };

}

#endif /* LIFECYCLEREGISTER_H_ */
