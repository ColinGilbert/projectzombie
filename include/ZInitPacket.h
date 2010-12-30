#pragma once
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
#include "ZPrerequisites.h"

namespace ZGame
{
    struct ZInitPacket
    {
        ZInitPacket(const GameStateBootstrapInfo *bootstrap, Ogre::SceneManager* mgr = 0, 
            Ogre::Camera* cam = 0,
            Ogre::RenderWindow* renWindow = 0,
            OIS::Keyboard* key = 0,
            ZWorkspaceController* wrkspaceCtrl = 0,
            GraphicsController* grafxCtrl = 0,
            Gui::GuiController* guiCtrler = 0); 
            
        bool valid();
        Ogre::SceneManager* sceneManager;
        Ogre::Camera* initialCamera;
        Ogre::RenderWindow* renderWindow;
        OIS::Keyboard* keyboard;
        ZWorkspaceController* workspaceCtrl;
        GraphicsController* gfxCtrl;
        Gui::GuiController* guiCtrl;
        const GameStateBootstrapInfo* bootstrap;
    };
}