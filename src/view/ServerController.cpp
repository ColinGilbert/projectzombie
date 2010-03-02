#include <iostream>

#include "ServerController.h"
#include "MessageIdentifiers.h"


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
        while(1) //run forever. The program handles system interrupts (ctrl-c in console) for server shutdown event.
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
