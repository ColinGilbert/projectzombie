/*
 * ZTestSuite.h
 *
 *  Created on: Sep 20, 2008
 *      Author: bey0nd
 */

#ifndef ZTESTSUITE_H_
#define ZTESTSUITE_H_


namespace ZGame
{
class EngineView;
}
class ZTestConfig
{
public:

  ZTestConfig();
  ~ZTestConfig();

  static const Ogre::Root* getRoot() { return _root; }
  static const Ogre::SceneManager* getScnMgr() { return _scnMgr; }
  static const Ogre::RenderWindow* getWindow() { return _window; }

protected:

  static Ogre::Root* _root;
  static Ogre::SceneManager* _scnMgr;
  static Ogre::RenderWindow* _window;

  ZGame::EngineView* _engineView;


  bool init();
  bool loadAssets();
  Ogre::Camera* createDefaultCamera();

};

#endif /* ZTESTSUITE_H_ */
