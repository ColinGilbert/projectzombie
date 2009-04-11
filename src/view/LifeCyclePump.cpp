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
  if (obs.onInit != 0)
    _onInitObs.push_back(obs.onInit);
  if (obs.onUpdate != 0)
    _onUpdateObs.push_back(obs.onUpdate);
  if (obs.onDestroy != 0)
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
LifeCyclePump::removeAllObs()
{
  _onInitObs.clear();
  _onUpdateObs.clear();
  _onDestroyObs.clear();
}
