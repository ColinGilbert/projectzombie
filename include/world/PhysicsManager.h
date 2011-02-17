#pragma once

/**
* \file This file contain source code to implement a manager for managing Physics based objects.
*
*/
//#include <deque>
#include "ZPrerequisites.h"
#include <Ogre.h>
#include <OgreBulletDynamicsRigidBody.h>
#include <OgreBulletCollisions.h>
//#include <Utils/OgreBulletCollisionsMeshToShapeConverter.h>

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
                onInit(ZGame::ZInitPacket *initPacket);
            bool
                onDestroy();
            bool
                onUpdate(const Ogre::FrameEvent &evt);
            /** \brief This method assumes managment responsiblity for the given shape.**/
            OgreBulletDynamics::RigidBody*
                staticObjectFromManual(Ogre::ManualObject* manual,
                OgreBulletDynamics::RigidBody* currentBody = 0,
                float restitution = 0.6f, float friction = 0.6f,
                const Ogre::Matrix4 &transform=Ogre::Matrix4::IDENTITY,
                const Ogre::String &name=""
                );

            
            OgreBulletCollisions::CompoundCollisionShape*
                createCompoundShape();
           


            void
                destroyBody(OgreBulletDynamics::RigidBody* body);

            void 
                addCube(const Ogre::Vector3 &pos);

            void
                addCubeToCompoundShape(const Ogre::Vector3 &pos,
                OgreBulletCollisions::CompoundCollisionShape* compoundShape);

            OgreBulletDynamics::RigidBody* 
                createVolumeRigidyBodyFromCompoundShape(OgreBulletCollisions::CompoundCollisionShape* compoundShape,
                Ogre::SceneNode* node,
                OgreBulletDynamics::RigidBody* currentBody = 0,
                float restitution = 0.6f, float friction = 0.6f,
                const Ogre::String &name=""
                );
                



            OgreBulletCollisions::ConvexHullCollisionShape* createConvexHullShape();

            bool
                getCollisionPoint(Ogre::Vector3 &intersectPoint, Ogre::Ray &rayTo, Ogre::Real farClipDistance);

        protected:

        private:
            void _freeAll();
        private:
            Ogre::SceneManager* _scnMgr;
            OgreBulletDynamics::DynamicsWorld* _world;
            OgreBulletCollisions::DebugDrawer* _debugDrawer;
            typedef Ogre::map<OgreBulletDynamics::RigidBody*, OgreBulletDynamics::RigidBody* >::type RBODY_MAP;
            typedef Ogre::map<OgreBulletCollisions::CollisionShape*, OgreBulletCollisions::CollisionShape* >::type CSHAPE_MAP;
            RBODY_MAP _bodies;
            CSHAPE_MAP _shapes;
            //std::deque<OgreBulletDynamics::RigidBody* > _bodies;
            //std::deque<OgreBulletCollisions::CollisionShape* > _shapes;
            Ogre::AxisAlignedBox _bounds;
            Ogre::Vector3 _gravityVector;


        private:

             OgreBulletCollisions::BoxCollisionShape*
                _createBoxShape();
            /*
            class ManualObjectToShapeConverter : public OgreBulletCollisions::VertexIndexToShape
            {
            public:
            ManualObjectToShapeConverter();
            ~ManualObjectToShapeConverter();

            void
            addManualObject(Ogre::ManualObject* manual);
            };
            */
        };
    }
}