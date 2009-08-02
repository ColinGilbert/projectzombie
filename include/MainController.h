/**
*This interface defines a contract for a controller which will drive the entire application.
*
**/

#ifndef _MAINCONTROLLER_H
#define _MAINCONTROLLER_H

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
    onDestroy() = 0;
  };
}


#endif