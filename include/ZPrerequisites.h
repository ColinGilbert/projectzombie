/**
*Copyright (c) 2010 Fdastero LLC

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
**/

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
        //class HDRSettingsView;
        class EditorScreen;
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
        typedef Ogre::uint16 CAMERA_ID;
        class VolumeMap;
        class VolumeMapPaging;
        class WorldMap;
        struct WorldMapConfig;
        class WorldConfig;
        class PhysicsManager;
        class WorldController;
        class CinematicController;
        class CinematicManager;

        struct ZCameraInfo
        {
            ZCameraInfo(CAMERA_ID ids, Ogre::String types,
                Ogre::String names) : id(ids), 
                type(types), name(names)
            {
            }
            CAMERA_ID id;
            Ogre::String type;
            Ogre::String name;
        };

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