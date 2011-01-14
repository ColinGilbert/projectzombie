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
#include "ZCamInfo.h"

namespace ZGame
{
    namespace World
    {
        class CinematicManager
        {
        public:
            
            CinematicManager(Ogre::SceneManager* scnMgr);
            virtual ~CinematicManager();

            void
                createPerspectiveCamera(Ogre::Real windowW, Ogre::Real windowH,
                const Ogre::Vector3 &initialPos,
                const Ogre::Quaternion &initialOrient, World::Control* control);
            void
                createOrthoCamera(Ogre::Real windowW, Ogre::Real windowH,
                const Ogre::Vector3 &initialPos,
                const Ogre::Quaternion &initialOrient, World::Control* control);

            World::ZCameraInfo*
                getCameraInfo(CAMERA_ID id)
            {
                return _camsVec.at(id);
            }

            void
                setRootCam(CAMERA_ID id);
            Ogre::Camera*
                getRootCam()
            {
                if(!_rootCam)
                    OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, 
                    "Trying to get null root camera", "CinematicManager::getRootCam");
                return _rootCam->getCamera();
            }

            World::ZCameraInfo*
                getRootCamInfo()
            {
                if(!_rootCam)
                    OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, 
                    "Trying to get null root camera", "CinematicManager::getRootCam");
                return _rootCam;
            }


          CAM_INFO_CITERS
                getIterators()
            {
                return std::pair<CAM_INFOS::const_iterator, CAM_INFOS::const_iterator>(
                    _camsVec.cbegin(), _camsVec.cend());
            }

        protected:
        private:
            static const CAMERA_ID _camCurId;
            const Ogre::String ORTHO_CAM_NAME;
            const Ogre::String PERSP_CAM_NAME;
            CAM_INFOS _camsVec;

            Ogre::SceneManager* _scnMgr;
            World::ZCameraInfo* _rootCam;

        };
    }
}