/*
 * GameState.h
 *
 *  Created on: Aug 28, 2008
 *      Author: bey0nd
 */

#ifndef GAMESTATE_H_
#define GAMESTATE_H_

#include <vector>

using namespace std;

#include "LifeCycleDelegates.h"
#include "EventDelegates.h"

namespace ZGame
{

  class GameState
  {
  public:
    virtual
    ~GameState();

    void injectLifeCycleSubject(ZGame::LifeCycle::LifeCycleSubject &subject);
    void injectKeyEvtSubject(ZGame::EVENT::KeyEvtSubject &subject);
    virtual void initialize() = 0;

  protected:
    typedef vector<ZGame::LifeCycle::LifeCycleSubjectInjector>::iterator SubjectInjectorsItr;
    typedef vector<ZGame::EVENT::KeyEvtSubjectInjector>::iterator KeyEvtSubInjectorItr;
    vector<ZGame::LifeCycle::LifeCycleSubjectInjector> _subjectInjectors;
    vector<ZGame::EVENT::KeyEvtSubjectInjector> _keyEvtSubjectInjectors;

    GameState(); //protected constructor

    void addLifeCycleSubjectInjector(ZGame::LifeCycle::LifeCycleSubjectInjector &injector);
    void addKeySubjectInjector(ZGame::EVENT::KeyEvtSubjectInjector &injector);

  };

}

#endif /* GAMESTATE_H_ */
