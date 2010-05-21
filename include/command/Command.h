#ifndef _ZGAME_COMMAND_H
#define _ZGAME_COMMAND_H

/** 
*\file Please do not instantiate the Command class in this file! USE static const classes in CommandList.
*
*/

#include <vector>
#include <map>
#include <utility>

#include <Ogre.h>

#include "fastdelegate/FastDelegate.h"

//#include "command/CommandList.h"
//#include "CommandController.h"

namespace ZGame
{
    /**
    *Command class.
    *
    *\invariants class invariants: Command's key must not be null.
    *
    */
    class CommandController;
    namespace COMMAND
    {
        using fastdelegate::DelegateMemento;
        using fastdelegate::FastDelegate;
        class CommandList;
        typedef Ogre::String COMMAND_KEY;
        class Command
        {
        public:
            Command(){}
            Command(const COMMAND_KEY &key): _key(key)
            {
            }
            virtual ~Command(){}
            void setCommandMemento(fastdelegate::DelegateMemento memento)
            {
                _memento = memento;
            }

            void setKey(const Ogre::String key)
            {
                assert(!key.empty() && "KEY IS NULL!");
                _key = key;
            }

            const Ogre::String 
                getKey() const
            {
                assert(!_key.empty() && "KEY IS NULL!"); //assert class invariant.
                return _key; 
            }

            virtual fastdelegate::DelegateMemento 
                execute(const Command &cmd)
            {
                fastdelegate::DelegateMemento nullMemento;
                return nullMemento;
            }

            const fastdelegate::DelegateMemento* 
                getCommandMemento() const
            {
                assert(!_memento.empty() && "Class invariant invalidated. You cannot get a null Command Memento.");
                return &_memento;
            }



        protected:     


        private:
            friend class ZGame::CommandController;
            Ogre::String _key;
            fastdelegate::DelegateMemento _memento;
            /*
            virtual fastdelegate::DelegateMemento execute(const Ogre::StringVector &params)
            { 
            fastdelegate::DelegateMemento nullMemento;
            return nullMemento;
            }*/

            //virtual fastdelegate
        };

        /**
        *This class defines a wrapper for the old ConsoleCommands. Basically any commands that invovles execution based on a StringVector of parameters and calling
        *the delegate ConsoleCommand.
        *
        */
        class StringCommand : public Command //wrapper to the old console command
        {
        public:

            StringCommand() : Command(){}
            StringCommand(const COMMAND_KEY &key, int numOfParams=1);

            /** \brief Execute this command with an Command as it's input interface. (See class invariants for description of input interface.)*/
            virtual fastdelegate::DelegateMemento
                execute(const Command &cmd);

            /** \brief This method will push Ogre::String parameter to the command.**/
            void 
                push_back(const Ogre::String &param)
            {
                assert(!_params.empty() && "Class invarience failed! Trying to push_back a parameter on an empty parameter vector. There should be at least the key in this vector!");
                //if(_params.empty())
                // setKey(param);
                _params.push_back(param);
            }           
            const Ogre::StringVector& getParams() const
            {
                return _params;
            }
            /**
            *This method will set the parameters by deep copy. 
            *
            *\precondition The passed in params must not be empty. If that is the case, just use the provided parameters in this class.
            *
            */
            void
                setParams(const Ogre::StringVector &params)
            {
                assert(!params.empty() && "Class invariance failed!");
                _params = params;
                setKey(_params[0]); //params[0] is ALWAYS assumed to the key!
            }
        protected:
        private:
            Ogre::StringVector _params;
        };
        using std::map;
        using std::pair;
        using std::make_pair;
        /**
        *This class defines a Command to allow for both pushing String and Memento parameters.
        */
        class StrAndMementoCmd : public StringCommand
        {
        public:
            //CreateRenderEntCmd(){}
            StrAndMementoCmd(const COMMAND::COMMAND_KEY &key, int numOfParams=1);

            virtual fastdelegate::DelegateMemento
                execute(const Command &cmd) = 0;
            
        protected:
            
        private:
            
        };   
    }
}

#endif