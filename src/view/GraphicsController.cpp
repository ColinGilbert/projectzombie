/*
* GraphicsController.cpp
*
*  Created on: Aug 20, 2010
*      Author: beyzend
*/

#include "GraphicsController.h"
#include "ZInitPacket.h"
#include "CommandController.h"
#include "command/CommandList.h"
#include "CommandDelegates.h"
#include "graphics/PreethamSH.h"
#include "graphics/SunSH.h"

using std::cout;
using std::endl;

using ZGame::GraphicsController;
using namespace Ogre;

GraphicsController::GraphicsController() :
_hdrCompositor(0), _WHICH_TONEMAPPER(3), _WHICH_STARTYPE(1), _WHICH_GLARETYPE(1), _AUTO_KEY(0.16),
    _ADAPT_SCALE(3), _GLARE_STRENGTH(0.1), _STAR_STRENGTH(0.1), _skyX(0),_timeCount(0.0f),_stateOnce(false),
    _vp(0)
{
    _compositorNames.push_back("SSAO/HemisphereMC");
    _compositorNames.push_back("SSAO/Volumetric");
    _compositorNames.push_back("SSAO/HorizonBased");
    _compositorNames.push_back("SSAO/Crytek");
    _compositorNames.push_back("SSAO/CreaseShading");
    _compositorNames.push_back("SSAO/UnsharpMask");
    _compositorNames.push_back("SSAO/ShowDepth");
    _compositorNames.push_back("SSAO/ShowNormals");
    _compositorNames.push_back("SSAO/ShowViewPos");

    _postEfxNames.push_back("SSAO/Post/NoFilter");
    _postEfxNames.push_back("SSAO/Post/CrossBilateralFilter");
    _postEfxNames.push_back("SSAO/Post/SmartBoxFilter");
    _postEfxNames.push_back("SSAO/Post/BoxFilter");

    _currentCompositor = _compositorNames[0];
    _currentPostEfx = _postEfxNames[0];
}

GraphicsController::~GraphicsController()
{
    cout << "In graphicsController destructor." << endl;
    //delete _hdrCompositor;
}

bool
    GraphicsController::onRenderQueueStart(Ogre::uint8 queueGroupId,
    const Ogre::String& invocation, bool& skipThisInvocation)
{
    if(queueGroupId == Ogre::RENDER_QUEUE_OVERLAY && _vp->getOverlaysEnabled())
    {
        if(!_stateOnce)
        {
            _toggleAllCompositors(false);
            _stateOnce = !_stateOnce;
        }
    }
    return true;
}

bool
    GraphicsController::onRenderQueueEnd(Ogre::uint8 queueGroupId,
    const Ogre::String& invocation, bool& skipThisInvocation)
{

    if(queueGroupId == Ogre::RENDER_QUEUE_OVERLAY && _vp->getOverlaysEnabled())
    {
        if(_stateOnce)
        {
            _toggleAllCompositors(true);
            _stateOnce = !_stateOnce;
        }
    }
    return true;
}

bool
    GraphicsController::onInit(ZGame::ZInitPacket* packet)
{

    Ogre::CompositorManager& compMgr = Ogre::CompositorManager::getSingleton();

    compMgr.registerCompositorLogic("HDR", new HDRLogic);

    for(size_t i = 0; i < NUM_OF_BANDS * 4; i++)
    {
        _SHC_R[i] = 0.0f;
        _SHC_G[i] = 0.0f;
        _SHC_B[i] = 0.0f;
    }
    //Add the commands for adjusting shadow.
    using ZGame::CommandController;
    CommandController &cmdCtrl = CommandController::getSingleton();
    ZGame::COMMAND::ConsoleCommand cmd;
    cmd.bind(this, &GraphicsController::adjustShadow);
    cmdCtrl.addCommand(CommandList::GRAPHICS_ADJUSTSHADOW, cmd.GetMemento());

    Ogre::LogManager* lm = Ogre::LogManager::getSingletonPtr();
    lm->logMessage(Ogre::LML_TRIVIAL, "In GraphicsController::onInit()");
    _scnMgr = packet->sceneManager;
    _vp = packet->initialCamera->getViewport();
    lm->logMessage(Ogre::LML_NORMAL, "Adding compositor bloom");

    //First build the lights.
    Vector3 lightDir(1.0f, -0.6f, 0.0f);
    lightDir.normalise();
    _scnMgr->setAmbientLight(Ogre::ColourValue(0.0f, 0.0f, 0.0f));
    Light* light = _scnMgr->createLight("terrainLight");
    light->setType(Light::LT_DIRECTIONAL);
    light->setDirection(lightDir);
    light->setDiffuseColour(ColourValue(1.0, 1.0, 1.0));
    light->setSpecularColour(ColourValue(0.1f, 0.1f, 0.1f));

    _ssaoListener.setCamera(packet->initialCamera);
    Ogre::ColourValue fadeColour(0.109f, 0.417f, 0.625f);
    fadeColour *= 0.0001f;
    _scnMgr->setFog(Ogre::FOG_NONE);
    //_initBackgroundHdr();
    _vp->setBackgroundColour(fadeColour);

    this->_parseHDRConfig();


    _ogreHdr = CompositorManager::getSingleton().addCompositor(_vp, "HDR", 0);
    Ogre::CompositorManager::getSingleton().setCompositorEnabled(_vp, "HDR", true);

    _initSkyX();
    _initSSAO();
    //_initHDR(packet->renderWindow, packet->initialCamera);

    _bloomInstance = CompositorManager::getSingleton().addCompositor(_vp, "Bloom");

    _scnMgr->setShadowTechnique(Ogre::SHADOWTYPE_NONE);
    return true;
}

