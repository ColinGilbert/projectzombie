/*
* GameMainMenuState.h
*
*  Created on: Sep 5, 2008
*      Author: bey0nd
*/

#ifndef GAMEMAINMENUSTATE_H_
#define GAMEMAINMENUSTATE_H_
#include "ZPrerequisites.h"

#include "GameState.h"

namespace ZGame
{
    class GameMainMenuState : public GameState
    {
    public:
        GameMainMenuState();
        virtual ~GameMainMenuState();

        //life cycle methods
        bool onUpdate(const Ogre::FrameEvent& evt);
        bool onInit(ZGame::ZInitPacket *packet);
        bool onDestroy();

        //control methods
        bool onKeyUp(const OIS::KeyEvent &evt);
        bool onKeyDown(const OIS::KeyEvent &evt);

        virtual void
            getGameStateBootstrapInfo(GameStateBootstrapInfo &info);
        virtual void
            onGuiConfiguration(Gui::GuiController* guiCtrl);
        virtual void
            onCinematicControllerConfiguration(World::CinematicController* cineCtrl);

    protected:
        virtual void regLfcObsForInjection(LifeCycleRegister &lfcReg);
        virtual void regKeyObsForInjection(KeyEventRegister &keyReg);
    private:
        //auto_ptr<Gui::MainMenuScreen> _mainMenu;
        Gui::MainMenuScreen* _mainMenu;
    };
}

#endif /* GAMEMAINMENUSTATE_H_ */
