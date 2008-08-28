/*
 * StatesLoader.cpp
 *
 *  Created on: Aug 27, 2008
 *      Author: bey0nd
 */

#include "StatesLoader.h"

using namespace ZGame;

StatesLoader::StatesLoader()
{
  // TODO Auto-generated constructor stub

}

StatesLoader::~StatesLoader()
{
  // TODO Auto-generated destructor stub
}

void StatesLoader::loadStates(GameStateInfoMap &stateInfoMap, GameStateInfo &startState)
{
  GameStateInfo info;

  fillSInfo(info,"GameEditStateKey","GameEditState",GameStateInfo::STATELESS);
  addSInfo(stateInfoMap,info);

  startState = info;

  fillSInfo(info,"GameMainMenuStateKey","GameMainMenuState",GameStateInfo::STATELESS);
  addSInfo(stateInfoMap,info);
}

void StatesLoader::addSInfo(GameStateInfoMap &stateInfoMap, GameStateInfo &info)
{
  GameStateInfoMapItr it = stateInfoMap.find(info.key);
  if(it == stateInfoMap.end())
    stateInfoMap[info.key]=info;
  //else throw exception
}

void StatesLoader::fillSInfo(GameStateInfo &info,const string key, const string stateClass,GameStateInfo::GameStateType type)
{
  info.key = key;
  info.gameStateClass  = stateClass;
  info.stateType = type;
}





