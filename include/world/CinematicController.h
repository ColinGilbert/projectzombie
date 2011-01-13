#pragma once
/**
*Copyright (c) 2010 Fdastero LLC

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
**/

#include "ZPrerequisites.h"
#include "world/ZCamInfo.h"
namespace ZGame
{
    namespace World
    {
        //A temp. solution for different operations.
        //In the future we can sub-class this 
        //for different operations (ex: FPS vs. Editor mode).
        class
            CinematicOperation
        {
        public:
            virtual ~CinematicOperation(){}

            virtual void
                onMouseDown(const OIS::MouseEvent &e, OIS::MouseButtonID id) = 0;
            virtual void
                onMouseUp(const OIS::MouseEvent &e, OIS::MouseButtonID id) = 0;
            virtual void
                onMouseMove(const OIS::MouseEvent &e) = 0;
            virtual void
                onKeyDown(const OIS::KeyEvent& e) = 0;
            virtual void
                onKeyUp(const OIS::KeyEvent& e) = 0;
            //NOT CHECKED
            void
                setInput(Ogre::Camera* cam, Control* control)
            {
                _cam = cam;
                _control = control;
            }
        protected:
            CinematicOperation(){}
            Ogre::Camera* _cam;
            Control* _control;
        private:
        };

        class EditorOperation : public CinematicOperation
        {
        public:
            EditorOperation();
            virtual  ~EditorOperation();
            
            virtual void
                onMouseDown(const OIS::MouseEvent &e, OIS::MouseButtonID id);
            virtual void
                onMouseUp(const OIS::MouseEvent &e, OIS::MouseButtonID id);
            virtual void
                onMouseMove(const OIS::MouseEvent &e);
            virtual void
                onKeyDown(const OIS::KeyEvent &e);
            virtual void
                onKeyUp(const OIS::KeyEvent &e);

        };

        class CinematicController
        {
        public:



            typedef Ogre::map<CAMERA_ID, ZCameraInfo>::type CAM_MAP;
            typedef std::pair<CAM_MAP::const_iterator, CAM_MAP::const_iterator> CAM_MAP_CITERS;
            typedef std::pair<Ogre::Vector3, Ogre::Quaternion> CAM_INIT_STATE;
            typedef std::pair<Ogre::String, CAM_INIT_STATE> CAM_PAIR;

            CinematicController(std::auto_ptr<CinematicManager> cineMgr,
                Ogre::RenderWindow* rendWin);
            virtual ~CinematicController();

            void
                loadCameras(std::vector<CAM_PAIR> camStates, size_t rootCamIdx);

            CinematicManager*
                getCinematicManager()
            {
                return _cineMgr.get();
            }

            CAM_MAP_CITERS
                getCameraInfosIterators()
            {
                return std::make_pair<CAM_MAP::const_iterator, CAM_MAP::const_iterator>(_camInfoVec.cbegin(),
                    _camInfoVec.cend());
            }

            void
                onCameraChange(World::CAMERA_ID camId);

            Ogre::Viewport*
                getViewport()
            {
                if(!_vp)
                    OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, "Viewport is null",
                    "CinematicController::getViewport");
                return _vp;
            }

            bool
                onMouseMove(const OIS::MouseEvent& e);
            bool
                onMouseDown(const OIS::MouseEvent& e, OIS::MouseButtonID id);
            bool
                onMouseUp(const OIS::MouseEvent& e, OIS::MouseButtonID id);
            bool
                onKeyDown(const OIS::KeyEvent& e);
            bool
                onKeyUp(const OIS::KeyEvent& e);


        protected:
        private:



            std::auto_ptr<CinematicManager> _cineMgr;

            CAM_MAP _camInfoVec;

            Ogre::RenderWindow* _rendWin;
            Ogre::Viewport* _vp;

            std::auto_ptr<PerspectiveControl> _perspControl;

            EditorOperation _currentOperation;

        };
    }
}