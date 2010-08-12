//#include <boost/thread.hpp>
#define _SECURE_SCL 0

#include <iostream>
#include <stdexcept>

using namespace std;
//#include <boost/random.hpp>
#include <Ogre.h>
//#include <signal.h>
//#include "InputController.h"
#include "EngineController.h"
#include "ServerController.h"
//#include "GPUEntsDistributor.h"

#define CROWD 1

/*
* ZombieDriver.cpp
*
*  Created on: Aug 20, 2008
*      Author: bey0nd
*/
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#define __DEBUG
#endif

#ifdef __cplusplus
extern "C" {
#endif



    ZGame::MainController* engineControl = 0;

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
#ifdef __DEBUG
        cout << "Debug mode: " << endl;
#endif
        //signal(SIGINT,clean_up_func);

        cout << "About to new EngineController" << endl;

        //#if
        if(argc == 1) //run client
            engineControl = new ZGame::EngineController();
        else if(argc > 1) //run server
            engineControl = new ZGame::ServerController();

        try
        {

            cout << "Finished new EngineController" << endl;
            if (!engineControl->onInit())
                return 1;
            engineControl->run();

        }
        catch (Ogre::Exception e)
        {
            ostringstream oss;
            oss << "Something bad happened when running the engine." << endl;
            oss << e.what() << endl;
            Ogre::LogManager::getSingleton().logMessage(Ogre::LML_CRITICAL, oss.str());
            engineControl->onDestroy();
            delete engineControl;
        }
        catch (std::exception e)
        {
            ostringstream oss;
            oss << "Something bad happened when running the engine." << endl;
            oss << e.what() << endl;
            Ogre::LogManager::getSingleton().logMessage(Ogre::LML_CRITICAL, oss.str());
            delete engineControl;
        }
        catch(...)
        {
           
            delete engineControl;
        }

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
