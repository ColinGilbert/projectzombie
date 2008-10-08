/*
 * GameState.cpp
 *
 *  Created on: Aug 28, 2008
 *      Author: bey0nd
 */

#include <Ogre.h>
#include "GameState.h"
#include "LifeCycleRegister.h"
#include "KeyEventRegister.h"
#include "MouseEventRegister.h"
#include "LFCObsInjector.h"
#include "LifeCycleDelegates.h"
#include "DelegatesUtil.h"
namespace ZGame
{

  GameState::GameState()
  {
    // TODO Auto-generated constructor stub
  }

  GameState::~GameState()
  {
    // TODO Auto-generated destructor stub
  }

  void
  GameState::init(LifeCycleRegister &lfcReg, KeyEventRegister &keyReg,
      MouseEventRegister &mouseReg)
  {
    regLfcObsForInjection(lfcReg); //register life cycle observers for injection.
    registerLfcObs(lfcReg);
    regKeyObsForInjection(keyReg); //register key observers for injection.
    regMouseObsForInjection(mouseReg);
  }

  void
  GameState::addLfcObsInjector(
      const boost::shared_ptr<LifeCycle::LFCObsInjector> &injector)
  {
    _lfcObsInjectors.push_back(injector);
  }

  void
  GameState::registerLfcObs(LifeCycleRegister &lfcReg)
  {
    InjIter it(_lfcObsInjectors.begin());
    InjIter end(_lfcObsInjectors.end());
    for (; it != end; ++it)
      {
        LifeCycle::LifeCycleObserver obs;
        (*it)->fillLfcObservers(obs);
        //it->fillLfcObservers(obs);
        lfcReg.registerLfcObs(obs);
        LifeCycle::clearLfcObs(obs);
      }
    _lfcObsInjectors.clear(); //we done with injection, clear memory.
  }

}
