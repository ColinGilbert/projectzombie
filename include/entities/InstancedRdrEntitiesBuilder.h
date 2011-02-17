#pragma once

#include <string>
#include <vector>
#include <Ogre.h>

namespace ZGame
{
    namespace Entities
    {
        /**
        *This class will build Ogre InstancedGeometry. THis is used for downstream classes which requires instanced geometry (such as RdrEntitiesManager.)
        *Code concepts and ideas taken liberally from Ogre SDK Sample on InstancedGeometry. Please review those for further clarifications.
        **/
        class InstancedRdrEntitiesBuilder
        {
        public:
            static const size_t MAX_PER_BATCH;
            InstancedRdrEntitiesBuilder(Ogre::SceneManager* scnMgr);
            virtual ~InstancedRdrEntitiesBuilder();

            void build(size_t numOfMeshes, std::vector<Ogre::InstancedGeometry *> &renderInstance, std::string meshName);
        protected:
        private:
            void _chkVertexProgram();
            /** \brief This method will setup material for the geometry instance--instance material. Code taken from Ogre SDK Samples.**/
            void setupInstancedMaterialToEntity(Ogre::Entity* ent);
            Ogre::String buildInstancedMaterial(const Ogre::String &originalMaterialName);
        private:
            Ogre::SceneManager* _scnMgr;

        };
    }
}