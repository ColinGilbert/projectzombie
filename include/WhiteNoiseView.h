/*
 * WhiteNoiseView.h
 *
 *  Created on: Oct 4, 2008
 *      Author: bey0nd
 */

#ifndef WHITENOISEVIEW_H_
#define WHITENOISEVIEW_H_

#include <Ogre.h>
using namespace Ogre;

namespace ZGame
{
  namespace LifeCycle
  {
    struct LifeCycleObserver;
  }

  class WhiteNoiseView
  {
  public:
    WhiteNoiseView();
    virtual
    ~WhiteNoiseView();

    bool onUpdate(const Ogre::FrameEvent &evt);
    void fillLfcObservers(LifeCycle::LifeCycleObserver &obs);

    void init();

  protected:

    TexturePtr _whiteNoiseTex;
    Ogre::RenderTarget* _rtt;
    Rectangle2D* _fullScreenQuad;

    void createMesh();

  };

}

#endif /* WHITENOISEVIEW_H_ */
