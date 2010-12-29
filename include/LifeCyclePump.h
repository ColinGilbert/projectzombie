#ifndef LIFECYCLEPUMP_H
#define LIFECYCLEPUMP_H

/**
* This class defines a subject/observer 'pump.' It's job is to pump lifecycle events to lifecycle observers.
*/
#include <vector>
using namespace std;
#include <Ogre.h>
#include "LifeCycleDelegates.h"
#include "ZInitPacket.h"
namespace ZGame
{
    
    class LifeCyclePump
    {
    public:
        LifeCyclePump();
        ~LifeCyclePump();

        void addLifeCycleObserver(const ZGame::LifeCycle::LifeCycleObserver &obs);

        //life cycle updates
        void updateOnItObs(ZGame::ZInitPacket *initPacket);
        void updateOnUpdateObs(const Ogre::FrameEvent& evt);
        void updateOnDestroyObs();
        void updateOnRenderQueueStartObs(Ogre::uint8 queueGroupId, 
            const Ogre::String& invocation, bool& skipThisInvocation);
        void updateOnRenderQueueEndObs(Ogre::uint8 queueGroupId, 
            const Ogre::String& invocation, bool& skipThisInvocation);
        void removeAllObs();

    protected:
        //life cycle
        typedef std::vector<ZGame::LifeCycle::LifeCycleEvent>::iterator LifeCycleObsItr;
        typedef std::vector<ZGame::LifeCycle::LifeCycleEvent2>::iterator LifeUpdateObsItr;
        typedef std::vector<ZGame::LifeCycle::LifeCycleRdrQueueStart>::iterator LifeRdrQueueObsItr;
        typedef std::vector<ZGame::LifeCycle::LifeCycleOnInit>::iterator LifeCycleOnInitItr;;
        std::vector<ZGame::LifeCycle::LifeCycleOnInit> _onInitObs; 
        std::vector<ZGame::LifeCycle::LifeCycleEvent2> _onUpdateObs; 
        std::vector<ZGame::LifeCycle::LifeCycleEvent> _onDestroyObs; 
        std::vector<ZGame::LifeCycle::LifeCycleRdrQueueStart> _onRenderQueueStartObs;
        std::vector<ZGame::LifeCycle::LifeCycleRdrQueueStart> _onRenderQueueEndObs;


    };

}

#endif