void
    GraphicsController::_initBackgroundHdr()
{
    Ogre::Rectangle2D* rect = new Ogre::Rectangle2D(false);
    rect->setCorners(-1.0f, 1.0f, 1.0f, -1.0f);
    rect->setMaterial("PRJZ/HDRBackground");
    rect->setRenderQueueGroup(Ogre::RENDER_QUEUE_BACKGROUND + 1);
    rect->setBoundingBox(Ogre::AxisAlignedBox(-100000.0*Ogre::Vector3::UNIT_SCALE, 100000.0*Ogre::Vector3::UNIT_SCALE));
    //Ogre::AxisAlignedBox aabInf;
    //aabInf.setInfinite();
    //rect->setBoundingBox(aabInf);
    Ogre::SceneNode* node = _scnMgr->getRootSceneNode()->createChildSceneNode("HdrBackground");
    node->attachObject(rect);
    node->setVisible(true);
}

void
    GraphicsController::_initSkyX()
{
    _skyX.reset(new SkyX::SkyX(_scnMgr, _vp->getCamera()));
    _skyX->create();
    //_skyX->getGPUManager()->addGroundPass(
    //  static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().
    //  getByName("PRJZ/Minecraft"))->getTechnique(0)->createPass(), 128, Ogre::SBT_TRANSPARENT_COLOUR);

    //Upadte SkyX
    SkyX::AtmosphereManager::Options SkyXOptions = _skyX->getAtmosphereManager()->getOptions();
    SkyXOptions.EastPosition = Ogre::Vector2(1, 0);
    _skyX->getAtmosphereManager()->setOptions(SkyXOptions);
    _skyX->setLightingMode(SkyX::SkyX::LM_LDR);




}

/**
** \note PLEASE REFACTOR THIS CODE to load from a defined path instead of current path.
*/
void
    GraphicsController::_parseHDRConfig()
{
    Ogre::ConfigFile cf;
    cf.load("hdr.cfg");
    ConfigFile::SectionIterator seci = cf.getSectionIterator();
    Ogre::String secName, typeName, archName;
    std::istringstream instream;
    while(seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        ConfigFile::SettingsMultiMap* settings = seci.getNext();
        ConfigFile::SettingsMultiMap::iterator i;
        for(i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            instream.clear();
            instream.str(archName);

            if(typeName.compare("ISAUTOKEY") == 0)
            {
                instream >> _IS_AUTO_KEY;
            }
            else if(typeName.compare("TONEMAPPER") == 0)
            {
                instream >> _WHICH_TONEMAPPER;
            }
            else if(typeName.compare("STARTYPE") == 0)
            {
                instream >> _WHICH_STARTYPE;
            }
            else if(typeName.compare("GLARETYPE") == 0)
            {
                instream >> _WHICH_GLARETYPE;
            }
            else if(typeName.compare("AUTOKEY") == 0)
            {
                instream >> _AUTO_KEY;
            }
            else if(typeName.compare("ADAPTSCALE") == 0)
            {
                instream >> _ADAPT_SCALE;
            }
            else if(typeName.compare("GLARESTRENGTH") == 0)
            {
                instream >> _GLARE_STRENGTH;
            }
            else if(typeName.compare("STARSTRENGTH") == 0)
            {
                instream >> _STAR_STRENGTH;
            }
        }
    }
}

