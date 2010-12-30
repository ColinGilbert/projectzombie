#pragma once
/**
* \file This file defines a controller for controlling application graphical state.
*/

#include "ZPrerequisites.h"
#include "graphics/HDRCompositor.h"
#include "SkyX/SkyX.h"

namespace ZGame
{

    struct shadowListener : public Ogre::SceneManager::Listener
    {
        // this is a callback we'll be using to set up our shadow camera
        void
            shadowTextureCasterPreViewProj(Ogre::Light *light, Ogre::Camera *cam, size_t)
        {
            // basically, here we do some forceful camera near/far clip attenuation
            // yeah.  simplistic, but it works nicely.  this is the function I was talking
            // about you ignoring above in the Mgr declaration.
            float range = light->getAttenuationRange();
            cam->setNearClipDistance(0.01);
            cam->setFarClipDistance(range);
            // we just use a small near clip so that the light doesn't "miss" anything
            // that can shadow stuff.  and the far clip is equal to the lights' range.
            // (thus, if the light only covers 15 units of objects, it can only
            // shadow 15 units - the rest of it should be attenuated away, and not rendered)
        }

        // these are pure virtual but we don't need them...  so just make them empty
        // otherwise we get "cannot declare of type Mgr due to missing abstract
        // functions" and so on
        void
            shadowTexturesUpdated(size_t)
        {
        }
        void
            shadowTextureReceiverPreViewProj(Ogre::Light*, Ogre::Frustum*)
        {
        }
        void
            preFindVisibleObjects(Ogre::SceneManager*, Ogre::SceneManager::IlluminationRenderStage, Ogre::Viewport*)
        {
        }
        void
            postFindVisibleObjects(Ogre::SceneManager*, Ogre::SceneManager::IlluminationRenderStage, Ogre::Viewport*)
        {
        }
    };

    struct ssaoListener : public Ogre::CompositorInstance::Listener
    {
        Ogre::Camera* cam;

        void setCamera(Ogre::Camera* cam)
        {
            this->cam = cam;
        }

        // this callback we will use to modify SSAO parameters
        void
            notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat)
        {
            if (pass_id != 42) // not SSAO, return
                return;

            // this is the camera you're using

            // calculate the far-top-right corner in view-space
            Ogre::Vector3 farCorner = cam->getViewMatrix(true) * cam->getWorldSpaceCorners()[4];

            // get the pass
            Ogre::Pass *pass = mat->getBestTechnique()->getPass(0);

            // get the vertex shader parameters
            Ogre::GpuProgramParametersSharedPtr params = pass->getVertexProgramParameters();
            // set the camera's far-top-right corner
            if (params->_findNamedConstantDefinition("farCorner"))
                params->setNamedConstant("farCorner", farCorner);

            // get the fragment shader parameters
            params = pass->getFragmentProgramParameters();
            // set the projection matrix we need
            static const Ogre::Matrix4 CLIP_SPACE_TO_IMAGE_SPACE(0.5, 0, 0, 0.5, 0, -0.5, 0, 0.5, 0, 0, 1, 0, 0, 0, 0, 1);
            if (params->_findNamedConstantDefinition("ptMat"))
                params->setNamedConstant("ptMat", CLIP_SPACE_TO_IMAGE_SPACE * cam->getProjectionMatrixWithRSDepth());
            if (params->_findNamedConstantDefinition("far"))
                params->setNamedConstant("far", cam->getFarClipDistance());
        }
    } ;


    class GraphicsController
    {
    public:
        GraphicsController();
        virtual
            ~GraphicsController();

        static const size_t NUM_OF_BANDS = 5;

        bool
            onInit(ZGame::ZInitPacket* packet);
        bool
            onDestroy();
        bool
            onUpdate(const Ogre::FrameEvent &evt);

        bool
            onKeyUp(const OIS::KeyEvent &evt);
        bool
            onKeyDown(const OIS::KeyEvent &evt);

        bool adjustShadow(const Ogre::StringVector &params);
        bool onRenderQueueStart(Ogre::uint8 queueGroupId,
            const Ogre::String& invocation, bool& skipThisInvocation);
        bool onRenderQueueEnd(Ogre::uint8 queueGroupId,
            const Ogre::String& invocation, bool& skipThisInvocation);

        HDRCompositor* 
            getHdrCompositor()
        {
            if(_hdrCompositor.get() == 0)
                OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                "hdr compositor is null", "GraphicsController::getHdrCompositor");
            return _hdrCompositor.get();
        }

        bool
            onFrameEnded(const Ogre::FrameEvent& evt);


    protected:

    private:
        void _initShadows();
        void _initSSAO();
        void _initHDR(Ogre::RenderWindow* windowm, Ogre::Camera* initialCam);
        void _parseHDRConfig();
        void _initSkyX();
        //This is a temp method to disable compositors before rendering of GUI.
        //We need to do this on a material level, by implementing a custom GEOM method
        //for GUI so we don't render them to compositors.
        //This problem is due to libRocket calling _render directly instead of adding
        //it to an RenderQueue.
        void _toggleAllCompositors(bool enable);
    private:

        Ogre::CompositorInstance* _gBufferInstance;
        Ogre::CompositorInstance* _ssaoInstance;
        Ogre::CompositorInstance* _bloomInstance;
        shadowListener _shadowListener;
        ssaoListener _ssaoListener;
        std::auto_ptr<HDRCompositor> _hdrCompositor;
        std::auto_ptr<ListenerFactoryLogic> _logic;

        bool _compositorState[3];
        bool _stateOnce;
        Ogre::SceneManager* _scnMgr;
        Ogre::Viewport* _vp;
        size_t _WHICH_TONEMAPPER;
        size_t _WHICH_STARTYPE;
        size_t _WHICH_GLARETYPE;
        size_t _ADAPT_SCALE;
        bool _IS_AUTO_KEY;
        float _AUTO_KEY;
        float _GLARE_STRENGTH;
        float _STAR_STRENGTH;
        Ogre::ShadowCameraSetupPtr _pssmSetup;
        std::auto_ptr<SkyX::SkyX> _skyX;
        Ogre::Real _SHC_R[NUM_OF_BANDS*NUM_OF_BANDS]; //4 is because we have to use multiple of fours. WARNING, this only works of 3 or 4 bands only!!!
        Ogre::Real _SHC_G[NUM_OF_BANDS*NUM_OF_BANDS];
        Ogre::Real _SHC_B[NUM_OF_BANDS*NUM_OF_BANDS];
        Ogre::Real _timeCount;

        std::vector<Ogre::String> _compositorNames;
        std::vector<Ogre::String> _postEfxNames;
        Ogre::String _currentCompositor;
        Ogre::String _currentPostEfx;
    };
}
