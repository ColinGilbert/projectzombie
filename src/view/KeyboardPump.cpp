/*
 * KeyboardPump.cpp
 *
 *  Created on: Sep 5, 2008
 *      Author: bey0nd
 */

#include "KeyboardPump.h"

using namespace ZGame;

KeyboardPump::KeyboardPump()
{

}

KeyboardPump::~KeyboardPump()
{

}

void KeyboardPump::addKeyboardObserver(const ZGame::EVENT::KeyboardEvtObserver &obs)
{
  if(obs.kde)
    _onKeyDownObs.push_back(obs.kde);
  if(obs.kde)
    _onKeyUpObs.push_back(obs.kue);
}

void
    KeyboardPump::addModifierStateObserver(const ZGame::EVENT::ZModifierStateEvt &obs)
{
    if(obs)
        _onModifierStateObs.push_back(obs);
}

void KeyboardPump::updateKeyDownObs(const OIS::KeyEvent &evt)
 {
   for(KeyDownObsItr it=_onKeyDownObs.begin();it!=_onKeyDownObs.end();++it)
     {
       if(!(*it)(evt))
           break; //make delegate call
     }
 }

 void KeyboardPump::updateKeyUpObs(const OIS::KeyEvent &evt)
{
  for (KeyUpObsItr it = _onKeyUpObs.begin(); it != _onKeyUpObs.end(); ++it)
    {
      if(!(*it)(evt))
          break;
    }
}

void
    KeyboardPump::updateModifierStateObs(const unsigned int modifierState)
{
    for(auto iter = _onModifierStateObs.begin(); iter != _onModifierStateObs.end(); ++iter)
    {
        (*iter)(modifierState);
    }
}
void KeyboardPump::removeAllObs()
{
  _onKeyUpObs.clear();
  _onKeyDownObs.clear();
  _onModifierStateObs.clear();
}

