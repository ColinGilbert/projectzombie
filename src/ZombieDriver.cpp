#include <boost/thread.hpp>
#include <cmath>
#include <iostream>
using namespace std;

/*
 * ZombieDriver.cpp
 *
 *  Created on: Aug 20, 2008
 *      Author: bey0nd
 */

#define PI 3.14159265

#include <auto_ptr>


struct MyThreadFunc
{
  void operator()()
  {
    int loopNum = 1000;
    double dx = 0.1*PI;
    double j = 0.0;
    for(int i=0; i<loopNum; i++)
      {
        cout << "loop #" << i << ": " << cos(j)+sin(j) << endl;
        j+=dx;
      }
  }
}threadFun;

int main(int argc, char** argv)
{

  boost::thread myThread(threadFun);
  boost::thread::yield();
  myThread.join();
  return 0;
}


