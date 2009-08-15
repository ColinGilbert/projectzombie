/*
 * StatesLoader.cpp
 *
 *  Created on: Aug 27, 2008
 *      Author: bey0nd
 */
#include <Ogre.h>
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

  Ogre::ConfigFile cf;
  cf.load("states.cfg");

  Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

  Ogre::String secName, typeName, archName, startName;

  while(seci.hasMoreElements())
  {
    
    secName = seci.peekNextKey();
    Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
    Ogre::ConfigFile::SettingsMultiMap::iterator i;
    for(i = settings->begin(); i!= settings->end(); ++i)
    {
       
       typeName = i->first;
       archName = i->second;
       cout << "SEC_NAME,TYPENAME,ARCHNAME:" << secName << " " << typeName << " " << archName << endl;
       
      if(secName.compare("BeginState") == 0)
      {
        startName = archName;
      }
      else
      {
        GameStateInfo info;
        fillSInfo(info,typeName,archName,GameStateInfo::STATELESS);
        addSInfo(stateInfoMap,info);
        if(typeName.compare(startName) == 0)
        {
          startState = info;
        }
      }
      
    }
  }

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





