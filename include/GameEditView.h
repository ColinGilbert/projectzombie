/*
 * GameEditView.h
 *
 *  Created on: Sep 4, 2008
 *      Author: bey0nd
 */

#ifndef GAMEEDITVIEW_H_
#define GAMEEDITVIEW_H_

#include "LifeCycleDelegates.h"
#include "EventDelegates.h"

namespace ZGame
{
  class ImposterGen;
  class Imposter;
  class ImposterView;
  class GameEditView
  {
  public:
    GameEditView();
    ~GameEditView();

    void injectLifeCycleSubject(ZGame::LifeCycle::LifeCycleSubject &subject);
    void injectKeyEvtSubject(ZGame::EVENT::KeyEvtSubject &subject);

    void fillKeySubjectInjector(EVENT::KeyEvtSubjectInjector &injector); //fill out the detail of key subject injector
    void fillLifeCycleSubjectInjector(LifeCycle::LifeCycleSubjectInjector &injector);

    //life cycle methods
    bool onUpdate(const Ogre::FrameEvent& evt);
    bool onInit();
    bool onDestroy();
    //control event methods
    //keyboard
    bool onKeyUp(const OIS::KeyEvent &evt);
    bool onKeyDown(const OIS::KeyEvent &evt);

  protected:
    ImposterGen* _imposterGen;
    Imposter* _imposter;
    ImposterView* _imposterView;
    Ogre::Real _dz;
  };
}

#endif /* GAMEEDITVIEW_H_ */
