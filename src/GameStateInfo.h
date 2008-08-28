/*
 * GameStateInfo.h
 *
 *  Created on: Aug 27, 2008
 *      Author: bey0nd
 */

#ifndef GAMESTATEINFO_H_
#define GAMESTATEINFO_H_

#include <string>
#include <map>

using namespace std;

namespace ZGame
{
  class GameStateInfo
  {
  public:

    GameStateInfo();
    virtual
    ~GameStateInfo();

    std::string prevState;
    std::string nextState;
    std::string key;
    std::string gameStateClass;

    //GameState* gameState;
    enum GameStateType {STATELESS,STATEFUL};

    GameStateType stateType;

  };

  typedef map<std::string, ZGame::GameStateInfo> GameStateInfoMap;
  typedef GameStateInfoMap::iterator GameStateInfoMapItr;

}

#endif /* GAMESTATEINFO_H_ */
