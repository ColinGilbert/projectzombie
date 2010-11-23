#include "world/PhysicsManager.h"
#include "EngineView.h"
#include <Shapes/OgreBulletCollisionsConvexHullShape.h>
#include <Shapes/OgreBulletCollisionsBoxShape.h>
#include <Utils/OgreBulletCollisionsMeshToShapeConverter.h>
#include <Shapes/OgreBulletCollisionsTrimeshShape.h>
#include <OgreBulletCollisionsRay.h>
#include <iostream>
using std::cout;
using std::endl;

using ZGame::World::PhysicsManager;
using namespace Ogre;

enum QueryFlags
{
	ANY_QUERY_MASK					= 1<<0,
	RAGDOLL_QUERY_MASK				= 1<<1,
	GEOMETRY_QUERY_MASK				= 1<<2,
	VEHICLE_QUERY_MASK				= 1<<3,
	STATIC_GEOMETRY_QUERY_MASK		= 1<<4
};

PhysicsManager::PhysicsManager() : _bounds(-3000, -3000, -3000, 3000, 3000, 3000), _gravityVector(0.0, -9.8, 0.0)
{
}

PhysicsManager::~PhysicsManager()
{
    using OgreBulletDynamics::DynamicsWorld;
    _freeAll();
    _world->setDebugDrawer(0);
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
    _debugDrawer->setDrawAabb(true);
    
    _debugDrawer->setDebugDisplayEnabled(true);
    
    _world->setDebugDrawer(_debugDrawer);
    _world->setShowDebugShapes(true);
    

    SceneNode* node = _scnMgr->getRootSceneNode()->createChildSceneNode("debugDrawer", Vector3::ZERO);
    node->attachObject(static_cast<SimpleRenderable* >(_debugDrawer));
    node->setVisible(true, true);
    //_debugDrawer->
    return true;
}

bool
    PhysicsManager::onUpdate(const Ogre::FrameEvent &evt)
{
    _world->stepSimulation(evt.timeSinceLastFrame);
    //_debugDrawer->draw();
   
    return true;
}

bool 
    PhysicsManager::onDestroy()
{
    return true;
}

OgreBulletCollisions::ConvexHullCollisionShape*
    PhysicsManager::createConvexHullShape()
{
    using namespace OgreBulletCollisions;
    ConvexHullCollisionShape* shape = new ConvexHullCollisionShape();
    //_shapes.push_back(static_cast<CollisionShape*>(shape));
    _shapes[shape] = shape;
    return shape;
}

void PhysicsManager::_freeAll()
{
    RBODY_MAP::iterator bodyIter;
    CSHAPE_MAP::iterator shapeIter;

    for(bodyIter = _bodies.begin(); bodyIter != _bodies.end(); ++bodyIter)
    {
        delete bodyIter->second;
    }
    _bodies.clear();
    for(shapeIter = _shapes.begin(); shapeIter != _shapes.end(); ++shapeIter)
    {
        delete shapeIter->second;
    }
    _shapes.clear();
}
static size_t cubeIdx = 0;
void
    PhysicsManager::addCube(const Ogre::Vector3 &pos)
{
    using namespace OgreBulletCollisions;
    using namespace OgreBulletDynamics;
    using namespace Ogre;
    const float bodyMass = 1.0f;
    const float bodyRes = 0.6;
    const float bodyFriction = 0.6f;
    Entity* entity = _scnMgr->createEntity("cube" + Ogre::StringConverter::toString(cubeIdx++), "Bulletbox.mesh");
    entity->setQueryFlags(GEOMETRY_QUERY_MASK);
    const Ogre::Vector3 cubeBodyBounds(1, 1, 1);
    BoxCollisionShape* cubeShape = new BoxCollisionShape(cubeBodyBounds);
    RigidBody *defaultBody = new RigidBody(
        "defaultcuberigid" + StringConverter::toString(_bodies.size()),
        _world);

    SceneNode* node = _scnMgr->getRootSceneNode()->createChildSceneNode();
    node->attachObject(entity);

    defaultBody->setShape(node, cubeShape, bodyRes, bodyFriction, bodyMass, pos);
    
    _bodies[defaultBody] = defaultBody;
    _shapes[cubeShape] = cubeShape;

}
/**
** \note this method interface should be refactored to correspond to the way we are assuming about reusing static rigid bodies
*relating to order or destruction of bodies.
**/
OgreBulletDynamics::RigidBody*
    PhysicsManager::staticObjectFromManual(Ogre::ManualObject* manual,
    OgreBulletDynamics::RigidBody* currentBody,
    float restitution, float friction, const Ogre::Matrix4 &transform,
    const Ogre::String &name
    )

{
    using namespace OgreBulletDynamics;
    using namespace OgreBulletCollisions;

    assert(_manual && "Trying to let Physics Manager create a object from a null manual object.");
    //if(!currentBody)
    //{
        StaticMeshToShapeConverter shapeConvert(manual->getSection(0), transform);
        TriangleMeshCollisionShape* shape = shapeConvert.createTrimesh();

        RigidBody *shapeBody = new RigidBody("SHAPE" + Ogre::StringConverter::toString(_bodies.size()), _world);
        shapeBody->setStaticShape(shape, restitution, friction);
        //_world->addRigidBody(shapeBody, 0, 0);
    
        _bodies[shapeBody] = shapeBody; //There should be a one-to-one mapping of shapes into bodies. So don't need to check.
        _shapes[shape] = shape;
    //}
    //else
    /*{
        //FIND IT. although we assume now our propgram passes in a valid pointer.
        StaticMeshToShapeConverter shapeConvert(manual->getSection(0), transform);
        TriangleMeshCollisionShape* shape = shapeConvert.createTrimesh();
        //delete the existing shape and replace it.
        CollisionShape* currentShape = currentBody->getShape();
        delete currentShape;
        currentBody->setStaticShape(shape, restitution, friction);
    }*/
    return shapeBody;
}

void 
    PhysicsManager::destroyBody(OgreBulletDynamics::RigidBody* body)
{
    if(!body)
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Passed in pointer is null.",
                "PhysicsManager::destroyBody");
    RBODY_MAP::iterator findBody = _bodies.find(body);
    if(findBody != _bodies.end())
    {
        //delete the corresponding shape. There should be a one-to-one mapping here.
        OgreBulletCollisions::CollisionShape* shape = findBody->second->getShape();
        CSHAPE_MAP::iterator findShape = _shapes.find(shape);
        assert(findShape != _shapes.end() && "One-to-one mapping of bodies to shapes invariance violation!");
        //_world->removeObject(findBody->second);
        delete findBody->second;
        delete findShape->second;
        _bodies.erase(findBody);
        _shapes.erase(findShape);
        
        return;
    }
    OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Trying to destroy a shape that is not managed by the Manager.",
        "PhysicsManager::destroyShape");
}

bool
    PhysicsManager::getCollisionPoint(Ogre::Vector3 &intersectPoint, Ogre::Ray &rayTo, Ogre::Real farClipDistance)
{
    OgreBulletCollisions::CollisionClosestRayResultCallback callback(rayTo, _world, farClipDistance);
    _world->launchRay(callback);
    if(callback.doesCollide())
    {
        //OgreBulletDynamics::RigidBody* body = static_cast<OgreBulletDynamics::RigidBody *>
            //(callback.getCollidedObject());
        intersectPoint = callback.getCollisionPoint();
        return true;
    }
    return false;
}