void
    GraphicsController::_initHDR(Ogre::RenderWindow* renderWindow, Ogre::Camera* initialCam)
{
    _hdrCompositor.reset(new HDRCompositor(renderWindow, initialCam));
    //_logic.reset(new ListenerFactoryLogic);
    //_logic->setCompositorListener(_hdrCompositor.get());

    //Ogre::CompositorManager::getSingleton().registerCompositorLogic("HDR", logic);
    cout << "Finished registering logic for HDR." << endl;
    _hdrCompositor->SetToneMapper((HDRCompositor::TONEMAPPER)(_WHICH_TONEMAPPER));
    cout << "Tone mapper set." << endl;
    _hdrCompositor->SetStarPasses(2);
    cout << "Star passes set." << endl;
    _hdrCompositor->SetGlarePasses(2);
    cout << "Glare passes set." << endl;
    _hdrCompositor->SetGlareType((HDRCompositor::GLARETYPE)_WHICH_GLARETYPE);
    _hdrCompositor->SetStarType((HDRCompositor::STARTYPE)_WHICH_STARTYPE);
    _hdrCompositor->SetAutoKeying(true);
    _hdrCompositor->SetKey(_AUTO_KEY);
    _hdrCompositor->SetLumAdapdation(true);
    //_hdrCompositor->set
    _hdrCompositor->SetAdaptationScale(_ADAPT_SCALE);
    _hdrCompositor->SetStarPasses(4); //was 4
    _hdrCompositor->SetGlarePasses(2); //was 2
    _hdrCompositor->SetGlareStrength(_GLARE_STRENGTH);
    _hdrCompositor->SetStarStrength(_STAR_STRENGTH);
    _hdrCompositor->update(); //dirtied so we need to update.
    cout << "Done enabling HDR." << endl;
}

bool
    GraphicsController::onKeyUp(const OIS::KeyEvent &evt)
{
    return true;
}

bool
    GraphicsController::onKeyDown(const OIS::KeyEvent &evt)
{
    //Upadte SkyX
    SkyX::AtmosphereManager::Options SkyXOptions = _skyX->getAtmosphereManager()->getOptions();

    switch(evt.key)
    {

    case OIS::KC_M:
        std::cout << "ssao toggle!" << std::endl;
        _ssaoInstance->setEnabled(!_ssaoInstance->getEnabled());
        break;
    case OIS::KC_B:

        std::cout << "HDR toggle!" << std::endl;
        //_bloomInstance->setEnabled(!_bloomInstance->getEnabled());
        //_hdrCompositor->Enable(!_hdrCompositor->IsEnabled());
        _ogreHdr->setEnabled(!_ogreHdr->getEnabled());
        break;

    case OIS::KC_N:

        _bloomInstance->setEnabled(!_bloomInstance->getEnabled());
        break;

    case OIS::KC_L:
        _skyX->setLightingMode(SkyX::SkyX::LM_HDR);
        break;
    case OIS::KC_K:
        _skyX->setLightingMode(SkyX::SkyX::LM_LDR);
        break;
    case OIS::KC_V:
        _skyX->setTimeMultiplier(0.001f);
        break;
    case OIS::KC_MINUS:
        _skyX->setTimeMultiplier(1.0f);
        break;

    case OIS::KC_EQUALS:
        _skyX->setTimeMultiplier(0.1f);
        break;

    case OIS::KC_1:
        _skyX->setTimeMultiplier(1.0f);
        break;

    case OIS::KC_2:
        _skyX->setTimeMultiplier(0.1f);
        break;

    case OIS::KC_3:
        SkyXOptions.Exposure += 0.16 * 0.5f;
        break;
    case OIS::KC_4:
        SkyXOptions.Exposure -= 0.16 * 0.5f;
        break;
    case OIS::KC_5:
        SkyXOptions.InnerRadius += 0.16 * 0.25f;
        break;
    case OIS::KC_6:
        SkyXOptions.InnerRadius -= 0.16 * 0.25f;
        break;
    case OIS::KC_7:
        SkyXOptions.OuterRadius += 0.16 * 0.25f;
        break;
    case OIS::KC_8:
        SkyXOptions.OuterRadius -= 0.16 * 0.25f;

    case OIS::KC_9:
        SkyXOptions.SunIntensity += 10;
        break;
    case OIS::KC_0:
        SkyXOptions.SunIntensity -= 10;
        break;
    case OIS::KC_Y:

        SkyXOptions.HeightPosition += 0.16 * 0.05f;
        break;
    case OIS::KC_U:

        SkyXOptions.HeightPosition -= 0.16*0.05f;
        break;
    default:
        break;
    }
    _skyX->getAtmosphereManager()->setOptions(SkyXOptions);

    return true;
}

