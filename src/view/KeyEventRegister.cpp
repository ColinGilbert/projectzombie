/*
 * KeyEventRegister.cpp
 *
 *  Created on: Sep 27, 2008
 *      Author: bey0nd
 */

#include <stdexcept>
using namespace std;
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
    if(!_registered)
      throw std::invalid_argument("Trying to inject observers to key subject when no observers have been registered!");
    for(KeyObsIt it=_keyObs.begin();it<_keyObs.end();++it)
      {
        subj(*it);
      }
  }

  void KeyEventRegister::registerKeyObs(const EVENT::KeyboardEvtObserver &obs)
  {
    _registered = true;
    _keyObs.push_back(obs);
  }


}
