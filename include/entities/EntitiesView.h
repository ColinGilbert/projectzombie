/**
 * \file THis file defines class for Entities View.
 */
#pragma once

#include <Ogre.h>
#include <SdkTrays.h>
#include <OIS/OIS.h>
#include <iostream>

using std::cout;
using std::endl;

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include <memory>
#else
#include <tr1/memory>
#endif

namespace ZGame
{
  class ZWorkspace;

  namespace Entities
  {
    class EntitiesGroup;
    class EntitiesMenu
    {
    public:
      EntitiesMenu(ZWorkspace* workspace);
      virtual
      ~EntitiesMenu()
      {
      }
      virtual bool
      handleButton(OgreBites::Button* button) = 0;
      virtual bool
      handleSelectMenu(OgreBites::SelectMenu* selectMenu) = 0;
      virtual bool
      handleSlider(OgreBites::Slider* slider) = 0;
  protected:
    OgreBites::SdkTrayManager* _tray;
    ZWorkspace* _workspace;
  };

class EntitiesCreateMenu : public EntitiesMenu
  {
  public:
    EntitiesCreateMenu(ZWorkspace* workspace);
    virtual
    ~EntitiesCreateMenu()
      {
        cout << "In EntitiesCreateMenu destructor." << endl;
      }
    virtual bool
    handleButton(OgreBites::Button* button);

    virtual bool
    handleSelectMenu(OgreBites::SelectMenu* selectMenu);
    virtual bool
    handleSlider(OgreBites::Slider* slider);

    bool
    onMouseDown(const OIS::MouseEvent &evt, const OIS::MouseButtonID id);
    bool
    onMouseMove(const OIS::MouseEvent &evt);

  private:
    static int _GRP_KEY;
    OgreBites::Label* _createLabel;
    OgreBites::Button* _addGroupButton;
    OgreBites::SelectMenu* _groupSelectMenu;
    OgreBites::Button* _centerGroupButton;
    OgreBites::Button* _createButton;
    OgreBites::Button* _resetButton;
    OgreBites::Slider* _perGroupSlider;
    OgreBites::Slider* _groupRadiusSlider;
    Ogre::StringVector _groupNames;

    bool _adjustCenterMode;
    Ogre::SceneNode* _curIcon;
    Ogre::Plane _ground;
    Ogre::Camera* _cam;
    int _curGrpIdx;

  private:
    void
    _getKey(Ogre::String &key);
    void
    _addGroup();
    void
    _adjustCenter(bool engage);
    void
    _createGroups();
    void
    _resetAll();

  };
/*
 class EntitiesGoalMenu : public EntitiesMenu
 {
 public:
 EntitiesGoalMenu(OgreBites::SdkTrayManager* tray);
 virtual
 ~EntitiesGoalMenu()
 {
 cout << "In EntitiesGoalMenu destructor." << endl;
 }
 virtual bool
 handleButton(OgreBites::Button* button);
 private:
 OgreBites::Label* _goalLabel;
 OgreBites::Button* _goalButton;
 };
 */
class EntitiesView : public OgreBites::SdkTrayListener
  {
  public:
    EntitiesView() :
    _tray(0)
      {
      }
    virtual
    ~EntitiesView()
      {
      }

    //Handle mouse events
    bool
    onMouseMove(const OIS::MouseEvent &evt);
    bool
    onMouseUp(const OIS::MouseEvent &evt, const OIS::MouseButtonID id);
    bool
    onMouseDown(const OIS::MouseEvent &evt, const OIS::MouseButtonID id);

    bool
    init(ZWorkspace* workspace);
    void
    itemSelected(OgreBites::SelectMenu* menu);
    void
    buttonHit(OgreBites::Button* button);
    void
    sliderMoved(OgreBites::Slider* slider);
  protected:
  private:

  private:

  private:
    OgreBites::SdkTrayManager* _tray;
    Entities::EntitiesCreateMenu* _createMenu;
  };

}
}