bool GraphicsController::onFrameEnded(const Ogre::FrameEvent& evt)
{
    //_hdrCompositor->update(); //need to call this during the frame started phase.
    return true;
}


bool
    GraphicsController::onUpdate(const Ogre::FrameEvent &evt)
{
    using namespace Ogre;
    
    //Update HDR
    //_hdrCompositor->SetFrameTime(evt.timeSinceLastFrame);

    //pass int he Skylight SH coefficients
    //Compute theta and phi and get turbulence
    Vector3 xyz = -_skyX->getAtmosphereManager()->getSunDirection();
    //Vector3 xyz = Ogre::Vector3::UNIT_Y;
    Ogre::Radian theta = Math::ACos(xyz.y);
    Ogre::Radian phi = Math::ATan2(xyz.x, xyz.z);

    Ogre::String shr("SHC_R_");
    Ogre::String shg("SHC_G_");
    Ogre::String shb("SHC_B_");

    Real turbulence = 3.5;
    
    if(xyz.y >= 0.0f)
    {
        CalculatePreethamSH(theta.valueRadians(),phi.valueRadians(),turbulence, NUM_OF_BANDS, true, _SHC_R, _SHC_G, _SHC_B, 0.000035f);
        CalculateSunSH(theta.valueRadians(), phi.valueRadians(), turbulence, NUM_OF_BANDS, _SHC_R, _SHC_G, _SHC_B, 0.000035f);
    }
    Ogre::MaterialPtr matPtr = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName("PRJZ/Minecraft"));
    Ogre::Pass* pass = matPtr->getTechnique(0)->getPass(0);
    matPtr = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName("PRJZ/MinecraftCharacter"));
    Ogre::Pass* passC = matPtr->getTechnique(0)->getPass(0);
    size_t idx = 0;
    for(size_t i=0; i < 9; ++i)
    {
        String namer = shr+StringConverter::toString(idx);
        String nameg = shg+StringConverter::toString(idx);
        String nameb = shb+StringConverter::toString(idx);
        pass->getFragmentProgramParameters()->setNamedConstant(namer,_SHC_R[i]);
        pass->getFragmentProgramParameters()->setNamedConstant(nameg,_SHC_G[i]);
        pass->getFragmentProgramParameters()->setNamedConstant(nameb,_SHC_B[i]);
        passC->getFragmentProgramParameters()->setNamedConstant(namer,_SHC_R[i]);
        passC->getFragmentProgramParameters()->setNamedConstant(nameg,_SHC_G[i]);
        passC->getFragmentProgramParameters()->setNamedConstant(nameb,_SHC_B[i]);
        idx++;
    }
    //Skip band 4th band (or 3rd degree).
    for(size_t i=16; i < 25; ++i)
    {
        String namer = shr+StringConverter::toString(idx);
        String nameg = shg+StringConverter::toString(idx);
        String nameb = shb+StringConverter::toString(idx);
        pass->getFragmentProgramParameters()->setNamedConstant(namer,_SHC_R[i]);
        pass->getFragmentProgramParameters()->setNamedConstant(nameg,_SHC_G[i]);
        pass->getFragmentProgramParameters()->setNamedConstant(nameb,_SHC_B[i]);
        passC->getFragmentProgramParameters()->setNamedConstant(namer,_SHC_R[i]);
        passC->getFragmentProgramParameters()->setNamedConstant(nameg,_SHC_G[i]);
        passC->getFragmentProgramParameters()->setNamedConstant(nameb,_SHC_B[i]);
        idx++;
    }

    //sun direction for light
    //light
    
    pass->getFragmentProgramParameters()->setNamedConstant("uLightY", -_skyX->getAtmosphereManager()->getSunDirection().y);
    passC->getFragmentProgramParameters()->setNamedConstant("uLightY", -_skyX->getAtmosphereManager()->getSunDirection().y);
    //pass->getFragmentProgramParameters()->setNamedConstant("uLightY", xyz.y);
    _skyX->update(evt.timeSinceLastFrame);
    return true;
}

bool
    GraphicsController::onDestroy()
{
    _logic.reset(0);
    _hdrCompositor.reset(0);
    _skyX.reset(0);
    Ogre::CompositorManager::getSingleton().removeCompositor(_vp, "ssao");
    return true;
}

