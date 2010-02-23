/*
* StatesLoader.h
*
*  Created on: Aug 27, 2008
*      Author: bey0nd
*/

#ifndef STATESLOADER_H_
#define STATESLOADER_H_

#include "GameStateInfo.h"

namespace ZGame
{
    class StatesLoader
    {
    public:
        StatesLoader();
        virtual
            ~StatesLoader();

        void loadStates(GameStateInfoMap& stateInfoMap,GameStateInfo &startState);

    protected:
        void fillSInfo(GameStateInfo &info,const Ogre::String key,const Ogre::String type, const GameStateInfo::GameStateType);
        void addSInfo(GameStateInfoMap& stateInfoMap,GameStateInfo &info);

    };
}
#endif /* STATESLOADER_H_ */
