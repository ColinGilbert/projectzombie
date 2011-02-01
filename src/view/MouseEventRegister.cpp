/*
* MouseEventRegister.cpp
*
*  Created on: Sep 29, 2008
*      Author: bey0nd
*/

#include <Ogre.h>
using namespace Ogre;
#include "MouseEventRegister.h"
#include "DelegatesUtil.h"

namespace ZGame
{

    MouseEventRegister::MouseEventRegister()
    {
        // TODO Auto-generated constructor stub

    }

    MouseEventRegister::~MouseEventRegister()
    {
        // TODO Auto-generated destructor stub
    }

    void
        MouseEventRegister::injectMouseSubj(const EVENT::MouseEvtSubject &subj)
    {
        LogManager::getSingleton().logMessage(LML_TRIVIAL,
            "MouseEventRegister::injectMouseSubj");
        for (ObsItr it = _obs.begin(); it != _obs.end(); ++it)
        {
            subj(*it);
        }
        _obs.clear();
        LogManager::getSingleton().logMessage(LML_TRIVIAL,
            "MouseEventRegister::injectMouseSubj done");
    }

    void
        MouseEventRegister::registerMouseObs(EVENT::MouseEvtObserver &obs, 
        REGISTER_ORDER order)
    {
        if(order == ORDER_DONOTCARE)
        {
            _obs.push_back(obs);
        }
        else
            _obs.push_front(obs);
        EVENT::clearMouseObs(obs);
    }

}
