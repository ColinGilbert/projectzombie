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
#include "ObsInjectors.h"
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
    regKeyObsForInjection(keyReg); //register key observers for injection.
    regMouseObsForInjection(mouseReg);
    registerObs(lfcReg, keyReg, mouseReg);
  }

  void
  GameState::addLfcObsInjector(
      const boost::shared_ptr<LFCObsInjector> &injector)
  {
    _lfcObsInj.push_back(injector);
  }
  void
  GameState::addKeyObsInjector(
      const boost::shared_ptr<KeyEvtObsInjector> &injector)
  {
    _keyObsInj.push_back(injector);
  }
  void
  GameState::addMouseObsInjector(
      const boost::shared_ptr<MouseEvtObsInjector> &injector)
  {
    _mouseObsInj.push_back(injector);
  }

  void
  GameState::registerObs(LifeCycleRegister &lfcReg,
      KeyEventRegister &keyReg, MouseEventRegister &mouseReg)
  {
    //Life Cycle
    LfcInjIter it(_lfcObsInj.begin());
    LfcInjIter end(_lfcObsInj.end());
    for (; it != end; ++it)
      {
        LifeCycle::LifeCycleObserver obs;
        (*it)->fillLfcObservers(obs);
        //it->fillLfcObservers(obs);
        lfcReg.registerLfcObs(obs);
        LifeCycle::clearLfcObs(obs);
      }
    _lfcObsInj.clear(); //we done with injection, clear memory.

    //Key
    KeyInjIter itk(_keyObsInj.begin());
    KeyInjIter endk(_keyObsInj.end());
    for (; itk != endk; ++itk)
      {
        EVENT::KeyboardEvtObserver obs;
        (*itk)->fillKeyObservers(obs);
        keyReg.registerKeyObs(obs);
        EVENT::clearKeyObs(obs);
      }

    //mouse
    MouseInjIter itm(_mouseObsInj.begin());
    MouseInjIter endm(_mouseObsInj.end());
    for (; itm != endm; ++itm)
      {
        EVENT::MouseEvtObserver obs;
        (*itm)->fillMouseObservers(obs);
        mouseReg.registerMouseObs(obs);
        EVENT::clearMouseObs(obs);
      }
  }

}
