//#include <boost/thread.hpp>


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
  if(!engineControl.onInit())
    return 1;
  //if(!inControl.onInit(engineControl.getRenderWindow()))
    //return 1;
  //engineControl.injectInputSubject(&inControl);
  //boost::thread myThread(inputThread);

  engineControl.run();


  // myThread.join();
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
