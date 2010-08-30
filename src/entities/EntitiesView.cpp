/*
 * EntitiesView.cpp
 *
 *  Created on: Aug 21, 2010
 *      Author: beyzend
 */
#include "entities/EntitiesView.h"
#include <iostream>
#include <sstream>
#include <utility>
#include "entities/EntitiesManager.h"
#include "entities/RenderEntitiesManager.h"
#include "ZWorkspace.h"
#include "EngineView.h"
using std::cout;
using std::endl;
using ZGame::Entities::EntitiesMenu;
using ZGame::Entities::EntitiesView;
using ZGame::Entities::EntitiesCreateMenu;
using ZGame::Entities::EntitiesGroup;
using ZGame::Entities::EntitiesManager;
using ZGame::Entities::RenderEntitiesManager;
using OgreBites::Button;
using OgreBites::SelectMenu;

int EntitiesCreateMenu::_GRP_KEY = 0;

void
EntitiesView::itemSelected(SelectMenu* menu)
{
  //There is only one menu. We could store a bunch of menus in a vector and call their handle method.
  _createMenu->handleSelectMenu(menu);
}

void
EntitiesView::buttonHit(Button* button)
{
  _createMenu->handleButton(button);
}

void
EntitiesView::sliderMoved(OgreBites::Slider* slider)
{
  _createMenu->handleSlider(slider);
}

bool
EntitiesView::onMouseMove(const OIS::MouseEvent &evt)
{
  _createMenu->onMouseMove(evt);
}

bool
EntitiesView::onMouseDown(const OIS::MouseEvent &evt, const OIS::MouseButtonID id)
{
  cout << "MOUSE DOWN CALLED!" << endl;
  _createMenu->onMouseDown(evt, id);
}

bool
EntitiesView::onMouseUp(const OIS::MouseEvent &evt, const OIS::MouseButtonID id)
{

}

bool
EntitiesView::init(ZWorkspace* workspace)
{
  using std::tr1::shared_ptr;
  using OgreBites::SdkTrayManager;
  _tray = workspace->getSdkTrayManager();
  _tray->setListener(this);
  //Top tray is going to have the entities creation
  _createMenu = new EntitiesCreateMenu(workspace);
  return true;
}

EntitiesMenu::EntitiesMenu(ZWorkspace* workspace) :
  _tray(workspace->getSdkTrayManager()), _workspace(workspace)
{

}

/********************
 * EntitiesCreateMenu
 */

EntitiesCreateMenu::EntitiesCreateMenu(ZWorkspace* workspace) :
  EntitiesMenu(workspace), _adjustCenterMode(false), _curIcon(0), _ground(Ogre::Vector3::UNIT_Y, 0.0f), _groupSelectMenu(0), _centerGroupButton(0),
      _createButton(0), _updateButton(0), _resetButton(0), _perGroupSlider(0), _groupRadiusSlider(0)
{
  using namespace OgreBites;
  const Ogre::Real width = 150.0f;
  const Ogre::Real sepWidth = 500.0f;

  OgreBites::SdkTrayManager* tray = workspace->getSdkTrayManager();

  /**** TOP: **/
  _createLabel = tray->createLabel(OgreBites::TL_TOPLEFT, "ENTCREATELABEL", "Entity Group Creation.", width);
  _addGroupButton = tray->createButton(OgreBites::TL_TOPLEFT, "ENTADDGROUPBUTTON", "Add A Group", width);

  _cam = EngineView::getSingleton().getCurrentCamera();
}

bool
EntitiesCreateMenu::onMouseDown(const OIS::MouseEvent &evt, const OIS::MouseButtonID id)
{
  if (id == OIS::MB_Left)
    {
      if (_adjustCenterMode)
        {
          _adjustCenter(false);
        }
    }
}
bool
EntitiesCreateMenu::onMouseMove(const OIS::MouseEvent &evt)
{
  if (_adjustCenterMode)
    {
      cout << "ABS X, Y: " << evt.state.X.abs << ", " << evt.state.Y.abs << endl;
      Ogre::Ray r = _tray->getCursorRay(_cam);
      std::pair<bool, Ogre::Real> intersect = r.intersects(_ground);
      if (intersect.first)
        {
          Ogre::Vector3 pos = r.getPoint(intersect.second);
          _curIcon->setPosition(pos);

        }
    }
}

void
EntitiesCreateMenu::_getKey(Ogre::String &key)
{
  std::ostringstream oss;
  oss << "GROUP" << _GRP_KEY++;
  key = oss.str();
}
void
EntitiesCreateMenu::_addGroup()
{
  using namespace OgreBites;
  SdkTrayManager* tray = _tray;
  //Get key.
  Ogre::String key;
  _getKey(key);
  Ogre::DisplayString str(key);

  _workspace->createIcon();
  //_icons.push_back(_workspace->createIcon());
  EntitiesGroup* grp = _workspace->getEntitiesManager()->createGroup();
  grp->numOfEnts = 160;
  grp->radius = 10.0f;
  const Ogre::Real width = 150.0f;
  const Ogre::Real sepWidth = 500.0f;

  if (_groupSelectMenu == 0)
    _groupSelectMenu = tray->createLongSelectMenu(TL_TOPLEFT, "ENTGROUPSELECTMENU", "Select A Group", width, 10, _groupNames);
  _groupSelectMenu->addItem(str);
  if (_centerGroupButton == 0)
    _centerGroupButton = tray->createButton(OgreBites::TL_LEFT, "ENTCENTERBUTTON", "Center A Group", 150.0f);
  if (_createButton == 0)
    _createButton = tray->createButton(OgreBites::TL_BOTTOMLEFT, "ENTCREATEBUTTON", "Create Entity Group", width);
  //if (_resetbutton == 0)
  //_resetButton = tray->createButton(OgreBites::TL_BOTTOMLEFT, "ENTRESETBUTTON", "Reset", width);


  if (_perGroupSlider == 0)
    {
      unsigned int nTimes = 20;
      _perGroupSlider = _tray->createThickSlider(OgreBites::TL_LEFT, "ENTPERGRPSLIDER", "Ents Per Group", 320.0f, 150.0f, 80.0f, 80.0f * nTimes, nTimes);
    }
  if (_groupRadiusSlider == 0)
    {
      unsigned int nTimes = 25;
      _groupRadiusSlider = _tray->createThickSlider(OgreBites::TL_LEFT, "ENTGRPRADIUSSLIDER", "Start Radius", 320.0f, 150.0f, 1.0f, 1.0f * nTimes, nTimes);
    }

  //Select the just added item.
  _curGrpIdx = _groupSelectMenu->getNumItems() - 1;
  _groupSelectMenu->selectItem(_curGrpIdx, true);
}

