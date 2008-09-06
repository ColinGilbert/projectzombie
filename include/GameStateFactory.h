/*
 * GameStateFactory.h
 *
 *  Created on: Aug 28, 2008
 *      Author: bey0nd
 */

#ifndef GAMESTATEFACTORY_H_
#define GAMESTATEFACTORY_H_

#include <string>
#include "GameEditState.h"
#include "GameMainMenuState.h"
namespace ZGame
{
class GameStateFactory
{
public:
  GameStateFactory();
  virtual
  ~GameStateFactory();

  static GameState* createGameState(const std::string gameStateClass);
};
}
#endif /* GAMESTATEFACTORY_H_ */
