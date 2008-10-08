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
#include "ObsInjectors.h" //interface so we can get Life Cycle observers injectected to us.

namespace ZGame
{
  /*
  namespace LifeCycle
  {
    struct LifeCycleObserver;
  }*/

  class WhiteNoiseView : public LFCObsInjector
  {
  public:
    WhiteNoiseView();
    virtual
    ~WhiteNoiseView();

    bool onInit();
    bool onUpdate(const Ogre::FrameEvent &evt);
    void fillLfcObservers(LifeCycle::LifeCycleObserver &obs);



  protected:

    TexturePtr _whiteNoiseTex;
    auto_ptr<Rectangle2D> _fullScreenQuad;
    Ogre::Timer _timer;

    void createMesh();

  };

}

#endif /* WHITENOISEVIEW_H_ */
