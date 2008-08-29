/*
 * GameStateFactory.cpp
 *
 *  Created on: Aug 28, 2008
 *      Author: bey0nd
 */
#include <stdexcept>
#include "GameStateFactory.h"

namespace ZGame
{
GameStateFactory::GameStateFactory()
{
  // TODO Auto-generated constructor stub

}

GameStateFactory::~GameStateFactory()
{
  // TODO Auto-generated destructor stub
}

GameState* GameStateFactory::createGameState(const std::string gameStateClass)
{
  if(gameStateClass.compare("GameEditState") == 0)
    return new GameEditState();
  else
    throw(invalid_argument("The GameState you are trying to create is invalid!"));
}

}


