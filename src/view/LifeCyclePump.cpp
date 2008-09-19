/*
 * LifeCyclePump.cpp
 *
 *  Created on: Sep 5, 2008
 *      Author: bey0nd
 */

#include "LifeCyclePump.h"

using namespace ZGame;

LifeCyclePump::LifeCyclePump()
{

}

LifeCyclePump::~LifeCyclePump()
{

}

void LifeCyclePump::addLifeCycleObserver(ZGame::LifeCycle::LifeCycleObserver obs)
{
  _onInitObs.push_back(obs.onInit);
  _onUpdateObs.push_back(obs.onUpdate);
  _onDestroyObs.push_back(obs.onDestroy);
}

/**
 * Update onInit() observers
 */
void
LifeCyclePump::updateOnItObs()
{
  for (LifeCycleObsItr it = _onInitObs.begin(); it != _onInitObs.end(); ++it)
    {
      (*it)();
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

void LifeCyclePump::removeAllObs()
{
  _onInitObs.clear();
  _onUpdateObs.clear();
  _onDestroyObs.clear();
}
