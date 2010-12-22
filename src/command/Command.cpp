#include "command/Command.h"
#include "CommandDelegates.h" //for ConsoleCommand
#include "CommandController.h"

using namespace ZGame::COMMAND;
//using namespace fastdelegate;
using fastdelegate::DelegateMemento;

void
    Command::executeThis()
{
    CommandController::getSingleton().executeCmd(*this);
}

StringCommand::StringCommand(const COMMAND_KEY &key, int numOfParams) : Command(key) //_params(numOfParams)
{
    _params.push_back(getKey());
}


/**
*
*\note This is legacy support for Ogre Console. We need to refactor ASAP so OgreConsole calls the parameterless version of execute.
*

fastdelegate::DelegateMemento 
StringCommand::execute(const Ogre::StringVector &params)
{
    DelegateMemento NULL_MEMENTO;
    ConsoleCommand cmd;
    cmd.SetMemento(*getCommandMemento());
    cmd(params); //invoke the ConsoleCommand;
    return NULL_MEMENTO;
}*/

/**
*This method will execute this Command, provided with the input of a StringCommand input object. This is a recursive definition. Per the class invariant, when one is executing
*this command, they must initiate the call chain by passing in a StringCommand object (e.g. so they can pass in string parameters). That string object defines the input part of this command. 
*Per the class invariant, there is a one-to-one mapping from the Command implementor to CommandController via a StringCommand object
*which contains the delegate that implements the Command. That is to say, by the class invariant, the parameter Command SHOULD be guarranteed to be a StringCommand object!
*
*\precondition must provide an input Command object which is a StringCommand, as stated in the class invariant. The StringCommand contains
*valid input parameters. Class invariance must also be valid for the Command class! (See Command class.)
*
*\note Notice that this is a recursive definition. Common usage: An implementor add the StringCommand by using AddCommand, which gets mapped into the __cmdMap. Then, anyone wanting to invoke
*this command can do so by getting a const static StringCommand (for a specific StringCommand, e.g: say NodeList) and providing parameters by adding the parameters to the StringCommand. Then
*they call CommandController::executeCmd with said StringCommand and the particular StringCommand executes.
*/
fastdelegate::DelegateMemento
StringCommand::execute(const Command &cmd)
{
    using Ogre::StringVector;
    using COMMAND::ConsoleCommand;
    DelegateMemento NULL_MEMENTO;
    const StringCommand* cmdPtr = static_cast<const StringCommand*>(&cmd);
    //Get the parameters map.
    //const StringVector* params = 0;
   
    const StringVector *params = &cmdPtr->getParams();
    //cmdPtr->getParams(params);
    
    //execute the ConsoleCommand delegate with the parameters.
    ConsoleCommand consoleCmd;
    consoleCmd.SetMemento(*getCommandMemento());
    consoleCmd(*params); //invoke this StringCommand through console delegate.

    return NULL_MEMENTO;
}

StrAndMementoCmd::StrAndMementoCmd(const COMMAND_KEY &key, int numOfParams) : StringCommand(key, numOfParams)
{
}


