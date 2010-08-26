#pragma once
/**
 * \file This file defines a controller for controlling application graphical state.
 */

#include <Ogre.h>

namespace ZGame
{
  class GraphicsController
  {
  public:
    GraphicsController();
    virtual ~GraphicsController();

    bool onInit();
    bool onDestroy();
    bool onUpdate();

  protected:

    Ogre::CompositorInstance* _gBufferInstance;
    Ogre::CompositorInstance* _ssaoInstance;

  private:
  };
}
