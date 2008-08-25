/*
 * EngineController.h
 *
 *  Created on: Aug 24, 2008
 *      Author: bey0nd
 */

#ifndef ENGINECONTROLLER_H_
#define ENGINECONTROLLER_H_

#include <delegate.h>
#include "ZDelegates.h"

namespace ZGame
{

  class EngineController
  {
  public:
    EngineController();
    virtual
    ~EngineController();

    void transitionState();
    void onInit();
    void run();
    void loadStates();
    void addKeyboardListener(ZGame::EVENT::KeyboardEvent ke);
    void addMouseListener(ZGame::EVENT::MouseEvent me);

  };

}

#endif /* ENGINECONTROLLER_H_ */
