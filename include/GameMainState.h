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
    using std::tr1::shared_ptr;
    class GameMainState : public ZGame::GameState
    {
    public:
        GameMainState();
        virtual ~GameMainState();

        virtual void
            getGameStateBootstrapInfo(GameStateBootstrapInfo &info);

        //life cycle methods
        bool onUpdate(const Ogre::FrameEvent &evt);
        bool onInit(ZGame::ZInitPacket packet);
        bool onDestroy();

        //Mouse events
        bool onMouseUp(const OIS::MouseEvent &evt, const OIS::MouseButtonID id);
        bool onMouseDown(const OIS::MouseEvent &evt, const OIS::MouseButtonID id);
        bool onMouseMove(const OIS::MouseEvent &evt);
        //key events
        bool onKeyDown(const OIS::KeyEvent &evt);
        bool onKeyUp(const OIS::KeyEvent &evt);
    protected:
        Ogre::Camera* _cam;
        Ogre::Real _dz;
        bool _forward;
        bool _backward;
        Ogre::Real _trans;
       
                
        virtual void
            regLfcObsForInjection(LifeCycleRegister &lfcReg);
        virtual void
            regKeyObsForInjection(KeyEventRegister &keyReg);
        virtual void
            regMouseObsForInjection(MouseEventRegister &mouseReg);



    private:
    private:
        std::auto_ptr<ControlModuleProto> _controlMod;
        std::auto_ptr<ZGame::World::WorldController> _worldController;
        std::auto_ptr<ZGame::Util::CharacterUtil> _charUtil;
        std::auto_ptr<Entities::EntitiesManager> _entMgr;
        std::auto_ptr<Entities::RenderEntitiesManager> _rdrEntMgr;
        std::auto_ptr<ZCL::ZCLController> _zclCtrl;
        std::auto_ptr<Entities::EntitiesView> _entsView;
        std::auto_ptr<ZWorkspace> _workspace;
        std::auto_ptr<ZGame::ZWorkspaceController> _workspaceCtrl;
    };
}

