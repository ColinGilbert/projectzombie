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





namespace ZGame
{
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
        struct ZEntityBuffers;
    }
    namespace World
    {
        class WorldMap;
        class VolumeMap;
        class VolumeMapPaging;
        struct WorldMapConfig;
        class PhysicsManager;
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