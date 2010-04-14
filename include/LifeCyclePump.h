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

        void addLifeCycleObserver(const ZGame::LifeCycle::LifeCycleObserver &obs);

        //life cycle updates
        void updateOnItObs();
        void updateOnUpdateObs(const Ogre::FrameEvent& evt);
        void updateOnDestroyObs();
        void removeAllObs();

    protected:
        //life cycle
        typedef std::vector<ZGame::LifeCycle::LifeCycleEvent>::iterator LifeCycleObsItr;
        typedef std::vector<ZGame::LifeCycle::LifeCycleEvent2>::iterator LifeUpdateObsItr;
        std::vector<ZGame::LifeCycle::LifeCycleEvent> _onInitObs; //on init observers
        std::vector<ZGame::LifeCycle::LifeCycleEvent2> _onUpdateObs; //on update observers
        std::vector<ZGame::LifeCycle::LifeCycleEvent> _onDestroyObs; //on destroy observers



    };

}

#endif
