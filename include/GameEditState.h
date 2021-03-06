/*
* GameEditState.h
*
*  Created on: Aug 28, 2008
*      Author: bey0nd
*/

#pragma once
#include <memory>
#include <Ogre.h>
#include "GameState.h"
#include <boost/shared_ptr.hpp>
#include "world/VolumeMap.h"
#include "ControlModuleProto.h"
#include "world/VolumeMapPaging.h"
#include "world/VolumePagedWorldSection.h"

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
            onInit(ZGame::ZInitPacket* initPacket);
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

        virtual void
            onGuiConfiguration(Gui::GuiController* guiCtrl);

        virtual void
            onWorldControllerConfiguration(World::WorldController* worldCtrl);

        virtual void
            getGameStateBootstrapInfo(GameStateBootstrapInfo &info);

        virtual void
            onCinematicControllerConfiguration(World::CinematicController* cineCtrl);

    protected:

        //boost::shared_ptr<GameEditView> _editView;
        virtual void
            regLfcObsForInjection(LifeCycleRegister &lfcReg);
        virtual void
            regKeyObsForInjection(KeyEventRegister &keyReg);
        virtual void
            regMouseObsForInjection(MouseEventRegister &mouseReg);



    private:
        ZGame::ControlModuleProto _controlMod;
        Gui::EditorScreen* _editorScreen;


    };
}
