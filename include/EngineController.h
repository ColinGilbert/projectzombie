/*
* EngineController.h
*
*  Created on: Aug 24, 2008
*      Author: bey0nd
*/

#pragma once

//#include <string>

#include <vector>
#include <Ogre.h>
#include <OIS/OIS.h>
#include <SdkTrays.h>

#include <boost/thread/thread.hpp>
#include <boost/thread/xtime.hpp>
#include <boost/shared_ptr.hpp>

#include "EventDelegates.h"
#include "LifeCycleDelegates.h"
#include "LifeCycleRegister.h"
#include "GameStateInfo.h"
#include "MainController.h"
#include "RakNetworkFactory.h"
#include "RakPeerInterface.h"
#include "net/NetClientController.h"

namespace ZGame
{
    class InputController;
    class GameState;
    class GameStateInfo;
    class LifeCyclePump;
    class KeyboardPump;
    class MousePump;
    class CommandController; //This is a service that should exist for all permuation of the Controller.

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

    class GameStateBootstrapInfo;

    class GraphicsController;
    namespace Gui
    {
        class GuiController;
    }


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

    class EngineController : public Ogre::FrameListener, public MainController, public OgreBites::SdkTrayListener,
        public Ogre::RenderQueueListener
    {
    public:
        EngineController();
        virtual
            ~EngineController();

        //bool frameStarted(const Ogre::FrameEvent &evt);
        bool
            frameRenderingQueued(const Ogre::FrameEvent &evt);
        void
            transitionState(const Ogre::String key);
        bool
            onInit();
        void
            run();
        void
            onDestroy();

        void
            injectInputSubject();

        //input events
        bool
            onKeyUp(const OIS::KeyEvent &event);
        bool
            onKeyDown(const OIS::KeyEvent &evnt);
        bool
            onMouseMove(const OIS::MouseEvent &event);
        bool
            onMouseUp(const OIS::MouseEvent &event, const OIS::MouseButtonID id);
        bool
            onMouseDown(const OIS::MouseEvent &event, const OIS::MouseButtonID id);

        virtual void
            renderQueueStarted(Ogre::uint8 queueGroupdId, const Ogre::String& invocation, bool& skipThisInovcation);
        virtual void
            renderQueueEnded(Ogre::uint8 queueGroupdId, const Ogre::String& invocation, bool& skipThisInovcation);

        bool
            executeCmd(const Ogre::StringVector& params);
        bool
            loadStartStates();

        //input observers
        //void
        //addMouseObserver(ZGame::EVENT::MouseEvtObserver obs);
        //setters getters
        //Ogre::RenderWindow* getRenderWindow(){return _window.get();}
    private:
        std::auto_ptr<Ogre::Root> _root;
        Ogre::SceneManager* _scnMgr;
        Ogre::RenderWindow* _window;
        Ogre::Overlay* _debugOverlay;

        std::auto_ptr<ZGame::InputController> _inController;
        GameStateInfoMap _gameSInfoMap;

        //Current state
        //std::auto_ptr<GameStateInfo> _curStateInfo;
        GameStateInfo* _curStateInfo;
        std::auto_ptr<GameState> _curGameState;
        //LifeCycle pump
        std::auto_ptr<LifeCyclePump> _lfcPump;
        std::auto_ptr<KeyboardPump> _keyPump;
        std::auto_ptr<MousePump> _mousePump;

        void
            loadAssets(Ogre::String filename);
        void
            chooseSceneManager();

        void
            loadStates();

        //state handling
        void
            loadCurrentState(const Ogre::String curKey);
        void
            unloadCurrentState();
        void
            realizeCurrentState(); //"realize" as in use meta-data to load actual class data.

        Ogre::Camera*
            createDefaultCamera(Ogre::Vector3 initialPos);

    private:

        auto_ptr<ZGame::Networking::NetClientController> _netClient;
        auto_ptr<OgreBites::SdkTrayManager> _sdkTrayMgr;
        Ogre::Viewport* _vp;

        Ogre::String _listenerID;
        bool _stillRunning;

        void
            loadStartStateToCurrentState(const Ogre::String curKey);

        void
            initConsole(); //temp method for initialize the console.
        void
            manuallyRegisterNetClient(LifeCycleRegister &lfcReg); //temp method for register the net client to the system.

        void
            updateStats();

        void
            loadSdkTrays();

        void
            _initSubSystemsOnLoadState(const ZGame::GameStateBootstrapInfo &info, 
            LifeCycleRegister &lfcReg, KeyEventRegister &keyReg, MouseEventRegister &mouseReg);
        void
            _removeSubSystemsOnUnloadState();
        void
            _initCommands();
        

        void
            _switchState(const Ogre::String &state);
        /** \brief actual method for switching state.**/
        void
            _doSwitchingState();
        
        int _statsClockVariable;

        bool _switchingState;
        Ogre::String _switchToStateKey;
        

        ZInitPacket* _initPacket;
        auto_ptr<GraphicsController> _gfxCtrl;
        auto_ptr<Gui::GuiController> _guiCtrl;

        std::auto_ptr<ControlModuleProto> _controlMod;
        std::auto_ptr<ZGame::World::WorldController> _worldController;
        std::auto_ptr<ZGame::Util::CharacterUtil> _charUtil;
        std::auto_ptr<Entities::EntitiesManager> _entMgr;
        std::auto_ptr<Entities::RenderEntitiesManager> _rdrEntMgr;
        std::auto_ptr<ZCL::ZCLController> _zclCtrl;
        std::auto_ptr<Entities::EntitiesView> _entsView;
        std::auto_ptr<ZWorkspace> _workspace;
        std::auto_ptr<ZGame::ZWorkspaceController> _workspaceCtrl;
        std::auto_ptr<CommandController> _commandController;

    };

}
