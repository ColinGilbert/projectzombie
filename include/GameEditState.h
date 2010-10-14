/*
 * GameEditState.h
 *
 *  Created on: Aug 28, 2008
 *      Author: bey0nd
 */

#pragma once
#include <memory>
using namespace std;
#include <Ogre.h>
#include "GameState.h"
#include <boost/shared_ptr.hpp>
#include "world/VolumeMap.h"
#include "ControlModuleProto.h"
#include "world/VolumeMapPaging.h"
#include "world/VolumePagedWorldSection.h"
using namespace Ogre;
namespace ZGame
{
  class GameEditView;
  class LifeCycleRegister;
  class KeyEventRegister;
  class GraphicsController;
  class GameEditState : public ZGame::GameState
  {
  public:
    GameEditState();
    virtual
    ~GameEditState();

    //life cycle methods
    bool
    onUpdate(const Ogre::FrameEvent& evt);
    bool
    onInit();
    bool
    onDestroy();

    bool
    onMouseUp(const OIS::MouseEvent &evt, const OIS::MouseButtonID id);
    bool
    onMouseDown(const OIS::MouseEvent &evt, const OIS::MouseButtonID id);
    bool
    onMouseMove(const OIS::MouseEvent &evt);

    //control methods
    bool
    onKeyUp(const OIS::KeyEvent &evt);
    bool
    onKeyDown(const OIS::KeyEvent &evt);

  protected:

    //boost::shared_ptr<GameEditView> _editView;
    virtual void
    regLfcObsForInjection(LifeCycleRegister &lfcReg);
    virtual void
    regKeyObsForInjection(KeyEventRegister &keyReg);
    virtual void
    regMouseObsForInjection(MouseEventRegister &mouseReg);

    class DummyPageProvider : public Ogre::PageProvider
    {
    public:
      bool
      prepareProceduralPage(Page* page, PagedWorldSection* section)
      {
        return true;
      }
      bool
      loadProceduralPage(Page* page, PagedWorldSection* section)
      {
        return true;
      }
      bool
      unloadProceduralPage(Page* page, PagedWorldSection* section)
      {
        return true;
      }
      bool
      unprepareProceduralPage(Page* page, PagedWorldSection* section)
      {
        return true;
      }
    };

  private:
    World::VolumeMap _map;
    ZGame::ControlModuleProto _controlMod;
    std::auto_ptr<ZGame::GraphicsController> _gfxCtrl;
    Ogre::PageManager _pageManager;
    DummyPageProvider _dummyPageProvider;
    World::VolumeMapPaging* _volumePaging;


  };
}
