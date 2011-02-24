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
        
        class ToolsetController : public Rocket::Controls::DataSource
        {
        public:

            static const unsigned int ONCHANGE_EVENT = BIT(1);
            static const unsigned int ONCURSOR3DPOSITION_EVENT = BIT(2);

            enum ToolType
            {
                SELECT=0,CURSOR, CUBE
            };
            //Note: Need to update the names for these modes.
            enum ToolsetMode
            {
                MOVECURSOR=0,TRANSLATE,SCALE,ROTATE,FILL
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

            void
                setToolsetMode(ToolsetMode mode);
           
            int
                getToolsetMode();

            /** Method is called whenever on screen 3d cursor position event is generated.**/
            bool
                onCursorPosition3d(Ogre::Vector3 pos);
            void
                onSetCursor3dPosition(World::WorldController* worldCtrl);

            void
                refreshToolView(Gui::ToolInfoView* toolView, int toolId, 
                Rocket::Core::Element* rootElement);

            void
                addListener(ToolsetControllerListener* listener);
            void
                removeListener(ToolsetControllerListener* listener);

            Ogre::Vector3
                getCursor3dPosition();
            bool
                isCursorMode();

            //implement datasource methods
            virtual void
                GetRow(Rocket::Core::StringList& row, const Rocket::Core::String& table, int row_index,
                const Rocket::Core::StringList& columns);
            virtual int
                GetNumRows(const Rocket::Core::String& table);

            /**
            * This method will get a constraint plane based on the current cursor and anchor position.
            **/
            bool
                getConstraintPlane(const Ogre::Ray &ray, Ogre::Plane &plane);
            void
                onMaterialChange(size_t matId);

            Ogre::AxisAlignedBox
                getSelectionBox()
            {
                return _currentSelectBox;
            }

        private:

           

            typedef Ogre::map<ToolsetControllerListener*, ToolsetControllerListener*>::type
                ListenerMap;
            ListenerMap _listenerMap;

            std::auto_ptr<ToolsetManager> _toolMgr;
            ToolDesc _toolsDesc;
            ToolType _curToolType;
            Ogre::uint16 _cursorId;
            Ogre::uint16 _cursorBlueId; //This is used as anchor
            Ogre::uint16 _cursorYellowId; //This is used as selection box.
            Gui::TemplateCloner* _templateCloner;
            ToolsetMode _curToolsetMode;
            ToolXForm* _cubeSelectXForm;
            Ogre::AxisAlignedBox _currentSelectBox;
            void
                _switchTool(ToolType type);
            void
                _informListeners(unsigned int event);
            void
                _computeSelectAABB(Ogre::AxisAlignedBox &aabb, const Ogre::Vector3 &cursor,
                const Ogre::Vector3 &anchor, unsigned int &whichCorner);
            size_t _materialId;
           
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