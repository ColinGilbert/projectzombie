#pragma once

#include <memory>
#include <vector>
#include <iostream>
#include <sstream>
#include <vector>
#include <utility>
#include <exception>
#include <string>
#include <map>
#include <OIS/OIS.h>
#include <Ogre.h>
#include <SdkTrays.h>
#include <OgreTerrain.h>
#include <OgreTerrainGroup.h>




class HDRCompositor;

namespace OgreBites
{
    class SdkTrayManager;

}

namespace ZGame
{
    /**
    * This class contains information needed to bootstrap the Engine. The system
    *requires certain bootstrapping information in order to boot various sub systems.
    *We do this so we can couple this with a state, say to load this information along
    *with the State description.
    **/
    class GameStateBootstrapInfo
    {
    public:
        GameStateBootstrapInfo() :
            requireRenderEntitiesmanager(false),
                requireZCLController(false),
                requireWorldController(false),
                requireControlModule(false),
                requireWorkspace(false),
                requireCharacterUtil(true)
            {}
        Ogre::Vector3 initalCameraPos;
        bool requireRenderEntitiesmanager;
        bool requireZCLController;
        bool requireWorldController;
        bool requireControlModule;
        bool requireWorkspace;
        bool requireCharacterUtil;
    };
   

    struct ZInitPacket;
    class LifeCycleRegister;
    class KeyEventRegister;
    class MouseEventRegister;
    class EngineController;

    class InputController;
    class GameState;
    class GameStateInfo;
    class LifeCyclePump;
    class KeyboardPump;
    class MousePump;
    class CommandController; //This is a service that should exist for all permuation of the Controller.

    class GPUEntities;
    class GPUEntsView;
    class ControlModuleProto;
    class WhiteNoiseView;
    class GPUEntsControl;
    class ZWorkspace;
    class ZWorkspaceController;

    


    class GraphicsController;

    class GameStateBootstrapInfo;

    namespace Networking
    {
        class NetClientController;
    }

    namespace Gui
    {

        class GuiController;
        class MainMenuScreen;
        class HDRSettingsView;
    }

    namespace ZCL
    {
        class ZCLController; 
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
        struct ZEntityBuffers;
    }
    namespace World
    {
        class VolumeMap;
        class VolumeMapPaging;
        class WorldMap;
        struct WorldMapConfig;
        class WorldConfig;
        class PhysicsManager;
        class WorldController;
    }

     namespace EVENT
    {
        struct KeyboardEvtObserver;
        struct MouseEvtObserver;
    }
    namespace LifeCycle
    {
        struct LifeCycleObserver;
    }


}