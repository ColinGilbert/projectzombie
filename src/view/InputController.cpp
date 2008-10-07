/*
 * InputController.cpp
 *
 *  Created on: Aug 25, 2008
 *      Author: bey0nd
 */
#include <iostream>

using namespace std;
#include "InputController.h"

using namespace ZGame;

InputController::InputController() : _inputSystem(0),_mouse(0),_keyb(0),_keyStates(0xFF,false)
,_stillRunning(true)
{
  // TODO Auto-generated constructor stub
}

InputController::~InputController()
{

}

bool InputController::onInit(Ogre::RenderWindow* window)
{
  if(_inputSystem)
    return false;
  //setup some basic variables
  OIS::ParamList paramList;
  size_t winHnd = 0;
  std::ostringstream winHndStr;
  //Get Window handle
#if defined OIS_WIN32_PLATFORM
  {
    window->getCustomAttributee("WINDOW",&winHnd);
    paramList.insert(std::make_pair(std::string("w32_mouse"),std::string("DISCL_FOREGROUND") ));
    paramList.insert(std::make_pair(string("w32_mouse"),string("DISCL_NONEXCLUSIVE") ));
  }
#elif defined OIS_LINUX_PLATFORM
  {
    paramList.insert(std::make_pair(std::string("x11_mouse_grab"),std::string("true") ));
    window->getCustomAttribute("WINDOW",&winHnd);
  }
#endif
  assert(winHnd !=0 && "Invalid window handle");

  //fill parameter list
  winHndStr << (unsigned int)winHnd;
  paramList.insert(std::make_pair(std::string("WINDOW"),winHndStr.str() ));

  _inputSystem = OIS::InputManager::createInputSystem(paramList);

  //If possible create a buffered keyboard
  _keyb = static_cast<OIS::Keyboard*> (_inputSystem->createInputObject(
      OIS::OISKeyboard, true));
  _keyb->setEventCallback(this);


  _mouse = static_cast<OIS::Mouse*> (_inputSystem->createInputObject(
      OIS::OISMouse, true));
  _mouse->setEventCallback(this);

  //Get window size
  unsigned int width, height, depth;
  int left, top;
  window->getMetrics(width, height, depth, left, top);

  //set mouse region
  this->setWindowExtents(width, height);

  return true;
}

void InputController::setWindowExtents(int width,int height)
{
  const OIS::MouseState &mouseState = _mouse->getMouseState();
  mouseState.width = width;
  mouseState.height = height;
}

void InputController::onDestroy()
{
  cout << "InputController::onDestroy" << endl;
    // TODO Auto-generated destructor stub
    if (_inputSystem)
    {
      if (_mouse)
        {
            _inputSystem->destroyInputObject(_mouse);
            _mouse = 0;
          }
        if(_keyb)
          {
            _inputSystem->destroyInputObject(_keyb);
            _keyb = 0;
          }
        _inputSystem->destroyInputSystem(_inputSystem);
        _inputSystem = 0;
        _keyObservers.clear();
        _mouseObservers.clear();
      }
}

void InputController::run()
{
  capture();
  /*
  while(_stillRunning)
    {
      capture();
      //boost::thread
    }
    */
}

void InputController::capture()
{
  if(_mouse)_mouse->capture();
  if(_keyb)_keyb->capture();
  OIS::MouseState &mutableMouseState = const_cast<OIS::MouseState &>(_mouse->getMouseState());
      mutableMouseState.X.abs = 500.0;
      mutableMouseState.Y.abs = 500.0;
}

void InputController::addKeyListeners(const string& oID,::EVENT::KeyboardEvtObserver keo)
{
  if(_keyb)
    {
      KeyObsIterator cur = _keyObservers.find(oID);
      if(cur == _keyObservers.end())
        _keyObservers[oID] = keo;
    }
}

void InputController::removeKeyListeners(const string &oID)
{
  KeyObsIterator cur = _keyObservers.find(oID);
  if(cur != _keyObservers.end())
    _keyObservers.erase(cur);
}

void InputController::addMouseListeners(const string& oID,::EVENT::MouseEvtObserver meo)
{
  if(_mouse)
    {
      MouseObsIterator cur = _mouseObservers.find(oID);
      if(cur == _mouseObservers.end())
        _mouseObservers[oID] = meo;
    }
}

void InputController::removeMouseListeners(const string& oID)
{
  MouseObsIterator cur = _mouseObservers.find(oID);
  if(cur != _mouseObservers.end())
    _mouseObservers.erase(cur);
}

void InputController::removeAllListeners()
{
  _keyObservers.clear();
  _mouseObservers.clear();
}

bool InputController::keyPressed(const OIS::KeyEvent &e)
{
  _keyStates[e.key] = true;
  for(KeyObsIterator it = _keyObservers.begin(); it != _keyObservers.end();++it)
    {
      it->second.kde(e);
    }
  return true;
}

bool InputController::keyReleased(const OIS::KeyEvent &e)
{
  _keyStates[e.key] = false;
  for(KeyObsIterator it=_keyObservers.begin();it!=_keyObservers.end();++it)
    {
      it->second.kue(e);
    }
  if(e.key == OIS::KC_ESCAPE)
    _stillRunning = false;
  return true;
}

bool InputController::mouseMoved(const OIS::MouseEvent &e)
{
  for(MouseObsIterator it=_mouseObservers.begin();it!=_mouseObservers.end();++it)
    {
      it->second.mme(e);
    }
  return true;
}

bool InputController::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  for(MouseObsIterator it=_mouseObservers.begin();it!=_mouseObservers.end();++it)
    {
      it->second.mde(e,id);
    }
  return true;
}

bool InputController::mouseReleased(const OIS::MouseEvent &e,OIS::MouseButtonID id)
{
  for(MouseObsIterator it=_mouseObservers.begin();it!=_mouseObservers.end();++it)
    {
      it->second.mue(e,id);
    }
  return true;
}
