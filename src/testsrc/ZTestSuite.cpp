/*
 * ZTestSuite.cpp
 *
 *  Created on: Sep 20, 2008
 *      Author: bey0nd
 */

#include <iostream>
#include <runtime_error>
using namespace std;

#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE ZombieTesting

#include <boost/test/unit_test.hpp>
#include <Ogre.h>
using namespace Ogre;
#include "ZTestSuite.h"
#include "EngineView.h"

Root* ZTestConfig::_root = 0;
SceneManager* ZTestConfig::_scnMgr = 0;
RenderWindow* ZTestConfig::_window = 0;

ZTestConfig::ZTestConfig()
{
  cout << "In ZTestConfig()" << endl;
  if(!init())
    {
      throw std::runtime_error("Failed in ZTestConfig");
    }
}

ZTestConfig::~ZTestConfig()
{
  cout << "In ~ZTestConfig()" << endl;
  if(_root)
    delete _root;
}

bool ZTestConfig::init()
{
  cout << "In ZTestConfig::init()" << endl;
  _root = new Ogre::Root("plugins.cfg");
   if (_root->showConfigDialog())
     {
       _window = _root->initialise(true);
     }
   else
     return false;

   _scnMgr = _root->createSceneManager(Ogre::ST_GENERIC, "ExampleSMInstance");

   loadAssets();
   Ogre::Camera* cam = createDefaultCamera();
   Ogre::Viewport* vp = _window->addViewport(cam);
   vp->setBackgroundColour(Ogre::ColourValue(1.0, 0.0, 0.0));

   cam->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));

   _engineView = new ZGame::EngineView(_window,cam,_scnMgr);

   return true;
}

bool ZTestConfig::loadAssets()
{
  Ogre::ConfigFile cf;
  string resourcePath;
  resourcePath = "";
  cf.load(resourcePath + "resources.cfg");
  //go thourhg all sections & settings in the file
  Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

  Ogre::String secName, typeName, archName;
  while (seci.hasMoreElements())
    {
      secName = seci.peekNextKey();
      Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
      Ogre::ConfigFile::SettingsMultiMap::iterator i;
      for (i = settings->begin(); i != settings->end(); ++i)
        {
          typeName = i->first;
          archName = i->second;
          Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
              resourcePath + archName, typeName, secName);
        }
    }
  Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
  return true;
}

Ogre::Camera* ZTestConfig::createDefaultCamera()
{
  Camera* cam = _scnMgr->createCamera("ENGINE_VIEW_CAMERA");
  cam->setPosition(0, 0, 1600);
  cam->lookAt(0, 0, -1);
  cam->setNearClipDistance(1.0);
  return cam;
}

BOOST_GLOBAL_FIXTURE(ZTestConfig);

