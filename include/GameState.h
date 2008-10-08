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
  class LFCObsInjector; //interface
  class KeyEvtObsInjector;
  class MouseEvtObsInjector;
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
    addLfcObsInjector(
        const boost::shared_ptr<LFCObsInjector> &injector);
    void
    addKeyObsInjector(
        const boost::shared_ptr<KeyEvtObsInjector> &injector);
    void
    addMouseObsInjector(
        const boost::shared_ptr<MouseEvtObsInjector> &injector);
  private:
    typedef vector<boost::shared_ptr<LFCObsInjector> >::iterator LfcInjIter;
    vector<boost::shared_ptr<LFCObsInjector> > _lfcObsInj;
    typedef vector<boost::shared_ptr<KeyEvtObsInjector> >::iterator KeyInjIter;
    vector<boost::shared_ptr<KeyEvtObsInjector> > _keyObsInj;
    typedef vector<boost::shared_ptr<MouseEvtObsInjector> >::iterator MouseInjIter;
    vector<boost::shared_ptr<MouseEvtObsInjector> > _mouseObsInj;

    void
    registerObs(LifeCycleRegister &lfcReg, KeyEventRegister &keyReg,
        MouseEventRegister &mouseReg);

  };

}

#endif /* GAMESTATE_H_ */
