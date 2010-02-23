/*
* GameMainState.h
*
*  Created on: Sep 18, 2008
*      Author: bey0nd
*/

#ifndef GAMEMAINSTATE_H_
#define GAMEMAINSTATE_H_

#include <memory>
//#include <string>
using namespace std;
#include <Ogre.h>
#include <OIS/OIS.h>
#include <boost/shared_ptr.hpp>
using namespace boost;
#include "GameState.h"

namespace ZGame
{
    class GPUEntities;
    class GPUEntsView;
    class LifeCycleRegister;
    class KeyEventRegister;
    class MouseEventRegister;
    class ControlModuleProto;
    class WhiteNoiseView;
    class GPUEntsControl;
    namespace World
    {
        class WorldController;
    }
    namespace Util
    {
        class CharacterUtil;
    }

    class GameMainState : public ZGame::GameState
    {
    public:
        GameMainState();
        virtual ~GameMainState();

        //life cycle methods
        bool onUpdate(const Ogre::FrameEvent &evt);
        bool onInit();
        bool onDestroy();

        //key events
        bool onKeyDown(const OIS::KeyEvent &evt);
        bool onKeyUp(const OIS::KeyEvent &evt);
    protected:
        auto_ptr<GPUEntities> _gpuEnts;
        shared_ptr<GPUEntsView> _gpuEntsView;
        Ogre::Camera* _cam;
        Ogre::Real _dz;
        bool _forward;
        bool _backward;
        Ogre::Real _trans;
        shared_ptr<ControlModuleProto> _controlMod;
        shared_ptr<WhiteNoiseView> _whtNoiseView;
        shared_ptr<GPUEntsControl> _gpuEntsControl;

        void createGPUEntities();
        void createWorld(); //create the world that the game resides in.
        void createCharacters();

        virtual void
            regLfcObsForInjection(LifeCycleRegister &lfcReg);
        virtual void
            regKeyObsForInjection(KeyEventRegister &keyReg);
        virtual void
            regMouseObsForInjection(MouseEventRegister &mouseReg);



    private:
        auto_ptr<ZGame::World::WorldController> _worldController;
        //Utilities for call back execution of commands
        auto_ptr<ZGame::Util::CharacterUtil> _charUtil;

    };
}

#endif /* GAMEMAINSTATE_H_ */
