/*
* ControlModuleProto.h
* This is a prototype for the control module (for controlling objects). This is a place holder only. We want
* to concentrate our efforts on implement other more important features for now and worry about
* proper design of control module later.
*  Created on: Sep 29, 2008
*      Author: bey0nd
*/

#pragma once

#include "ZPrerequisites.h"

namespace ZGame
{

   
    class ControlModuleProto 
    {
    public:
        ControlModuleProto();
        ControlModuleProto(float startTrans);
        virtual
            ~ControlModuleProto();

        bool
            onInit(ZGame::ZInitPacket *packet);

        //events methods
        bool
            onKeyDown(const OIS::KeyEvent &evt);
        bool
            onKeyUp(const OIS::KeyEvent &evt);
        bool
            onMouseMove(const OIS::MouseEvent &evt);
        bool
            onMouseUp(const OIS::MouseEvent &evt, const OIS::MouseButtonID id);
        bool
            onMouseDown(const OIS::MouseEvent &evt, const OIS::MouseButtonID id);
        bool
            onUpdate(const Ogre::FrameEvent &evt);
        bool
            onDestroy(){return true;}


        bool attachNode(const Ogre::StringVector &params);

    private:

        Ogre::Vector3 _transVector;
        Ogre::Real _dTrans; //change in translation
        Ogre::Real _transFactor; //translation factor;
        Ogre::Real _rotYaw;
        Ogre::Real _rotPitch;
        Ogre::Real _rotFactor;
        Ogre::Camera* _cam;
        int _mx_rel;
        int _my_rel;
        int _mx;
        int _my;

        void
            updateTransFactor(Ogre::Real factor);

        void printUsage();

        Ogre::SceneNode* _lookAtNode;
        Ogre::SceneNode* _cameraNode;

        Ogre::Real _camLocalZOffset;
        Ogre::Real _zoomFactor;

        //Clean up the lookat and camera nodes.
        void cleanUpNodes();
        void _doMouseMove(Ogre::Real dt);

    };
}