void
    GraphicsController::_initShadows()
{
    _scnMgr->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED);
    //three textures per directional light
    _scnMgr->setShadowTextureCountPerLightType(Ogre::Light::LT_DIRECTIONAL, 3);
    //_scnMgr->setShadowTextureSettings(1024, 3, PF_FLOAT32_R);
    _scnMgr->setShadowTextureSelfShadow(false);
    _scnMgr->setShadowFarDistance(512.0);

    //Leave only directional light //PRJZ/PSSM/shadow_caster
    _scnMgr->setShadowTextureCasterMaterial("PRJZ/PSSM/shadow_caster");
    //shadow camera setup
    PSSMShadowCameraSetup* pssmSetup = new PSSMShadowCameraSetup();
    pssmSetup->calculateSplitPoints(3, 1.0, 512.0);
    pssmSetup->setSplitPadding(1.0);
    pssmSetup->setOptimalAdjustFactor(0, 5);
    pssmSetup->setOptimalAdjustFactor(1, 2);
    pssmSetup->setOptimalAdjustFactor(2, 0.5);
    pssmSetup->setUseAggressiveFocusRegion(true);
    _pssmSetup.bind(pssmSetup);
    _scnMgr->setShadowCameraSetup(_pssmSetup);

    _scnMgr->setShadowTextureCount(3);
    //2048
    _scnMgr->setShadowTextureConfig(0, 512, 512, PF_FLOAT32_R);
    _scnMgr->setShadowTextureConfig(1, 512, 512, PF_FLOAT32_R);
    _scnMgr->setShadowTextureConfig(2, 512, 512, PF_FLOAT32_R);

    _scnMgr->setShadowTextureSelfShadow(true);
    _scnMgr->setShadowCasterRenderBackFaces(true);
    //_scnMgr->setShadowTextureCasterMaterial("PRJZ/PSSM/shadow_caster");

    MaterialPtr ret = MaterialManager::getSingleton().getByName("PRJZ/Minecraft");
    if(!ret.isNull())
    {
        Pass* p = ret->getTechnique(0)->getPass(0);
        Vector4 splitPoints;
        const PSSMShadowCameraSetup::SplitPointList& splitPointList = 
            pssmSetup->getSplitPoints();
        for(size_t i = 0; i < 3; ++i)
        {
            splitPoints[i] = splitPointList[i];
        }
        p->getFragmentProgramParameters()->setNamedConstant("pssmSplitPoints", splitPoints);
        //p->getTextureUnitState("diffuse")->setTextureName("BumpyMetal.jpg");
    }
    else
    {
        OGRE_EXCEPT( Ogre::Exception::ExceptionCodes::ERR_ITEM_NOT_FOUND, 
            "A material that supports shadowing is not found!", "GraphicsController::_initShadows");
    }
}

void
    GraphicsController::_initSSAO()
{
    _ssaoInstance = Ogre::CompositorManager::getSingleton().addCompositor(_vp, "ssao");
    _ssaoInstance->addListener(&_ssaoListener);
}

bool
    GraphicsController::adjustShadow(const Ogre::StringVector& params)
{
    if(params.size() != 4)
    {
        return false;
    }
    Real oneFactor, twoFactor, threeFactor;
    std::istringstream iss;
    iss.str(params[1] + " " + params[2] + " " + params[3]);
    iss >> oneFactor; iss >> twoFactor; iss >> threeFactor;
    Ogre::PSSMShadowCameraSetup* ptr = static_cast<Ogre::PSSMShadowCameraSetup*>(_pssmSetup.get());
    ptr->setOptimalAdjustFactor(0, oneFactor);
    ptr->setOptimalAdjustFactor(1, twoFactor);
    ptr->setOptimalAdjustFactor(2, threeFactor);

    return true;
}

void
    GraphicsController::_toggleAllCompositors(bool enable)
{
    if(enable)
    {
        _ssaoInstance->setEnabled(_compositorState[0]); //hardcoded
        //_bloomInstance->setEnabled(_compositorState[1]);
        //_hdrCompositor->Enable(_compositorState[2]);
    }
    else 
    {
        _compositorState[0] = _ssaoInstance->getEnabled();
        //_compositorState[1] = _bloomInstance->getEnabled();
        //_compositorState[2] = _hdrCompositor->IsEnabled();

        _ssaoInstance->setEnabled(false); //hardcoded
        //_bloomInstance->setEnabled(false);
        //_hdrCompositor->Enable(false);

    }
}
