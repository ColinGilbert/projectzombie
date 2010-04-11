/**
*This interface defines a contract for a controller which will drive the entire application.
*
**/

#ifndef _MAINCONTROLLER_H
#define _MAINCONTROLLER_H

#include <memory>

#include "CommandController.h" //This is for the singleton CommandController. This is consider an essential service that will exist for all instances--all permuation of the ADT--of MainController.

namespace ZGame 
{
  class MainController
  {
  public:
    virtual
    ~MainController()
    {
    }

    virtual bool
    onInit() = 0;
    virtual void
    run() = 0;
    virtual void
        onDestroy(){ _commandController->onDestroy();}
  protected:
      MainController():_commandController(new CommandController())
      {
          cout << "In MainController default constructor" << endl;
      }

      auto_ptr<ZGame::CommandController> _commandController;

  };
}


#endif