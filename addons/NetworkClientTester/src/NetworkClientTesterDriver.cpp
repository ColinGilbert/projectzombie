/**
* \file This file is the driver program for the Network Client Tester application. This application will test Network clients for project zombie.
*
*/

#include <iostream>
#include <stdexcept>

using namespace std;
#include <boost/random.hpp>
#include <Ogre.h>

#include "NetworkClientTesterController.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif

    

    int
        main(int argc, char** argv)
    {
        using namespace ZGame;
        ZGame::MainController* engineControl = 0;
        cout << "Starting NetClientTesterController." << endl;

        engineControl = new ZGame::NetworkClientTesterController();

        if(!engineControl->onInit())
        {
            cout << "Failed to initialize client tester controller." << endl;
            return 1;
        }
        try
        {
            engineControl->run();
        }catch(Ogre::Exception e)
        {
            ostringstream oss;
            oss << "Something bad happened when running the engine." << endl;
            oss << e.what() << endl;
            return 0;
        }
        catch(std::exception e)
        {
            ostringstream oss;
            oss << "Something bad happened when running the engine." << endl;
            oss << e.what() << endl;
            return 0;
        }

        engineControl->onDestroy();

        cout << "Finished client tester." << endl;
        delete engineControl;
        
        return 0;
    }
#ifdef __cplusplus
}
#endif