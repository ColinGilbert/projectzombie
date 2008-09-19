#ifndef LIFECYCLEPUMP_H
#define LIFECYCLEPUMP_H

/**
 * This class defines a subject/observer 'pump.' It's job is to pump lifecycle events to lifecycle observers.
 */
#include <vector>
using namespace std;
#include <Ogre.h>
#include "LifeCycleDelegates.h"

namespace ZGame
{
  class LifeCyclePump
  {
  public:
    LifeCyclePump();
    ~LifeCyclePump();

    void addLifeCycleObserver(ZGame::LifeCycle::LifeCycleObserver obs);

    //life cycle updates
    void updateOnItObs();
    void updateOnUpdateObs(const Ogre::FrameEvent& evt);
    void updateOnDestroyObs();
    void removeAllObs();

  protected:
    //life cycle
    typedef vector<ZGame::LifeCycle::LifeCycleEvent>::iterator LifeCycleObsItr;
    typedef vector<ZGame::LifeCycle::LifeCycleEvent2>::iterator LifeUpdateObsItr;
    vector<ZGame::LifeCycle::LifeCycleEvent> _onInitObs; //on init observers
    vector<ZGame::LifeCycle::LifeCycleEvent2> _onUpdateObs; //on update observers
    vector<ZGame::LifeCycle::LifeCycleEvent> _onDestroyObs; //on destroy observers



  };

}

#endif
