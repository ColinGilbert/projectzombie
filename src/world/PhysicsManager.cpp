#include "world/PhysicsManager.h"
#include "EngineView.h"
using ZGame::World::PhysicsManager;
using namespace Ogre;
PhysicsManager::PhysicsManager() : _bounds(-200, -200, -200, 200, 200, 200), _gravityVector(0.0, -9.8, 0.0)
{
}

PhysicsManager::~PhysicsManager()
{
    using OgreBulletDynamics::DynamicsWorld;
    OGRE_DELETE_T(_world, DynamicsWorld, Ogre::MEMCATEGORY_GENERAL);
    OGRE_DELETE _debugDrawer;
}

bool
    PhysicsManager::onInit()
{
    
    using ZGame::EngineView;
    _scnMgr = EngineView::getSingleton().getSceneManager();
    _world = OGRE_NEW_T(OgreBulletDynamics::DynamicsWorld(_scnMgr, _bounds, _gravityVector), Ogre::MEMCATEGORY_GENERAL);
    _debugDrawer = OGRE_NEW OgreBulletCollisions::DebugDrawer();
    _debugDrawer->setDrawWireframe(true);

    _world->setDebugDrawer(_debugDrawer);
    _world->setShowDebugShapes(true);

    SceneNode* node = _scnMgr->getRootSceneNode()->createChildSceneNode("debugDrawer", Vector3::ZERO);
    node->attachObject(static_cast<SimpleRenderable* >(_debugDrawer));

    return true;
}

bool
    PhysicsManager::onUpdate(const Ogre::FrameEvent &evt)
{
    return true;
}

bool 
    PhysicsManager::onDestroy()
{
    return true;
}

