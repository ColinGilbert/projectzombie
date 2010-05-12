/**
* \file This file is the driver program for the Network Client Tester application. This application will test Network clients for project zombie.
*
*/

#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <csignal>
using namespace std;

#include <tchar.h>

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


    void ignore_sigint_handler(int signum)
    {
        //ignore.
    }


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
    int redirectStdout(const HANDLE &childStdOutRd, int processNumber)
    {
        for(;;)
        {
            DWORD dwAvail = 0;
            if(!::PeekNamedPipe(childStdOutRd, NULL, 0, NULL, &dwAvail, NULL))
                break; //error, the child process might ended.

            if(!dwAvail) //no data available, return
                return 1;

            char szOutput[256];
            DWORD dwRead = 0;
            if(!::ReadFile(childStdOutRd, szOutput, std::min((DWORD)255, dwAvail),
                &dwRead, NULL) || !dwRead)
                break;

            szOutput[dwRead] = 0; //null termination.
            cout << "[Process #" << processNumber << "]: " << szOutput;
        }

        return 0;

    }
    int spawnChildProcesses(DWORD exitCodes[])
    {
        //Let's register for SIGINT signal. So we can ignore it in the master process.
        signal(SIGINT,ignore_sigint_handler);

        STARTUPINFO startInfoArray[numOfProcess];
        PROCESS_INFORMATION processInfoArray[numOfProcess];
        HANDLE childStdOutRdArray[numOfProcess];
        HANDLE childStdOutWrArray[numOfProcess];
        SECURITY_ATTRIBUTES saAttr;

        saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
        saAttr.bInheritHandle = TRUE;
        saAttr.lpSecurityDescriptor = NULL;
        //Initialize the child std out pipe.
        for(int i=0; i<numOfProcess; ++i)
        {
            if(!CreatePipe(&childStdOutRdArray[i], &childStdOutWrArray[i], &saAttr, 0))
                return 1;//ErrorExit(TEXT("Stdout read CreatePipe"));
            //Ensure the read handle to the pipe for STDOUT is not inherited.
            if(!SetHandleInformation(childStdOutRdArray[i], HANDLE_FLAG_INHERIT, 0))
                return 1;//ErrorExit(TEXT("Stdout SetHandleInformation"));
        }

        ////////////////Create the child process.///////////////////////

        for(int i=0; i<numOfProcess; ++i)
        {
            ZeroMemory(&startInfoArray[i], sizeof(startInfoArray[i]));
            //Setup the members of STARTUPINFO. This structure specified the STDIN and STDOUT handles for redirection.
            startInfoArray[i].cb = sizeof(startInfoArray[i]);
            startInfoArray[i].hStdError = childStdOutWrArray[i];
            startInfoArray[i].hStdOutput = childStdOutWrArray[i];
            startInfoArray[i].dwFlags |= STARTF_USESTDHANDLES;
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
            if(!CreateProcess(NULL,cmdLine[i],NULL,NULL,TRUE,0,NULL,NULL,&startInfoArray[i],&processInfoArray[i]))
            {
                cout << "CreateProcess failed at process #" << 0 << endl;
                return 1;
            }
        }

        HANDLE* handles = new HANDLE[numOfProcess];

        //Wait for child process to exit.
        for(int i=0; i < numOfProcess; ++i)
        {
            handles[i] = processInfoArray[i].hProcess;
            //WaitForSingleObject(processInfoArray[i].hProcess, INFINITE);
        }

        //The output pipe look.
        for(;;)
        {
            DWORD dwRc;
            dwRc = WaitForMultipleObjects(numOfProcess,handles,true,500); //true is wait for all. Wait for 500ms.

            for(int i=0; i < numOfProcess; ++i)
            {
                redirectStdout(childStdOutRdArray[i],i);
            }

            if(dwRc != WAIT_TIMEOUT) //We only care about continuing the loop if we timed out. Otherwise, this means wait exited other than timing out. So break!
                break;

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
        }



        return retCode;
    }
#ifdef __cplusplus
}
#endif