void
EntitiesCreateMenu::_adjustCenter(bool engage)
{
  if (engage)
    {
      _adjustCenterMode = true;
      _tray->hideTrays();
    }
  else
    {
      //Set the center for group.
      Entities::EntitiesGroup* grp = _workspace->getEntitiesManager()->getGroup(_curGrpIdx);
      grp->center = _curIcon->getPosition();
      _adjustCenterMode = false;
      _tray->showTrays();
    }
}

void
EntitiesCreateMenu::_resetAll()
{
  _GRP_KEY = 0;
  _workspace->resetAll();

  //Remove the buttons and clear
  _tray->destroyWidget(_groupSelectMenu);
  _groupSelectMenu = 0;
  _tray->destroyWidget(_centerGroupButton);
  _centerGroupButton = 0;
  _tray->destroyWidget(_resetButton);
  _resetButton = 0;
  _tray->destroyWidget(_perGroupSlider);
  _perGroupSlider = 0;
  _tray->destroyWidget(_groupRadiusSlider);
  _groupRadiusSlider = 0;
  _tray->destroyWidget(_updateButton);
  _updateButton = 0;

}

void
EntitiesCreateMenu::_createGroups()
{
  _workspace->buildGroups();
  if (_resetButton == 0)
    _resetButton = _tray->createButton(OgreBites::TL_BOTTOMLEFT, "ENTRESETBUTTON", "Reset", 150.0f);
  if (_updateButton == 0)
    _updateButton = _tray->createButton(OgreBites::TL_BOTTOMRIGHT, "ENTUPDATEBUTTON", "Update", 150.0f);
  _tray->destroyWidget(_createButton);
  _createButton = 0;
}

void
EntitiesCreateMenu::_updateGroups()
{
  _workspace->updateGroupGoals();
}

bool
EntitiesCreateMenu::handleButton(OgreBites::Button* button)
{
  Ogre::String bName = button->getName();
  if (bName.compare(_addGroupButton->getName()) == 0)
    {
      _addGroup();
    }
  else
    {
      if (_centerGroupButton != 0)
        {
          if (bName.compare(_centerGroupButton->getName()) == 0)
            {
              _adjustCenter(true);
            }
        }

      if (_createButton != 0)
        {
          if (bName.compare(_createButton->getName()) == 0)
            {
              _createGroups();

            }
        }
      if (_resetButton != 0)
        {
          if (bName.compare(_resetButton->getName()) == 0)
            {
              _resetAll();
            }
        }
      if (_updateButton != 0)
        {
          if (bName.compare(_updateButton->getName()) == 0)
            {
              _updateGroups();
            }
        }
    }
  return true;
}

bool
EntitiesCreateMenu::handleSelectMenu(OgreBites::SelectMenu* selectMenu)
{
  if (selectMenu->getName().compare(_groupSelectMenu->getName()) == 0)
    {

      //Do processing.
      cout << "Selected Item: " << _groupSelectMenu->getSelectedItem() << endl;
      cout << "selected item index: " << _groupSelectMenu->getSelectionIndex() << endl;
      _curGrpIdx = _groupSelectMenu->getSelectionIndex();
      _curIcon = _workspace->getIcon(_curGrpIdx);
      Entities::EntitiesGroup* grp = _workspace->getEntitiesManager()->getGroup(_curGrpIdx);
      _perGroupSlider->setValue(grp->numOfEnts);
      _groupRadiusSlider->setValue(grp->radius);

    }
}
bool
EntitiesCreateMenu::handleSlider(OgreBites::Slider* slider)
{
  Ogre::String name = slider->getName();
  Entities::EntitiesGroup* grp = _workspace->getEntitiesManager()->getGroup(_curGrpIdx);
  if (name.compare(_perGroupSlider->getName()) == 0)
    {
      grp->numOfEnts = (int) (_perGroupSlider->getValue());
    }
  else if (name.compare(_groupRadiusSlider->getName()) == 0)
    {
      grp->radius = (int) (_groupRadiusSlider->getValue());
    }

}
/*
 EntitiesGoalMenu::EntitiesGoalMenu(OgreBites::SdkTrayManager* tray) :
 EntitiesMenu(tray)
 {
 using namespace OgreBites;
 const Ogre::Real width = 150.0f;
 _goalLabel = tray->createLabel(OgreBites::TL_BOTTOM, "ENTGOALLABEL",
 "Entities Goal Menu", width);
 }

 bool
 EntitiesGoalMenu::handleButton(OgreBites::Button* button)
 {

 }
 */
