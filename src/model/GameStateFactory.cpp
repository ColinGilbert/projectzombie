/*
 * GameStateFactory.cpp
 *
 *  Created on: Aug 28, 2008
 *      Author: bey0nd
 */
#include <stdexcept>
#include "GameStateFactory.h"
#include "GameEditState.h"
#include "GameMainMenuState.h"
#include "GameMainState.h"
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

GameState* GameStateFactory::createGameState(const Ogre::String gameStateClass)
{
  //We need to change the behavior here to conform more to DRY (do not repeat yourself) principal.
  //I.E we should read some sort of file to enumerate all the game edit states, instead of using
  //a series of if, if else, else statements here.
  if(gameStateClass.compare("GameEditState") == 0)
    return new GameEditState();
  else if(gameStateClass.compare("GameMainMenuState") == 0)
    return new GameMainMenuState();
  else if(gameStateClass.compare("GameMainState") == 0)
    return new GameMainState();
  else
    throw(invalid_argument("The GameState you are trying to create is invalid!"));
}

}


