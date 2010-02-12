/*
 * WhiteNoiseView.h
 *
 *  Created on: Oct 4, 2008
 *      Author: bey0nd
 */

#ifndef WHITENOISEVIEW_H_
#define WHITENOISEVIEW_H_
#include <memory>
using namespace std;
#include <Ogre.h>
using namespace Ogre;

namespace ZGame
{
  /*
  namespace LifeCycle
  {
    struct LifeCycleObserver;
  }*/

  class PingPongShaders;
  class WhiteNoiseView
  {
  public:
    WhiteNoiseView();
    virtual
    ~WhiteNoiseView();

    bool onInit();
    bool onUpdate(const Ogre::FrameEvent &evt);

  protected:

    TexturePtr _whiteNoiseTex;
    auto_ptr<Rectangle2D> _fullScreenQuad;
    Ogre::Timer _timer;
    auto_ptr<PingPongShaders> _pingPong;

    void createMesh();

  };

}

#endif /* WHITENOISEVIEW_H_ */
