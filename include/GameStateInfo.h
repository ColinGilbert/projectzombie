/*
* GameStateInfo.h
*
*  Created on: Aug 27, 2008
*      Author: bey0nd
*/

#ifndef GAMESTATEINFO_H_
#define GAMESTATEINFO_H_

//#include <string>
#include <map>
#include <Ogre.h>
using namespace std;



namespace ZGame
{
    class GameStateInfo
    {
    public:

        GameStateInfo();
        virtual
            ~GameStateInfo();

        Ogre::String key;
        Ogre::String gameStateClass;

        //GameState* gameState;
        enum GameStateType {STATELESS,STATEFUL};

        GameStateType stateType;

    };

    typedef std::map<Ogre::String, ZGame::GameStateInfo> GameStateInfoMap;
    typedef GameStateInfoMap::iterator GameStateInfoMapItr;

}

#endif /* GAMESTATEINFO_H_ */
