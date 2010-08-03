/*
* LifeCycleRegister.cpp
*
*  Created on: Sep 27, 2008
*      Author: bey0nd
*/

#include <stdexcept>
#include "LifeCycleRegister.h"
#include <Ogre.h>
#include "DelegatesUtil.h"
using namespace Ogre;
namespace ZGame
{

    LifeCycleRegister::LifeCycleRegister() : _isRegistered(false)
    {
        // TODO Auto-generated constructor stub

    }

    LifeCycleRegister::~LifeCycleRegister()
    {
        // TODO Auto-generated destructor stub
    }

    void LifeCycleRegister::injectLfcSubj(const LifeCycle::LifeCycleSubject &subj)
    {
        LogManager::getSingleton().logMessage(LML_TRIVIAL,"LifeCycleRegister::injectLfcSubj");
        for(LfcObsIt it=_lfcObs.begin();it != _lfcObs.end(); ++it)
        {
            subj(*it);
        }
    }

    void LifeCycleRegister::registerLfcObs(LifeCycle::LifeCycleObserver& obs)
    {
        _lfcObs.push_back(obs);
        LifeCycle::clearLfcObs(obs);
    }

}
