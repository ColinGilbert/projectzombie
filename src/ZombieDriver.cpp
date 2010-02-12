//#include <boost/thread.hpp>

#include <iostream>
#include <stdexcept>

using namespace std;
#include <boost/random.hpp>
#include <Ogre.h>
#include <signal.h>
#include "InputController.h"
#include "EngineController.h"
#include "ServerController.h"
#include "GPUEntsDistributor.h"

/*
 * ZombieDriver.cpp
 *
 *  Created on: Aug 20, 2008
 *      Author: bey0nd
 */
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

ZGame::MainController* engineControl = 0;


void clean_up_func(int signum)
{
  engineControl->onDestroy();
  delete engineControl; 
  exit(0);
}
/*
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
INT 
WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else*/
int
main(int argc, char** argv)
//#endif
{
  using namespace ZGame;

  signal(SIGINT,clean_up_func);

  
//#if
  if(argc == 1) //run client
    engineControl = new ZGame::EngineController();
  else if(argc >= 1) //run server
    engineControl = new ZGame::ServerController();
  try
    {
      if (!engineControl->onInit())
        return 1;
    }
  catch (Ogre::Exception e)
    {
      ostringstream oss;
      oss << "EngineControl onInit failed: " << endl;
      oss << e.what() << endl;
      Ogre::LogManager::getSingleton().logMessage(Ogre::LML_CRITICAL, oss.str());
    }
  catch (std::exception e)
    {
      ostringstream oss;
      oss << "EngineContro onInit failed: " << endl;
      oss << e.what() << endl;
      Ogre::LogManager::getSingleton().logMessage(Ogre::LML_CRITICAL, oss.str());    
    }
  
  try
    {
      engineControl->run();
    }
  catch (Ogre::Exception e)
    {
      ostringstream oss;
      oss << "Something bad happened, when running the engine." << endl;
      oss << e.what() << endl;
      Ogre::LogManager::getSingleton().logMessage(Ogre::LML_CRITICAL, oss.str());
    }
  catch (std::exception e)
    {
      ostringstream oss;
      oss << "Something bad happened, when running the engine." << endl;
      oss << e.what() << endl;
      Ogre::LogManager::getSingleton().logMessage(Ogre::LML_CRITICAL, oss.str());
      return 0;
    }
  
    engineControl->onDestroy();
  
    cout << "returinging. " << endl;
    delete engineControl;
    return 0;
}
#ifdef __cplusplus
}
#endif

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
