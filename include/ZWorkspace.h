/**
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

/**
* \file This file defines ZWorkspace. A model for representing the concept of a workspace GUI, which interacts with the 3D world.
*/
#pragma once

#include "ZPrerequisites.h"
#include <SdkTrays.h>
#include "ZException.h"

namespace ZGame
{
    class ZWorkspace
    {
    public:
        ZWorkspace(Ogre::SceneManager* scnMgr, Entities::ComponentController*, OgreBites::SdkTrayManager* sdkTray,
            ZCL::ZCLController* zclCtrl, World::WorldController* worldCtrl, World::GameController* gameCtrl,
            World::CinematicController* cineCtrl, Geometry::GeometryManager* geoMgr, Toolset::ToolsetController* toolsetCtrl);
        virtual
            ~ZWorkspace();


        void
            buildGroups();
        void
            updateGroupGoals();

        void
            resetAll();
        Ogre::SceneNode*
            createIcon();
        Ogre::SceneNode*
            getIcon(size_t index)
        {
            size_t size = _icons.size();
            if(index >= size)
                throw Exception("Icon index is out of range.");
            return _icons[index];
        }

      
        OgreBites::SdkTrayManager*
            getSdkTrayManager()
        {
            return _tray;
        }
        World::PhysicsManager*
            getPhysicsManager()
        {
            return _phyMgr;
        }
        World::WorldController*
            getWorldController()
        {
            return _worldCtrl;
        }

        World::CinematicController*
            getCinematicController()
        {
            return _cineCtrl;
        }

        Geometry::GeometryManager*
            getGeometryManager()
        {
            return _geoMgr;
        }

        Toolset::ToolsetController*
            getToolsetController()
        {
            return _toolsetCtrl;
        }

        World::GameController*
            getGameController()
        {
            return _gameCtrl;
        }

        Entities::ComponentController*
            getComponentController()
        {
            return _componentCtrl;
        }

    protected:

    private:
        static size_t _ID;
        Ogre::SceneManager* _scnMgr;
        Ogre::SceneNode* _workspaceRoot;
        OgreBites::SdkTrayManager* _tray;
        std::vector<Ogre::SceneNode*> _icons;
        ZCL::ZCLController* _zclCtrl;
        World::WorldController* _worldCtrl;
        World::PhysicsManager* _phyMgr;
        World::CinematicController* _cineCtrl;
        World::GameController* _gameCtrl;
        Geometry::GeometryManager* _geoMgr;
        Toolset::ToolsetController* _toolsetCtrl;
        Entities::ComponentController* _componentCtrl;
        

    private:
        void
            _getKey(Ogre::String &key);
        void
            _removeIcons();
    };
}
