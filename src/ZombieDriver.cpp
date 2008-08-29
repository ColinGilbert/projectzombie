#include <boost/thread.hpp>
#include "InputController.h"
#include "EngineController.h"
using namespace std;

/*
 * ZombieDriver.cpp
 *
 *  Created on: Aug 20, 2008
 *      Author: bey0nd
 */

ZGame::InputController inControl;
ZGame::EngineController engineControl;

struct MyInputThread
{
  void operator()()
  {
    inControl.run();
    inControl.onDestroy();
  }
}inputThread;

int main(int argc, char** argv)
{
  using namespace ZGame;
  if(!engineControl.onInit())
    return 1;
  if(!inControl.onInit(engineControl.getRenderWindow()))
    return 1;
  engineControl.injectInputSubject(&inControl);
  boost::thread myThread(inputThread);

  engineControl.run();
  engineControl.onDestroy();

  myThread.join();
  return 0;
}


