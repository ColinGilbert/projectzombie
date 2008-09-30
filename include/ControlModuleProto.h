/*
 * ControlModuleProto.h
 * This is a prototype for the control module (for controlling objects). This is a place holder only. We want
 * to concentrate our efforts on implement other more important features for now and worry about
 * proper design of control module later.
 *  Created on: Sep 29, 2008
 *      Author: bey0nd
 */

#ifndef CONTROLMODULEPROTO_H_
#define CONTROLMODULEPROTO_H_
#include <Ogre.h>
#include <OIS/OIS.h>
using namespace Ogre;
namespace ZGame
{

  class ControlModuleProto
  {
  public:
    ControlModuleProto();
    virtual
    ~ControlModuleProto();

    bool onKeyDown(const OIS::KeyEvent &evt);
    bool onKeyUp(const OIS::KeyEvent &evt);

  };

}

#endif /* CONTROLMODULEPROTO_H_ */
