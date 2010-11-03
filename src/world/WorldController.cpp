#include <sstream>
#include <iostream>



using std::cout; using std::endl;

#include "world/WorldController.h"

using namespace Ogre;
//#include "cologre.h"
#include "EngineView.h"
#include "world/WorldMap.h"
using namespace ZGame;
using namespace ZGame::World;

WorldController::WorldController() :
_worldMap(0), _scnMgr(0) 
{
    //init();
}

WorldController::~WorldController()
{
}


//Life cycle events
bool WorldController::onInit()
{
    init();
    return true;
}

bool WorldController::onUpdate(const Ogre::FrameEvent &evt)
{
    using namespace std;
    Ogre::Real inc = evt.timeSinceLastFrame*0.8f;

    //_animState->addTime(inc);
    //_animState->setWeight(1.0);

    //cout << "WorldController::onUpdate" << endl;
    return true;
}

bool WorldController::onDestroy()
{
    cout << "WorldController::onDestroy called." << endl;
    cout << "Deleting WorldMap." << endl;
    _worldMap.reset(0);
    cout << "WorldController::onDestroy done." << endl;
    return true;
}

/**
*This method will initialize the world controller.
*
*\note We're putting some world load stuff in the init function because we're testing. When we get to implementing this fully we nee to have people call
*the controller method to accomplish loading the world. ie. to do the world controller thing.
*/
void WorldController::init()
{
    _scnMgr = EngineView::getSingleton().getSceneManager();
    //loadTerrain();
    //loadWorldMap();
    //loadSkyMap();
    //log->logMessage(Ogre::LML_TRIVIAL,"Out WorldController::init().");
}

void 
    WorldController::loadWorldMap()
{
    _worldMap.reset(new WorldMap());
    _worldMap->load(); //We should implement load from configuration file.
}
