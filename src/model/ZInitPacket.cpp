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

#include "ZInitPacket.h"
#include "GraphicsController.h"
#include "ZWorkspaceController.h"
#include "gui/GuiController.h"

using namespace ZGame;

ZInitPacket::ZInitPacket(const GameStateBootstrapInfo *bootstrap,
            Ogre::SceneManager* mgr, 
            Ogre::Camera* cam,
            Ogre::RenderWindow* renWindow,
            OIS::Keyboard* key,
            ZWorkspaceController* wrkspaceCtrl,
            GraphicsController* grafxCtrl,
            Gui::GuiController* guiCtrler) 
            : bootstrap(bootstrap), sceneManager(mgr), 
            initialCamera(cam), renderWindow(renWindow),
            keyboard(key),
            workspaceCtrl(wrkspaceCtrl), gfxCtrl(grafxCtrl),
            guiCtrl(guiCtrler)
        {
        }

bool
    ZInitPacket::valid()
{
    if(!sceneManager)
        return false;
    if(!initialCamera)
        return false;
    if(!renderWindow)
        return false;
    if(!keyboard)
        return false;
    if(!workspaceCtrl && bootstrap->requireWorkspace)
        return false;
    if(!gfxCtrl)
        return false;
    if(!guiCtrl)
        return false;

    return true;
}