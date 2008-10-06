//#include <boost/thread.hpp>

#include <iostream>
using namespace std;
#include <boost/random.hpp>
#include <Ogre.h>
#include "InputController.h"
#include "EngineController.h"
#include "GPUEntsDistributor.h"

/*
 * ZombieDriver.cpp
 *
 *  Created on: Aug 20, 2008
 *      Author: bey0nd
 */


int main(int argc, char** argv)
{
  using namespace ZGame;

  ZGame::EngineController engineControl;
  using namespace ZGame;
  try
  {
    if(!engineControl.onInit())
      return 1;
  }catch(Ogre::Exception e)
  {
    ostringstream oss;
    oss << "EngineControl onInit failed: " << endl;
    oss << e.what() << endl;
    Ogre::LogManager::getSingleton().logMessage(Ogre::LML_CRITICAL,oss.str());
    engineControl.onDestroy();
    return 1;
  }

  try
  {
    engineControl.run();
  }catch(Ogre::Exception e)
  {
    ostringstream oss;
    oss << "Something bad happened when running the engine." << endl;
    oss << e.what() << endl;
    Ogre::LogManager::getSingleton().logMessage(Ogre::LML_CRITICAL,oss.str());
    engineControl.onDestroy();
  }
  engineControl.onDestroy();

  return 0;
}


//ZGame::InputController inControl;

/*
struct MyInputThread
{
  void operator()()
  {
    inControl.run();
    inControl.onDestroy();
  }
}inputThread;
*/
