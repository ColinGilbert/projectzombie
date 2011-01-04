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

#ifndef _ZGAME_CMDCONTROLLER_H
#define _ZGAME_CMDCONTROLLER_H

//#include <boost\shared_ptr.hpp>


#include <Ogre.h>

#include <string>

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include <memory>
#else
#include <tr1/memory>
#endif



#include "fastdelegate/FastDelegate.h"
#include "command/Command.h"
#include "Controller.h"
#include "CommandDelegates.h"

namespace ZGame
{
   
    
    class OgreConsole; 
    
    namespace COMMAND
    {       
        class CommandList;
        typedef Ogre::String COMMAND_KEY;
        class ServicesManager;
    }

    using COMMAND::CommandList;

    class OgreConsole;
    using COMMAND::COMMAND_KEY;
    using fastdelegate::DelegateMemento;
    using ZGame::COMMAND::Command;
    using std::tr1::shared_ptr;
    class CommandController : public Ogre::Singleton<CommandController>
    {
    public:

        CommandController();
        ~CommandController();

        bool init();
        void addCommand(shared_ptr<Command> cmd);
        /** \brief This method is used to add a String command. It is used for String Command only. This is done to minimize coupling.*/
        void addCommand(const COMMAND_KEY &key, const DelegateMemento &memento);
        /** \brief This is a string command specific add command method. This is done to decrease coupling.*/
        void attachConsole(std::auto_ptr<OgreConsole> theConsole);
   
        OgreConsole* getConsole(); //Precondition: _console must valid!!!
        
        /** \brief This method is called by anyone to execute a Command object.**/
        fastdelegate::DelegateMemento 
            executeCmd(const Command &cmd);
        static ZGame::CommandController&
            getSingleton();
        static ZGame::CommandController*
            getSingletonPtr();

        void
            onDestroy();

    protected:

    private:
        typedef std::map<Ogre::String, shared_ptr<Command> > CMD_MAP;
        typedef CMD_MAP::iterator MAP_ITER;
        static CMD_MAP __cmdMap;
        std::auto_ptr<OgreConsole> _console;
    };
}


#endif
