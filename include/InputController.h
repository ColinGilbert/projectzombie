/**
*Copyright (c) 2010 Fdastero LLC

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
**/

#pragma once


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

    unsigned int
        getModifierState();

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
    unsigned int _modifierState;
  };
}
