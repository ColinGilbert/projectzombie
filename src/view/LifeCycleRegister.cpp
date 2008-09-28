/*
 * LifeCycleRegister.cpp
 *
 *  Created on: Sep 27, 2008
 *      Author: bey0nd
 */

#include <stdexcept>
#include "LifeCycleRegister.h"

namespace ZGame
{

  LifeCycleRegister::LifeCycleRegister() : _isRegistered(false)
  {
    // TODO Auto-generated constructor stub

  }

  LifeCycleRegister::~LifeCycleRegister()
  {
    // TODO Auto-generated destructor stub
  }

  void LifeCycleRegister::injectLfcSubj(const LifeCycle::LifeCycleSubject &subj)
  {
    if(!_isRegistered)
      throw std::invalid_argument("You are trying to inject life cycle observer to subject without registering said observer.");
    for(LfcObsIt it=_lfcObs.begin();it != _lfcObs.end(); ++it)
      {
        subj(*it);
      }
  }

  void LifeCycleRegister::registerLfcObs(const LifeCycle::LifeCycleObserver& obs)
  {
    _isRegistered = true;
    _lfcObs.push_back(obs);
  }

}
