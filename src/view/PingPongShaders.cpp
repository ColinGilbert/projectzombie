/*
* PingPongShaders.cpp
*
*  Created on: Oct 8, 2008
*      Author: bey0nd
*/

#include <cassert>
#include <stdexcept>
#include "PingPongShaders.h"
#include "EngineView.h"

namespace ZGame
{
    using namespace Ogre;
    using namespace std;
    const Ogre::Real PingPongShaders::_NEARPLANE = 1.0;
    const Ogre::Real PingPongShaders::_FARPLANE = 2.0;
    uint32 PingPongShaders::_ID = 0;
    PingPongShaders::PingPongShaders(Ogre::TexturePtr tex, Ogre::MaterialPtr mat,
        Ogre::Real camZDepth) :
    _input(tex), _mat(mat), _camZDepth(Math::Abs(camZDepth)), _curId(_ID++),
        _pingPongName("PINGPONGSHADER"), _camCoords(0.0, 0.0, _camZDepth)
    {
        // TODO Auto-generated constructor stub
        init();
    }

    PingPongShaders::~PingPongShaders()
    {
        // TODO Auto-generated destructor stub
    }

    void
        PingPongShaders::init()
    {
        Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL,
            "In PingPongShader::setup()");
        SceneManager* scnMgr = EngineView::getSingleton().getSceneManager();
        assert(!_mat.isNull() && "_mat in PingPongShaders.init is null");
        //setup full screen quad
        _fsQuad.reset(new Rectangle2D(true));
        _fsQuad->setCorners(-1.0, 1.0, 1.0, -1.0);
        _fsQuad->setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY);
        _fsQuad->setMaterial(_mat->getName());
        _fsQuad->setVisible(false); //hide it.
        _fsQuad->setBoundingBox(Ogre::AxisAlignedBox(-2.0, -2.0, -2.0, 2.0, 2.0,
            2.0));
        SceneNode* node = scnMgr->getRootSceneNode()->createChildSceneNode();
        node->attachObject(_fsQuad.get());
        //Now position the FS quad such that it is 0.5 unit in from of the camera.
        node->setPosition(0.0f, 0.0f, _camZDepth + 0.5f);

        //setup camera
        ostringstream oss;
        oss << _pingPongName << "CAM" << _curId << endl;
        Camera* cam = scnMgr->createCamera(oss.str());
        assert(_input->getHeight() != 0 && "In PingPongShader::setup, inputHeight is 0");
        cam->setAspectRatio(_input->getWidth() / _input->getHeight());
        cam->setNearClipDistance(_NEARPLANE);
        cam->setFarClipDistance(_FARPLANE);
        cam->setPosition(_camCoords);
        cam->setOrientation(Quaternion::IDENTITY); //face forward

        //initialize the output to be just like the input for swapping.
        duplicateInput();

        //setup RTT
        setupRTT(_input, cam);
        setupRTT(_output, cam);
        Ogre::LogManager::getSingleton().logMessage(LML_TRIVIAL,
            "Out of PingPongShaders::setup.");
    }

    void
        PingPongShaders::setupRTT(TexturePtr &tex, Camera* cam)
    {
        RenderTarget * rt = tex->getBuffer()->getRenderTarget();
        Viewport* vp = rt->addViewport(cam);
        vp->setBackgroundColour((ColourValue::Red));
        vp->setClearEveryFrame(true);
        vp->setDimensions(0.0, 0.0, 1.0, 1.0);
        rt->setAutoUpdated(false);
    }

    void
        PingPongShaders::pingPong()
    {
        //Set the input texture for the material to input texture. If this input does not exist
        //in said material, then an Ogre::Exception will be thrown, and our engine will catch it eventually upstream.
        _mat->getTechnique(0)->getPass(0)->getTextureUnitState("_input")->setTextureName(
            _input->getName());
        RenderTarget* rtt = _output->getBuffer()->getRenderTarget();
        _fsQuad->setVisible(true);
        rtt->update();
        _fsQuad->setVisible(false);
        //swap
        TexturePtr temp = _input;
        _input = _output;
        _output = temp;
    }

    void
        PingPongShaders::duplicateInput()
    {
        ostringstream oss;
        oss << _pingPongName << "TEMPTEX" << _curId << endl;
        _output = TextureManager::getSingleton().createManual(oss.str(),
            ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME,
            _input->getTextureType(), _input->getWidth(), _input->getHeight(),
            _input->getDepth(), _input->getNumMipmaps(), _input->getFormat(),
            _input->getUsage()); //we need to think about this some more.
    }

}
