/*
 * MousePump.cpp
 *
 *  Created on: Sep 29, 2008
 *      Author: bey0nd
 */

#include "MousePump.h"

namespace ZGame
{

  MousePump::MousePump()
  {
    // TODO Auto-generated constructor stub

  }

  MousePump::~MousePump()
  {
    // TODO Auto-generated destructor stub
  }
  void MousePump::addMouseObserver(const ZGame::EVENT::MouseEvtObserver &obs)
  {
    if(obs.mde)
      _downObs.push_back(obs.mde);
    if(obs.mue)
      _upObs.push_back(obs.mue);
    if(obs.mme)
      _moveObs.push_back(obs.mme);
  }
  bool MousePump::updateMouseDownEvt(const OIS::MouseEvent &evt, const OIS::MouseButtonID bId)
  {
    for(DownObsItr it=_downObs.begin();it !=_downObs.end();++it)
      {
        (*it)(evt,bId);
      }
    return true;
  }
  bool MousePump::updateMouseUpEvt(const OIS::MouseEvent &evt, const OIS::MouseButtonID bId)
  {
    for(UpObsItr it=_upObs.begin();it !=_upObs.end();++it)
      {
        (*it)(evt,bId);
      }
    return true;
  }
  bool MousePump::updateMouseMoveEvt(const OIS::MouseEvent &evt)
  {
    for(MoveObsItr it=_moveObs.begin();it !=_moveObs.end();++it)
      {
        (*it)(evt);
      }
    return true;
  }

  void MousePump::removeAllObs()
  {
    _upObs.clear();
    _downObs.clear();
    _moveObs.clear();
  }

}
