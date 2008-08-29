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
#include <string>
using namespace std;

#include <boost/thread/thread.hpp>
#include <boost/thread/xtime.hpp>
#include <Ogre.h>
#include <OIS/OIS.h>
#include "EventDelegates.h"

namespace ZGame
{
  class InputController : public OIS::KeyListener, OIS::MouseListener
  {
  public:
    InputController();
    virtual
    ~InputController();

    bool onInit(Ogre::RenderWindow* window);
    void run();
    void addKeyListeners(const string& oID,ZGame::EVENT::KeyboardEvtObserver keo);
    void removeKeyListeners(const string& oID);
    void addMouseListeners(const string& oID,ZGame::EVENT::MouseEvtObserver meo);
    void removeMouseListeners(const string& oID);
    void removeAllListeners();
    void setWindowExtents(int width,int height);


    //abstract methods from KeyListener,MouseListener
    bool keyPressed(const OIS::KeyEvent &e);
    bool keyReleased(const OIS::KeyEvent &e);

    bool mouseMoved(const OIS::MouseEvent &e);
    bool mousePressed(const OIS::MouseEvent &e,OIS::MouseButtonID id);
    bool mouseReleased(const OIS::MouseEvent &e,OIS::MouseButtonID id);

  protected:
    OIS::InputManager *_inputSystem;
    OIS::Mouse *_mouse;
    OIS::Keyboard *_keyb;

    vector<bool> _keyStates;

    typedef map<string,ZGame::EVENT::KeyboardEvtObserver> KeyObsType;
    typedef map<string,ZGame::EVENT::MouseEvtObserver> MouseObsType;
    typedef KeyObsType::iterator KeyObsIterator;
    typedef MouseObsType::iterator MouseObsIterator;
    KeyObsType _keyObservers;
    MouseObsType _mouseObservers;

    void capture();

  private:
    bool _stillRunning;
  };
}
#endif /* INPUTCONTROLLER_H_ */
