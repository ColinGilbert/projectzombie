/*
 * InputController.h
 *      InputController handles input in an input thread.
 *
 *      References:
 *      InputManager from Ogre Boilerplate found on Ogre's website.
 *
 *  Created on: Aug 25, 2008
 *      Author: bey0nd
 */

#ifndef INPUTCONTROLLER_H_
#define INPUTCONTROLLER_H_

#include <vector>
#include <map>
//#include <string>
#include <boost/thread/thread.hpp>
#include <boost/thread/xtime.hpp>
#include <Ogre.h>
//Use this define to signify OIS will be used as a DLL
//(so that dll import/export macros are in effect)
//#define OIS_DYNAMIC_LIB
//#include <OIS/OIS.h>
#include "EventDelegates.h"

namespace ZGame
{
  class InputController : public OIS::KeyListener, OIS::MouseListener
  {
  public:
    InputController();
    virtual
    ~InputController();

    bool
    onInit(Ogre::RenderWindow* window);
    void
    onDestroy();
    void
    run();
    void
    addKeyListeners(const Ogre::String& oID,
        ZGame::EVENT::KeyboardEvtObserver keo);
    void
    removeKeyListeners(const Ogre::String& oID);
    void
    addMouseListeners(const Ogre::String& oID,
        ZGame::EVENT::MouseEvtObserver meo);
    void
    removeMouseListeners(const Ogre::String& oID);
    void
    removeAllListeners();
    void
    setWindowExtents(int width, int height);

    //abstract methods from KeyListener,MouseListener
    bool
    keyPressed(const OIS::KeyEvent &e);
    bool
    keyReleased(const OIS::KeyEvent &e);

    bool
    mouseMoved(const OIS::MouseEvent &e);
    bool
    mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
    bool
    mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);

    OIS::Mouse*
    getMouse()
    {
      return _mouse;
    }
    OIS::Keyboard*
    getKeyboard()
    {
      return _keyb;
    }

  protected:
    OIS::InputManager *_inputSystem;
    OIS::Mouse *_mouse;
    OIS::Keyboard *_keyb;

    std::vector<bool> _keyStates;

    typedef std::map<Ogre::String, ZGame::EVENT::KeyboardEvtObserver>
        KeyObsType;
    typedef std::map<Ogre::String, ZGame::EVENT::MouseEvtObserver> MouseObsType;
    typedef KeyObsType::iterator KeyObsIterator;
    typedef MouseObsType::iterator MouseObsIterator;
    KeyObsType _keyObservers;
    MouseObsType _mouseObservers;

    void
    capture();

  private:
    bool _stillRunning;
  };
}
#endif /* INPUTCONTROLLER_H_ */
