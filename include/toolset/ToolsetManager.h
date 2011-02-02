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
#include "toolset/ToolInfo.h"
namespace ZGame
{
    namespace Toolset
    {
        class ToolsetManager : public Rocket::Controls::DataSource
        {
        public:
            ToolsetManager(Geometry::GeometryManager* geoMgr);
            virtual ~ToolsetManager();

            const Ogre::uint16 //we need to define this as ID.
                createCursor();
            /** \note This function is not safe. ID may be invalid or pointer may become invalid.**/
            ToolInfo*
                getTool(Ogre::uint16 id);
            void
                refreshTool(Ogre::uint16 id);
           //implement datasource methods
            virtual void
                GetRow(Rocket::Core::StringList& row, const Rocket::Core::String& table, int row_index,
                const Rocket::Core::StringList& columns);
            virtual int
                GetNumRows(const Rocket::Core::String& table);

        private:
            Ogre::vector<ToolInfo>::type _tools; //We are using vector because we don't expect
            //too much tools to be created.
            Geometry::GeometryManager* _geoMgr;



        };
    }
}