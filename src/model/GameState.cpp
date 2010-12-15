/*
* GameState.cpp
*
*  Created on: Aug 28, 2008
*      Author: bey0nd
*/

#include <Ogre.h>
#include <SdkTrays.h>
#include "GameState.h"

#include "GraphicsController.h"
#include "gui/GuiController.h"

namespace ZGame
{

    GameState::GameState() : _gfxCtrl(new GraphicsController()), _guiCtrl(new Gui::GuiController()),
        _tray(0)
    {
        // TODO Auto-generated constructor stub
    }

    GameState::~GameState()
    {
        cout << "In gamestate destrutor." << endl;
        // TODO Auto-generated destructor stub
    }

    void
        GameState::init(LifeCycleRegister &lfcReg, KeyEventRegister &keyReg,
        MouseEventRegister &mouseReg, OgreBites::SdkTrayManager* tray)
    {
        regLfcObsForInjection(lfcReg); //register life cycle observers for injection.
        regKeyObsForInjection(keyReg); //register key observers for injection.
        regMouseObsForInjection(mouseReg);
        _tray = tray;
    }

    void
        GameState::regLfcObsForInjection(LifeCycleRegister &lfcReg)
    {
        try
        {
            LifeCycle::LifeCycleObserver lfcObs;
            try
            {
            LifeCycle::bindAndRegisterLifeCycleObserver<ZGame::GraphicsController>(lfcReg, lfcObs, *_gfxCtrl);
            }catch(Ogre::Exception e)
            {
                OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, e.getDescription() + " in GraphicsController.", "");
            }
            try
            {
            LifeCycle::bindAndRegisterLifeCycleObserver<Gui::GuiController>(lfcReg, lfcObs, *_guiCtrl, LifeCycle::LFC_DEFAULT 
                | LifeCycle::LFC_ON_RENDER_QUEUE_START);
            }catch(Ogre::Exception e)
            {
                OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, e.getDescription() + " in GuiController", "");
            }
        }catch(Ogre::Exception e)
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, e.getDescription() + " in GameState",
                "GameState::regLfcObsForInjection");
        }
    }
    void
        GameState::regKeyObsForInjection(KeyEventRegister &keyReg)
    {
        EVENT::KeyboardEvtObserver keyObs;
        EVENT::bindKeyObserver(keyObs, *_gfxCtrl);
        keyReg.registerKeyObs(keyObs);
    }
    void
        GameState::regMouseObsForInjection(MouseEventRegister &mouseReg)
    {
        EVENT::MouseEvtObserver mouseObs;
        mouseReg.registerMouseObs(mouseObs);
    }


}
