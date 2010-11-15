#pragma once

/**
* \file This file contain source code to implement a manager for managing Physics based objects.
*
*/
#include <deque>
#include <Ogre.h>
#include <OgreBulletDynamicsRigidBody.h>
#include <OgreBulletCollisions.h>

namespace ZGame
{
    namespace World
    {
        class PhysicsManager
        {
        public:
            PhysicsManager();
            virtual 
                ~PhysicsManager();
            bool
                onInit();
            bool
                onDestroy();
            bool
                onUpdate(const Ogre::FrameEvent &evt);
        protected:

        private:
        private:
            Ogre::SceneManager* _scnMgr;
            OgreBulletDynamics::DynamicsWorld* _world;
            OgreBulletCollisions::DebugDrawer* _debugDrawer;
            std::deque<OgreBulletDynamics::RigidBody* > _bodies;
            std::deque<OgreBulletCollisions::CollisionShape* > _shapes;
            Ogre::AxisAlignedBox _bounds;
            Ogre::Vector3 _gravityVector;
        };
    }
}