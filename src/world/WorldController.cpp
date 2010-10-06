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
_worldMap(0), _scnMgr(0), _terrainGlobals(0), _terrainGroup(0),_terrainsImported(false)
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
    delete _worldMap;
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
    loadTerrain();
    loadWorldMap();
    loadSkyMap();
    //log->logMessage(Ogre::LML_TRIVIAL,"Out WorldController::init().");
}

void WorldController::loadSkyMap()
{
    _scnMgr->setSkyBox(true, "Examples/SpaceSkyBox", 80000, false);
}

void 
    WorldController::loadWorldMap()
{
    _worldMap = new WorldMap();
    _worldMap->load(); //We should implement load from configuration file.
}
/** 
*This method will load a terrain. Currently we are using the Ogre default Paging Terrain system.
*/
void
    WorldController::loadTerrain()
{
    Ogre::Log::Stream ls = Ogre::LogManager::getSingleton().getDefaultLog()->stream(Ogre::LML_NORMAL);
    Camera* cam = EngineView::getSingleton().getCurrentCamera();
    cam->setNearClipDistance(50.0f);
    cam->setFarClipDistance(0.0f);
    ls << "WorldController is loading terrain.\n";
    ls << "Creating lights.\n";
    //First build the lights.
    Vector3 lightDir(-0.5f, -0.3f, -0.3f);
    lightDir.normalise();
    _scnMgr->setAmbientLight(Ogre::ColourValue(0.0, -0.1, 1.0));
    Light* light = _scnMgr->createLight("terrainLight");
    light->setType(Light::LT_DIRECTIONAL);
    light->setDirection(lightDir);
    light->setDiffuseColour(ColourValue(1.0, 1.0, 1.0));
    light->setSpecularColour(ColourValue(1.0f, 1.0f, 1.0f));

    _scnMgr->setAmbientLight(ColourValue(0.3f, 0.3f, 0.3f));

    ls << "Configuring terrain.\n";
    //Configure terrain.
    _terrainGlobals = OGRE_NEW Ogre::TerrainGlobalOptions();
    _terrainGroup = OGRE_NEW Ogre::TerrainGroup(_scnMgr, Ogre::Terrain::ALIGN_X_Z, 513, 50825.22820697603f);
    _terrainGroup->setFilenameConvention(Ogre::String("ProjectChaosTerrain"), Ogre::String("dat"));
    _terrainGroup->setOrigin(Ogre::Vector3::ZERO);

    _configTerrainDefaults(light);
    for(long x=0; x<=0; ++x)
        for(long y=0; y<=0; ++y)
            _defineTerrain(x, y);
    _terrainGroup->loadAllTerrains(true);

    if(_terrainsImported)
    {
        Ogre::TerrainGroup::TerrainIterator ti = _terrainGroup->getTerrainIterator();
        const Real terrainW = 50825.22820697603f;
        const Real cx = terrainW / 2.0f;
        while(ti.hasMoreElements())
        {
            Terrain* t = ti.getNext()->instance;
            t->setPosition(Ogre::Vector3(cx, 0.0f, cx));
            //_initBlendMaps(t);
        }
    }
    _terrainGroup->freeTemporaryResources();
    ls << "WorldController Terrain done loading.\n";
}

void
    WorldController::_configTerrainDefaults(Light* light)
{
    _terrainGlobals->setMaxPixelError(8);
    _terrainGlobals->setCompositeMapDistance(50000);
    _terrainGlobals->setLightMapDirection(light->getDerivedDirection());
    _terrainGlobals->setCompositeMapAmbient(_scnMgr->getAmbientLight());
    _terrainGlobals->setCompositeMapDiffuse(light->getDiffuseColour());
    
    Terrain::ImportData& defaultimp = _terrainGroup->getDefaultImportSettings();
    defaultimp.terrainSize = 513;
    defaultimp.worldSize = 50825.22820697603f;
    defaultimp.inputScale = 3500;
    defaultimp.minBatchSize = 33;
    defaultimp.maxBatchSize = 65;

    //textures
    /**
    defaultimp.layerList.resize(3);
    defaultimp.layerList[0].worldSize = 421;
    defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_diffusespecular.dds");
    defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_normalheight.dds");
    defaultimp.layerList[1].worldSize = 127;
    defaultimp.layerList[1].textureNames.push_back("grass_green-01_diffusespecular.dds");
    defaultimp.layerList[1].textureNames.push_back("grass_green-01_normalheight.dds");
    defaultimp.layerList[2].worldSize = 848;
    defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_diffusespecular.dds");
    defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_normalheight.dds");
    **/
    defaultimp.layerList.resize(2);
    defaultimp.layerList[0].worldSize = 4000;
    //defaultimp.layerList[0].textureNames.push_back("tron_detail.dds");
    defaultimp.layerList[0].textureNames.push_back("tron2_diffuse.dds");
    defaultimp.layerList[0].textureNames.push_back("tron2_normal.dds");
    /*
    defaultimp.layerList[1].worldSize = 1000;
    defaultimp.layerList[1].textureNames.push_back("tron_diffuse.dds");
    defaultimp.layerList[1].textureNames.push_back("tron_normal.dds");
    */
}

void
    WorldController::_defineTerrain(long x, long y)
{
    Ogre::String filename = _terrainGroup->generateFilename(x, y);
    if(Ogre::ResourceGroupManager::getSingleton().resourceExists(_terrainGroup->getResourceGroup(), filename))
    {
        _terrainGroup->defineTerrain(x, y);
    }
    else
    {
        Ogre::Image img;
        bool flipX; bool flipY;
        flipX = x % 2 != 0;
        flipY = y % 2 != 0;
        img.load("city3.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
        if(flipX)
            img.flipAroundY();
        if(flipY)
           img.flipAroundX();
        _terrainGroup->defineTerrain(x, y, &img);
        _terrainsImported = true;
    }
}

void
    WorldController::_initBlendMaps(Ogre::Terrain* terrain)
{
    Ogre::TerrainLayerBlendMap* blendMap0 = terrain->getLayerBlendMap(1);
    Ogre::TerrainLayerBlendMap* blendMap1 = terrain->getLayerBlendMap(2);
    Ogre::Real minHeight0 = 70;
    Ogre::Real fadeDist0 = 40;
    Ogre::Real minHeight1 = 70;
    Ogre::Real fadeDist1 = 15;
    float* pBlend1 = blendMap1->getBlendPointer();
    for (Ogre::uint16 y = 0; y < terrain->getLayerBlendMapSize(); ++y)
    {
        for (Ogre::uint16 x = 0; x < terrain->getLayerBlendMapSize(); ++x)
        {
            Ogre::Real tx, ty;
 
            blendMap0->convertImageToTerrainSpace(x, y, &tx, &ty);
            Ogre::Real height = terrain->getHeightAtTerrainPosition(tx, ty);
            Ogre::Real val = (height - minHeight0) / fadeDist0;
            val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
 
            val = (height - minHeight1) / fadeDist1;
            val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
            *pBlend1++ = val;
        }
    }
    blendMap0->dirty();
    blendMap1->dirty();
    blendMap0->update();
    blendMap1->update();

}
