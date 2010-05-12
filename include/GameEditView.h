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

namespace ZGame
{
  class ImposterGen;
  class Imposter;
  class ImposterView;
  namespace Entities
  {
  class ZEntity;
  }

  namespace LifeCycle
  {
    struct LifeCycleObserver;
  }
  namespace EVENT
  {
    struct KeyboardEvtObserver;
    struct MouseEvtObserver;
  }

  class GameEditView 
  {
  public:
    GameEditView();
    ~GameEditView();

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
    Entities::ZEntity* _ent;
  };
}

#endif /* GAMEEDITVIEW_H_ */
