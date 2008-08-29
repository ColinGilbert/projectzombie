/*
 * GameState.cpp
 *
 *  Created on: Aug 28, 2008
 *      Author: bey0nd
 */

#include <Ogre.h>
#include "GameState.h"

namespace ZGame
{

  GameState::GameState()
  {
    // TODO Auto-generated constructor stub

  }

  GameState::~GameState()
  {
    // TODO Auto-generated destructor stub
  }


  void GameState::addLifeCycleSubjectInjector(ZGame::LifeCycle::LifeCycleSubjectInjector &injector)
  {
    Ogre::LogManager* lm = Ogre::LogManager::getSingletonPtr();
    lm->logMessage(Ogre::LML_NORMAL,"In addLifeCycleSubjectInjector");
    _subjectInjectors.push_back(injector);
  }

  void GameState::addKeySubjectInjector(ZGame::EVENT::KeyEvtSubjectInjector& injector)
  {
    Ogre::LogManager* lm = Ogre::LogManager::getSingletonPtr();
    lm->logMessage(Ogre::LML_NORMAL,"In addKeySubjectInjector");
    _keyEvtSubjectInjectors.push_back(injector);
  }

  void GameState::injectLifeCycleSubject(ZGame::LifeCycle::LifeCycleSubject &subject)
  {
    Ogre::LogManager* lm = Ogre::LogManager::getSingletonPtr();
    lm->logMessage(Ogre::LML_NORMAL,"In injectLifeCycleSubject");
    for(SubjectInjectorsItr it=_subjectInjectors.begin(); it != _subjectInjectors.end(); ++it)
      {
        (*it)(subject);
      }
    _subjectInjectors.clear(); //we're done with injectors since we injected subject to them.
  }

  void GameState::injectKeyEvtSubject(ZGame::EVENT::KeyEvtSubject& subject)
  {
    Ogre::LogManager* lm = Ogre::LogManager::getSingletonPtr();
    lm->logMessage(Ogre::LML_NORMAL,"In injectKeyEvtSubject");
    for(KeyEvtSubInjectorItr it=_keyEvtSubjectInjectors.begin();it!=_keyEvtSubjectInjectors.end();++it)
      {
        (*it)(subject);
      }
    _keyEvtSubjectInjectors.clear(); //done with injectors since we injected subjects to them.
  }


}
