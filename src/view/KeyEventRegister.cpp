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
#include "DelegatesUtil.h"

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

    _keyObs.clear();
  }

  void KeyEventRegister::registerKeyObs(EVENT::KeyboardEvtObserver &obs,
      REGISTER_ORDER order)
  {
      if(order == ORDER_DONOTCARE)
      {
        _keyObs.push_back(obs);
      }
      else
      {
          _keyObs.push_front(obs);
      }
    EVENT::clearKeyObs(obs);
  }


}
