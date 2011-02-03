#pragma once

/**
Permission is hereby granted by Fdastero LLC, free of charge, to any person obtaining a copy
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
*author: beyzend 
*email: llwijk@gmail.com
**/

#include "ZPrerequisites.h"
#include "gui/GuiPrerequisite.h"
#include "gui/ToolInfoView.h"

namespace ZGame
{
    namespace Toolset
    {
        
        class ToolsetController
        {
        public:

            enum ToolType
            {
                SELECT=0,CURSOR, CUBE
            };
            
            typedef Ogre::vector<std::pair<Ogre::String, ToolType> >::type ToolDesc;
            typedef std::pair<ToolDesc::const_iterator, ToolDesc::const_iterator> ToolDescCIter;

            ToolsetController(std::auto_ptr<ToolsetManager> toolMgr);
            virtual ~ToolsetController();

            ToolDescCIter
                getToolDescriptions();

            bool
                onInit(ZGame::ZInitPacket* initPacket);

            /** This method is called for onCreate events. Will
            create the current tool type at the cursor
            **/
            void
                onCreate();

            void
                setToolType(ToolType type);
            int
                getToolType();
            /** Method is called whenever on screen 3d cursor position event is generated.**/
            bool
                onCursorPosition3d(Ogre::Vector3 pos);
            void
                onSetCursor3dPosition();

            Gui::ToolInfoView*
                refreshToolView(Gui::ToolInfoView* toolView, int toolId);

            void
                addListener(ToolsetControllerListener* listener);
            void
                removeListener(ToolsetControllerListener* listener);

            Ogre::Vector3
                getCursor3dPosition();
            bool
                isCursorMode();

        private:

            typedef Ogre::map<ToolsetControllerListener*, ToolsetControllerListener*>::type
                ListenerMap;
            ListenerMap _listenerMap;

            std::auto_ptr<ToolsetManager> _toolMgr;
            ToolDesc _toolsDesc;
            ToolType _curToolType;
            Ogre::uint16 _cursorId;
            Gui::TemplateCloner* _templateCloner;
            void
                _switchTool(ToolType type);
            bool
                _informListeners(ListenerMap::iterator curIter);
                
        };
        class ToolsetControllerListener
        {
        public:
            virtual ~ToolsetControllerListener()
            {
            }
            virtual void
                onChange(ToolsetController* ctrl){};
            virtual void
                onSetCursor3dPosition(ToolsetController* ctrl){};
        protected:
            ToolsetControllerListener(){}
        };
        
    }
}