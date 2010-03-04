#include <iostream>
#include <csignal>

#include "ServerController.h"
#include "MessageIdentifiers.h"


bool STILL_RUNNING = true;

//This function is to handle User signal (ctrl x,c in console) 
void stop_running_signal_handler(int signum)
{
    STILL_RUNNING = false;
}


namespace ZGame
{
    ServerController::ServerController()
    {
    }

    ServerController::~ServerController()
    {

    }

    bool
        ServerController::onInit()
    {
        using namespace Ogre;

        signal(SIGINT,stop_running_signal_handler); //register for user initiated signal.

        _root.reset(new Ogre::Root("plugins.cfg","pchaos_server_display.cfg","Pchaos_server.log"));
        if(_root->showConfigDialog())
        {
            _root->initialise(false);
        }
        else
            return false;

        Ogre::LogManager::getSingleton().setLogDetail(Ogre::LL_BOREME);

        _serverController.onInit();
        return true;

    }

    void
        ServerController::run()
    {
        //Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL,"In ServerController run.");
        while(STILL_RUNNING) //run forever. The program handles system interrupts (ctrl-c in console) for server shutdown event.
        {
            _serverController.onUpdate();
        }
    }



    void
        ServerController::onDestroy()
    {
        _serverController.onDestroy();
        Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL,"Shutting down server.");
    }
}
