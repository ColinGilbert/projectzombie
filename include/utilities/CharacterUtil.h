#ifndef _ZGAME_CHARUTIL_H
#define _ZGAME_CHARUTIL_H

#include "ZPrerequisites.h"
#include <boost/random.hpp>
#include "GPUEntsDistributor.h"

namespace ZGame
{
    namespace Util
    {
        class CharacterUtil
        {
        public:
            CharacterUtil();
            ~CharacterUtil(){}

            bool
                onInit(ZGame::ZInitPacket *initPacket);

            void setInput();

            //Creates a new character.
            bool create(const Ogre::StringVector &params);
            //Utility functions (mainly used by ConsoleCommands
            bool list(const Ogre::StringVector &params);
            //Utility function for list meshes
            bool listMeshes(const Ogre::StringVector &params);

            bool listNodes(const Ogre::StringVector &params);
            /** \brief This function will remove a given node. */
            bool removeNode(const Ogre::StringVector &params);


        private:

            Ogre::SceneManager* _scnMgr;


            std::map<Ogre::String,Ogre::SceneNode*> _charNodes;
            typedef std::map<Ogre::String,Ogre::SceneNode*>::iterator CHAR_ITR;

            Ogre::Vector3 _minExt; //minExtent.
            Ogre::Vector3 _maxExt; //maxExtent
            boost::minstd_rand _rng;
            boost::uniform_int<> _xDist;
            boost::uniform_int<> _zDist;
            GPUEntsDistributor<boost::minstd_rand,boost::uniform_int<> > _dist;

            enum CHAR_CMDS_ENUM
            {
                LIST,
                CREATE,
                LISTMESH
            };
            void createCharacters();

            //void listChar();
            void listAllMeshes();
            void listAllChars();
            bool createCharFromMesh(const Ogre::StringVector &params);
            //print the usage.
            void printUsage(CHAR_CMDS_ENUM CMD_ENUM);

            bool removeNodeWithName(const Ogre::String &nodeName);

        };
    }
}

#endif