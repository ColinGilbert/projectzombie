/*
* VolumeMapView.h
*
*  Created on: Sep 21, 2010
*      Author: beyzend
*/

#pragma once

#include <vector>
#include <Ogre.h>
#include <SurfaceMesh.h>
#include <OgreBulletDynamics.h>
using std::vector;

namespace ZGame
{
    namespace World
    {
        class PhysicsManager;
        /**
        * This class will handle the representation layer of Volume Maps. It's main responsibility is to render the VolumeMap.
        */
        class VolumeMapView
        {
        public:
            VolumeMapView();
            virtual
                ~VolumeMapView();

            void
                createRegion(const Ogre::Vector3 &origin, PolyVox::SurfaceMesh<PolyVox::PositionMaterial>* mesh);
            void
                updateRegion(PolyVox::SurfaceMesh<PolyVox::PositionMaterial> *mesh);
            void
                unloadRegion(PhysicsManager* phyMgr);
            void
                createPhysicsRegion(PhysicsManager* phyMgr);
                //OgreBulletCollisions::ConvexHullCollisionShape* shape);
                

            Ogre::ManualObject*
                getManualObject()
            {
                return _manual;
            }
            void
                finalizeRegion();
            const Ogre::Vector3&
                getOrigin()
            {
                return _origin;
            }
            void
                updateOrigin(const Ogre::Vector3 &origin)
            {
                _origin = origin;
            }

        protected:



        private:

            static size_t man_count;

            Ogre::ManualObject* _manual;
            Ogre::SceneNode* _root;
            Ogre::Vector3 _origin; //the origin of this block
            Ogre::SceneManager* _scnMgr;
            OgreBulletDynamics::RigidBody* _phyBody;
            void
                _initManualObject();
            /** \brief This method will create a manual mesh from the given PolyVox SurfaceMesh.**/
            void
                _manualFromMesh(bool isUpdate, PolyVox::SurfaceMesh<PolyVox::PositionMaterial>* mesh, Ogre::ManualObject* manual);

            void
                _convertColorFromMaterialID(Ogre::ColourValue &val, uint8_t materialID);

        };
    }
}
