/**
* \file This file is the driver program for the Network Client Tester application. This application will test Network clients for project zombie.
*
*/

#include <iostream>
#include <stdexcept>

using namespace std;

#include <tchar.h>
#include <boost/thread/thread.hpp>
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


    static const int numOfProcess = 4; //WE ARE hardcoding this because we are not allocating the process infos on the heap.


    int runChildProcess()
    {
        using namespace ZGame;
        cout << "Running client tester as child process." << endl;
        //Note CommandController may not be thread safe.
        ZGame::MainController* engineControl = new ZGame::NetworkClientTesterController();
        if(!engineControl->onInit())
        {
            cout << "Failed to initialize client tester controller in thread." << endl;
            return 1;
        }

        try
        {
            engineControl->run();
        }catch(Ogre::Exception e)
        {
            ostringstream oss;
            oss << "Something bad happened when running the engine in thread." << endl;
            oss << e.what() << endl;
            return 1;
        }
        catch(std::exception e)
        {
            ostringstream oss;
            oss << "Something bad happened when running the engine in thread." << endl;
            oss << e.what() << endl;
            return 1;
        }

        engineControl->onDestroy();

        cout << "Finished client tester in thread." << endl;
        delete engineControl;
        return 0;
    }



    int spawnChildProcesses(DWORD exitCodes[])
    {
        STARTUPINFO startInfoArray[numOfProcess];
        PROCESS_INFORMATION processInfoArray[numOfProcess];
        for(int i=0; i<numOfProcess; ++i)
        {
            ZeroMemory(&startInfoArray[i], sizeof(startInfoArray[i]));
            startInfoArray[i].cb = sizeof(startInfoArray[i]);
            ZeroMemory(&processInfoArray[i], sizeof(processInfoArray[i]));
        }


        TCHAR* cmdLine[numOfProcess];

        for(int i=0; i < numOfProcess; i++)
        {
            cmdLine[i] = new TCHAR[50];
        }


        for(int i=0; i < numOfProcess; i++)
        {
            _stprintf(cmdLine[i],_T(".\\networkclienttester_d.exe spawnedprocess"));
        }
        //CreateProcess(NULL,cmdLine[i],NULL,NULL,FALSE,CREATE_NEW_CONSOLE,NULL,NULL,&startInfoArray[0],&processInfoArray[0]))
        for(int i=0; i<numOfProcess; ++i)
        {
            if(!CreateProcess(NULL,cmdLine[i],NULL,NULL,FALSE,CREATE_NEW_CONSOLE,NULL,NULL,&startInfoArray[0],&processInfoArray[0]))
            {
                cout << "CreateProcess failed at process #" << 0 << endl;
                return 1;
            }
        }

        //Wait for child process to exit.
        for(int i=0; i < numOfProcess; ++i)
        {
            WaitForSingleObject(processInfoArray[i].hProcess, INFINITE);
        }

        for(int i=0; i < numOfProcess; ++i)
        {
            //Get the exit code
            GetExitCodeProcess(processInfoArray[i].hProcess,&exitCodes[i]);
            CloseHandle(processInfoArray[i].hProcess);
        }



        for(int i=0; i < numOfProcess; i++)
        {
            delete cmdLine[i];
        }

        return 0;
    }

    int
        main(int argc, char** argv)
    {

        int retCode = 1;
        DWORD exitCodes[numOfProcess]; //DWORD are unsigned long!

        if(argc == 1)
        {
            retCode = spawnChildProcesses(exitCodes);

            for(int i=0; i < numOfProcess; ++i)
            {
                //Get the exit code
                cout << "Process #" << i << " exited with EXIT_CODE: " << exitCodes[i] << endl;
            }

        }
        else if (argc > 1)
        {
            retCode = runChildProcess();
            int waitForInput;
            cout << "Please press input." << endl;
            cin >> waitForInput;
        }



        return retCode;
    }
#ifdef __cplusplus
}
#endif