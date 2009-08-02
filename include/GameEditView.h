/*
 * GameEditView.h
 *
 *  Created on: Sep 4, 2008
 *      Author: bey0nd
 */

#ifndef GAMEEDITVIEW_H_
#define GAMEEDITVIEW_H_

#include <memory>
#include <Ogre.h>
#include <OIS/OIS.h>
#include "ObsInjectors.h"

namespace ZGame
{
  class ImposterGen;
  class Imposter;
  class ImposterView;
  class ZEntity;

  namespace LifeCycle
  {
    struct LifeCycleObserver;
  }
  namespace EVENT
  {
    struct KeyboardEvtObserver;
    struct MouseEvtObserver;
  }

  class GameEditView : public LFCObsInjector, public KeyEvtObsInjector
  {
  public:
    GameEditView();
    ~GameEditView();

    //void injectLifeCycleSubject(const ZGame::LifeCycle::LifeCycleSubject &subject);
    //void injectKeyEvtSubject(const ZGame::EVENT::KeyEvtSubject &subject);

    //void fillKeySubjectInjector(EVENT::KeyEvtSubjectInjector &injector); //fill out the detail of key subject injector
    //void fillLifeCycleSubjectInjector(LifeCycle::LifeCycleSubjectInjector &injector);

    void
    fillLfcObservers(LifeCycle::LifeCycleObserver &obs);
    void
    fillKeyObservers(EVENT::KeyboardEvtObserver &obs);

    //life cycle methods
    bool onUpdate(const Ogre::FrameEvent& evt);
    bool onInit();
    bool onDestroy();
    //control event methods
    //keyboard
    bool onKeyUp(const OIS::KeyEvent &evt);
    bool onKeyDown(const OIS::KeyEvent &evt);

    
  protected:
    //ImposterGen* _imposterGen;
   // std::auto_ptr<Imposter> _imposter;
    Imposter* _imposter;
    Imposter* _fuckPoster;
    std::auto_ptr<ImposterView> _imposterView;
    Ogre::Real _dz;
    ZEntity* _ent;
  };
}

#endif /* GAMEEDITVIEW_H_ */
