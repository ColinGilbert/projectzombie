/*
 * KeyEventRegister.cpp
 *
 *  Created on: Sep 27, 2008
 *      Author: bey0nd
 */

#include <stdexcept>
using namespace std;
#include <Ogre.h>
using namespace Ogre;
#include "KeyEventRegister.h"

namespace ZGame
{

  KeyEventRegister::KeyEventRegister() : _registered(false)
  {
    // TODO Auto-generated constructor stub

  }

  KeyEventRegister::~KeyEventRegister()
  {
    // TODO Auto-generated destructor stub
  }

  void KeyEventRegister::injectKeySubj(const EVENT::KeyEvtSubject &subj)
  {
    Ogre::LogManager::getSingleton().logMessage(LML_TRIVIAL,"KeyEventRegister::injectKeySubj");
    for(KeyObsIt it=_keyObs.begin();it!=_keyObs.end();++it)
      {
        subj(*it);
      }
  }

  void KeyEventRegister::registerKeyObs(const EVENT::KeyboardEvtObserver &obs)
  {
    _keyObs.push_back(obs);
  }


}
