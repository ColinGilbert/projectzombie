/*
 * MousePump.h
 *
 *  Created on: Sep 29, 2008
 *      Author: bey0nd
 */

#ifndef MOUSEPUMP_H_
#define MOUSEPUMP_H_
#include <OIS/OIS.h>
#include "EventDelegates.h"
namespace ZGame
{

  class MousePump
  {
  public:
    MousePump();
    virtual
    ~MousePump();

    void addMouseObserver(const ZGame::EVENT::MouseEvtObserver &obs);
    bool updateMouseMoveEvt(const OIS::MouseEvent& evt);
    bool updateMouseUpEvt(const OIS::MouseEvent& evt,const OIS::MouseButtonID bId);
    bool updateMouseDownEvt(const OIS::MouseEvent& evt,const OIS::MouseButtonID bId);
    void removeAllObs();
  protected:
      typedef std::vector<ZGame::EVENT::ZMouseMoveEvt>::iterator MoveObsItr;
      typedef std::vector<ZGame::EVENT::ZMouseEvt>::iterator UpObsItr;
      typedef std::vector<ZGame::EVENT::ZMouseEvt>::iterator DownObsItr;
      std::vector<ZGame::EVENT::ZMouseMoveEvt> _moveObs;
      std::vector<ZGame::EVENT::ZMouseEvt> _upObs;
      std::vector<ZGame::EVENT::ZMouseEvt> _downObs;

  };

}

#endif /* MOUSEPUMP_H_ */
