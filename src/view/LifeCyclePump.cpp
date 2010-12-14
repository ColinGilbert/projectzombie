/*
 * LifeCyclePump.cpp
 *
 *  Created on: Sep 5, 2008
 *      Author: bey0nd
 */
#include <iostream>
using namespace std;
#include "LifeCyclePump.h"

using namespace ZGame;

LifeCyclePump::LifeCyclePump()
{

}

LifeCyclePump::~LifeCyclePump()
{
  cout << "In LifeCyclePump::~LifeCyclePump()" << endl;
}

void
LifeCyclePump::addLifeCycleObserver(
    const ZGame::LifeCycle::LifeCycleObserver &obs)
{
  if (obs.onInit)
    _onInitObs.push_back(obs.onInit);
  if (obs.onUpdate)
    _onUpdateObs.push_back(obs.onUpdate);
  if (obs.onDestroy)
    _onDestroyObs.push_back(obs.onDestroy);
  if (obs.onRenderQueueStart)
      _onRenderQueueStartObs.push_back(obs.onRenderQueueStart);
}

/**
 * Update onInit() observers
 */
void
LifeCyclePump::updateOnItObs(ZGame::ZInitPacket initPacket)
{
  for (LifeCycleOnInitItr it = _onInitObs.begin(); it != _onInitObs.end(); ++it)
    {
      (*it)(initPacket);
    }
}
/**
 * Update onUpdate observers
 */
void
LifeCyclePump::updateOnUpdateObs(const Ogre::FrameEvent& evt)
{
  for (LifeUpdateObsItr it = _onUpdateObs.begin(); it != _onUpdateObs.end(); ++it)
    {
      //Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL,"upming updates to observers.");
      (*it)(evt); //make delegate call
    }
}
/**
 * Update onDestroy observers.
 */
void
LifeCyclePump::updateOnDestroyObs()
{
  for (LifeCycleObsItr it = _onDestroyObs.begin(); it != _onDestroyObs.end(); ++it)
    {
      (*it)(); //make delegate call
    }
}

void
    LifeCyclePump::updateOnRenderQueueStartObs(Ogre::uint8 queueGroupId,
    const Ogre::String& invocation, bool& skipThisInvocation)
{
    for(LifeRdrQueueStartObsItr it = _onRenderQueueStartObs.begin(); 
        it != _onRenderQueueStartObs.end(); ++it)
    {
        (*it)(queueGroupId, invocation, skipThisInvocation);
    }
}

void
LifeCyclePump::removeAllObs()
{
  _onInitObs.clear();
  _onUpdateObs.clear();
  _onDestroyObs.clear();
}
