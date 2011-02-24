/*
 * Workspace.cpp
 *
 *  Created on: Aug 23, 2010
 *      Author: beyzend
 */
#include "ZWorkspace.h"
#include "entities/EntitiesManager.h"
#include "entities/RenderEntitiesManager.h"
#include "ZCL/ZCLController.h"
#include "world/WorldController.h"
#include "world/PhysicsManager.h"
#include "world/CinematicController.h"
#include "geometry/GeometryManager.h"
#include "toolset/ToolsetController.h"
#include "entities/ComponentController.h"
#include "world/GameController.h"
using namespace ZGame;
using ZGame::ZWorkspace;

size_t ZWorkspace::_ID = 0;

ZWorkspace::ZWorkspace(Ogre::SceneManager* scnMgr, Entities::ComponentController* componentCtrl, OgreBites::SdkTrayManager* sdkTray, ZCL::ZCLController* zclCtrl,
    World::WorldController* worldCtrl, World::GameController* gameCtrl, World::CinematicController* cineCtrl, 
    Geometry::GeometryManager* geoMgr, 
    Toolset::ToolsetController* toolsetCtrl) : _scnMgr(scnMgr),
    _componentCtrl(componentCtrl), _tray(sdkTray),
    _zclCtrl(zclCtrl), _worldCtrl(worldCtrl), _workspaceRoot(scnMgr->getRootSceneNode()->createChildSceneNode("WorkspaceNode")),
    _cineCtrl(cineCtrl), _geoMgr(geoMgr), _toolsetCtrl(toolsetCtrl), _gameCtrl(gameCtrl)
{
    _toolsetCtrl->addListener(_cineCtrl);
}

ZWorkspace::~ZWorkspace()
{
}

void
ZWorkspace::_getKey(Ogre::String &key)
{
  std::ostringstream oss;
  oss << "WRK_ICON" << _ID++;
  key = oss.str();
}

void
ZWorkspace::_removeIcons()
{
  for(size_t i = 0; i < _icons.size(); ++i)
    {
      Ogre::SceneNode* node = _icons[i];
      _scnMgr->destroySceneNode(node);
    }
  _icons.clear();
}

void
ZWorkspace::resetAll()
{
  //remove icons
  _removeIcons();
}



Ogre::SceneNode*
ZWorkspace::createIcon()
{
  using Ogre::String;
  using Ogre::SceneNode;
  //Create an ICON node.
  Ogre::String key;
  _getKey(key);
  Ogre::Entity* iconEnt = _scnMgr->createEntity("ninja.mesh");
  Ogre::SceneNode* node = _workspaceRoot->createChildSceneNode(key);
  node->attachObject(iconEnt);
  node->setScale(10.0f, 10.0f, 10.0f);
  _icons.push_back(node);
  return node;

}

void
ZWorkspace::buildGroups()
{
}

void
ZWorkspace::updateGroupGoals()
{
}




