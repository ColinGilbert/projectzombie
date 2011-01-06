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
    namespace Entities
    {
        class EntitiesManager;
        class EntitiesGroup;
        class RenderEntitiesManager;
    }
    namespace ZCL
    {
        class ZCLController;
    }
    namespace World
    {
        class WorldController;
        class PhysicsManager;
    }
    class ZWorkspace
    {
    public:
        ZWorkspace(Ogre::SceneManager* scnMgr, Entities::EntitiesManager* entMgr, Entities::RenderEntitiesManager* rdrEntMgr, OgreBites::SdkTrayManager* sdkTray,
            ZCL::ZCLController* zclCtrl, World::WorldController* worldCtrl);
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

        Entities::EntitiesManager*
            getEntitiesManager()
        {
            return _entMgr;
        }
        Entities::RenderEntitiesManager*
            getRenderEntitiesManager()
        {
            return _rdrEntMgr;
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

    protected:

    private:
        static size_t _ID;
        Ogre::SceneManager* _scnMgr;
        Ogre::SceneNode* _workspaceRoot;
        Entities::EntitiesManager* _entMgr;
        Entities::RenderEntitiesManager* _rdrEntMgr;
        OgreBites::SdkTrayManager* _tray;
        std::vector<Ogre::SceneNode*> _icons;
        ZCL::ZCLController* _zclCtrl;
        World::WorldController* _worldCtrl;
        World::PhysicsManager* _phyMgr;

    private:
        void
            _getKey(Ogre::String &key);
        void
            _removeIcons();
    };
}
