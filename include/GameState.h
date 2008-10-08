/*
 * GameState.h
 *
 *  Created on: Aug 28, 2008
 *      Author: bey0nd
 */

#ifndef GAMESTATE_H_
#define GAMESTATE_H_

#include <vector>
#include <boost/shared_ptr.hpp>
using namespace std;

namespace ZGame
{
  class LifeCycleRegister;
  class KeyEventRegister;
  class MouseEventRegister;
  namespace LifeCycle
  {
    class LFCObsInjector; //interface
  }
  class GameState
  {
  public:
    virtual
    ~GameState();
    void
    init(LifeCycleRegister &lfcReg, KeyEventRegister &keyReg,
        MouseEventRegister &mouseReg);
  protected:

    GameState(); //protected constructor
    virtual void
    regLfcObsForInjection(LifeCycleRegister &lfcReg)
    {
    }
    virtual void
    regKeyObsForInjection(KeyEventRegister &keyReg)
    {
    }
    virtual void
    regMouseObsForInjection(MouseEventRegister &mouseReg)
    {
    }

    void
    addLfcObsInjector(const boost::shared_ptr<LifeCycle::LFCObsInjector> &injector);
  private:
    typedef vector<boost::shared_ptr<LifeCycle::LFCObsInjector> >::iterator InjIter;
    vector<boost::shared_ptr<LifeCycle::LFCObsInjector> > _lfcObsInjectors;

    void
    registerLfcObs(LifeCycleRegister &lfcReg);

  };

}

#endif /* GAMESTATE_H_ */
