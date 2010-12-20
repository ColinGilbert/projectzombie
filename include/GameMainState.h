/*
* GameMainState.h
*
*  Created on: Sep 18, 2008
*      Author: bey0nd
*/

#pragma once

#include "ZPrerequisites.h"
#include <Ogre.h>
#include "GameState.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include <memory>
#else
#include <tr1/memory>
#endif

namespace ZGame
{
    class GPUEntities;
    class GPUEntsView;
    class LifeCycleRegister;
    class KeyEventRegister;
    class MouseEventRegister;
    class ControlModuleProto;
    class WhiteNoiseView;
    class GPUEntsControl;
    class ZWorkspace;
    class ZWorkspaceController;
    namespace ZCL
    {
        class ZCLController; 
    }
    namespace World
    {
        class WorldController;
    }
    namespace Util
    {
        class CharacterUtil;
    }
    namespace Entities
    {
        class EntitiesManager;
        class RenderEntitiesManager;
        class EntitiesView;
    }

   

    class GameMainState : public ZGame::GameState
    {
    public:
        GameMainState();
        virtual ~GameMainState();

        virtual void
            getGameStateBootstrapInfo(GameStateBootstrapInfo &info);
        virtual void
            onGuiConfiguration(Gui::GuiController* guiCtrl);
            
    protected:

        virtual void
            regLfcObsForInjection(LifeCycleRegister &lfcReg);
        virtual void
            regKeyObsForInjection(KeyEventRegister &keyReg);
        virtual void
            regMouseObsForInjection(MouseEventRegister &mouseReg);

    private:
        
    };
}

