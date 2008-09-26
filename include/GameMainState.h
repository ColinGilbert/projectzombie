/*
 * GameMainState.h
 *
 *  Created on: Sep 18, 2008
 *      Author: bey0nd
 */

#ifndef GAMEMAINSTATE_H_
#define GAMEMAINSTATE_H_

#include <memory>
#include <string>
using namespace std;
#include <Ogre.h>

#include "GameState.h"

namespace ZGame
{
  class GPUEntities;
  class GPUEntsView;
  class GameMainState : public ZGame::GameState
  {
  public:
    GameMainState();
    virtual ~GameMainState();

    virtual void initialize();
    void injectLifeCycleSubject(ZGame::LifeCycle::LifeCycleSubject &subject);
    void injectKeyEvtSubject(ZGame::EVENT::KeyEvtSubject &subject);

    //life cycle methods
    bool onUpdate(const Ogre::FrameEvent &evt);
    bool onInit();
    bool onDestroy();

    //key events
    bool onKeyDown(const OIS::KeyEvent &evt);
    bool onKeyUp(const OIS::KeyEvent &evt);
  protected:
    auto_ptr<GPUEntities> _gpuEnts;
    GPUEntsView* _gpuEntsView;
    void createGPUEntities();
  };
}

#endif /* GAMEMAINSTATE_H_ */
