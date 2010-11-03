/*
* GraphicsController.cpp
*
*  Created on: Aug 20, 2010
*      Author: beyzend
*/
#include <iostream>
#include <sstream>
#include "GraphicsController.h"
#include "EngineView.h"
#include "CommandController.h"
#include "command/CommandList.h"
#include "CommandDelegates.h"
#include "graphics/PreethamSH.h"
#include "graphics/SunSH.h"
using std::cout;
using std::endl;
//#include "ogre/SSAOLogic.h"
//#include "ogre/DeferredLightCP.h"
//#include "ogre/GBufferSchemeHandler.h"
//#include "ogre/NullSchemeHandler.h"


using ZGame::GraphicsController;
using namespace Ogre;

GraphicsController::GraphicsController() :
_hdrCompositor(0), _WHICH_TONEMAPPER(3), _WHICH_STARTYPE(1), _WHICH_GLARETYPE(1), _AUTO_KEY(0.16),
    _ADAPT_SCALE(3), _GLARE_STRENGTH(0.1), _STAR_STRENGTH(0.1), _skyX(0),_timeCount(0.0f)
{
}

GraphicsController::~GraphicsController()
{
    cout << "In graphicsController destructor." << endl;
    //delete _hdrCompositor;
}

bool
    GraphicsController::onInit()
{
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
    _scnMgr = EngineView::getSingleton().getSceneManager();
    _vp = EngineView::getSingleton().getCurrentCamera()->getViewport();
    lm->logMessage(Ogre::LML_NORMAL, "Adding compositor bloom");

    //First build the lights.
    Vector3 lightDir(1.0f, -0.6f, 0.0f);
    lightDir.normalise();
    //_scnMgr->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));
    Light* light = _scnMgr->createLight("terrainLight");
    light->setType(Light::LT_DIRECTIONAL);
    light->setDirection(lightDir);
    light->setDiffuseColour(ColourValue(1.0, 1.0, 1.0));
    light->setSpecularColour(ColourValue(0.1f, 0.1f, 0.1f));

    _bloomInstance = CompositorManager::getSingleton().addCompositor(_vp, "Bloom");
    //CompositorManager::getSingleton().setCompositorEnabled(_vp,"Bloom",true);
    _ssaoListener.setCamera(EngineView::getSingleton().getCurrentCamera());
    Ogre::ColourValue fadeColour(0.109f, 0.417f, 0.625f);
    //_scnMgr->setFog(Ogre::FOG_LINEAR, fadeColour, 0.0, 500.0f, 600.0f);
    _scnMgr->setFog(Ogre::FOG_NONE);
    _vp->setBackgroundColour(fadeColour);

    this->_parseHDRConfig();

    _initHDR();
    _initSSAO();
    _initSkyX();
    _ssaoInstance->setEnabled(true);
    //_hdrCompositor->Enable(true);
    //_scnMgr->setSkyBox(true, "Examples/MorningSkyBox", 10, true);
    //_initShadows();
    _scnMgr->setShadowTechnique(Ogre::SHADOWTYPE_NONE);
    return true;
}

void
    GraphicsController::_initSkyX()
{


    SkyX::SkyX* sx = new SkyX::SkyX(_scnMgr, _vp->getCamera());
    _skyX.reset(sx);

    //Upadte SkyX
    SkyX::AtmosphereManager::Options SkyXOptions = _skyX->getAtmosphereManager()->getOptions();
    SkyXOptions.EastPosition = Ogre::Vector2(1, 0);
    _skyX->getAtmosphereManager()->setOptions(SkyXOptions);
    _skyX->setLightingMode(SkyX::SkyX::LM_LDR);
    _skyX->create();

    //Add ground atomospheric scattering pass to our terrain.
    _skyX->getGPUManager()->addGroundPass(
        static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().
        getByName("PRJZ/Minecraft"))->getTechnique(0)->createPass(), 128, Ogre::SBT_TRANSPARENT_COLOUR);
    //_skyX->getCloudsManager()->add(SkyX::CloudLayer::Options());
    

}


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
    GraphicsController::_initHDR()
{
    _hdrCompositor = new HDRCompositor(EngineView::getSingleton().renderWindow, EngineView::getSingleton().getCurrentCamera());
    ListenerFactoryLogic* logic = new ListenerFactoryLogic;
    logic->setCompositorListener(_hdrCompositor);

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
    _hdrCompositor->SetStarPasses(4);
    _hdrCompositor->SetGlarePasses(2);
    _hdrCompositor->SetGlareStrength(_GLARE_STRENGTH);
    _hdrCompositor->SetStarStrength(_STAR_STRENGTH);
    _hdrCompositor->Create();
    cout << "Done enabling HDR." << endl;
}

bool
    GraphicsController::onKeyUp(const OIS::KeyEvent &evt)
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
        _hdrCompositor->Enable(!_hdrCompositor->IsEnabled());
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
    case OIS::KC_1:
        _skyX->setTimeMultiplier(1.0f);
        break;
    case OIS::KC_2:
        _skyX->setTimeMultiplier(0.1f);
        break;
    case OIS::KC_V:
        _skyX->setTimeMultiplier(0.001f);
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
    default:
        break;
        return true;
    }
    _skyX->getAtmosphereManager()->setOptions(SkyXOptions);
}

bool
    GraphicsController::onKeyDown(const OIS::KeyEvent &evt)
{
    return true;
}

bool
    GraphicsController::onUpdate(const Ogre::FrameEvent &evt)
{
    using namespace Ogre;
    

    

    
    //Update HDR
    _hdrCompositor->SetFrameTime(evt.timeSinceLastFrame);

    //pass int he Skylight SH coefficients
    //Compute theta and phi and get turbulence
    Vector3 xyz = -_skyX->getAtmosphereManager()->getSunDirection();
    
    //xyz.normalise();
    

    
    Ogre::Radian theta = Math::ACos(xyz.y);
    Ogre::Radian phi = Math::ATan2(xyz.x, xyz.z);
    
    //if(phi.valueRadians() < 0.0f)
        //phi=  Ogre::Radian(phi.valueRadians() + Math::PI * 2.0f);


    //xyz.dotProduct(Ogre::Vector3::UNIT_Y) < 0.0f ? theta = Math::PI - theta : theta = theta;
    //theta = Ogre::Radian(Math::PI - theta.valueRadians());
    Real turbulence = 3.5;
#if 1
    //_SHC_R[0] = 1.0f; _SHC_G[0] = 1.0f; _SHC_G[

    /*
     for(size_t i = 0; i < NUM_OF_BANDS * 4; i++)
    {
        _SHC_R[i] = 1.0f;
        _SHC_G[i] = 0.0f;
        _SHC_B[i] = 0.0f;
    }*/
#endif
     
    if(xyz.y >= 0.0f)
    {
        CalculatePreethamSH(theta.valueRadians(),phi.valueRadians(),turbulence, NUM_OF_BANDS, true, _SHC_R, _SHC_G, _SHC_B, 1.0f);
        CalculateSunSH(theta.valueRadians(), phi.valueRadians(), turbulence, NUM_OF_BANDS, _SHC_R, _SHC_G, _SHC_B, 1.0);
    }
    //_SHC_R[0] = 500.0f;
    //_SHC_G[0] = 500.0f;
    //_SHC_B[0] = 500.0f;
    /*
    else
    {
        theta = Radian(Math::PI - theta.valueRadians());
        phi = Radian(Math::PI * 2.0f - theta.valueRadians());
        CalculatePreethamSH(theta.valueRadians(), phi.valueRadians(),turbulence, NUM_OF_BANDS, true, _SHC_R, _SHC_G, _SHC_B, 0.0001f);
        CalculateSunSH(theta.valueRadians(), phi.valueRadians(), turbulence, NUM_OF_BANDS, _SHC_R, _SHC_G, _SHC_B, 0.0001f);
    }*/
    

    //theta = Math::ACos(xyz.y).valueRadians();
    //phi = Math::ATan2(xyz.x, xyz.z).valueRadians();


    //
    Ogre::MaterialPtr matPtr = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName("PRJZ/Minecraft"));
    Ogre::Pass* pass = matPtr->getTechnique(0)->getPass(0);
    size_t idx = 0;
    for(size_t i=0; i < 9; ++i)
    {
        String namer = "SHC_R_"+StringConverter::toString(idx);
        String nameg = "SHC_G_"+StringConverter::toString(idx);
        String nameb = "SHC_B_"+StringConverter::toString(idx);
        pass->getFragmentProgramParameters()->setNamedConstant(namer,_SHC_R[i]);
        pass->getFragmentProgramParameters()->setNamedConstant(nameg,_SHC_G[i]);
        pass->getFragmentProgramParameters()->setNamedConstant(nameb,_SHC_B[i]);
        idx++;
    }
    //Skip band 4th band (or 3rd degree).
    for(size_t i=16; i < 25; ++i)
    {
        String namer = "SHC_R_"+StringConverter::toString(idx);
        String nameg = "SHC_G_"+StringConverter::toString(idx);
        String nameb = "SHC_B_"+StringConverter::toString(idx);
        pass->getFragmentProgramParameters()->setNamedConstant(namer,_SHC_R[i]);
        pass->getFragmentProgramParameters()->setNamedConstant(nameg,_SHC_G[i]);
        pass->getFragmentProgramParameters()->setNamedConstant(nameb,_SHC_B[i]);
        idx++;
    }
    //sun direction for light
    //light
    //static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName("PRJZ/Minecraft"))->getTechnique(0)->getPass(0)->
        pass->getFragmentProgramParameters()->setNamedConstant("uLightY", -_skyX->getAtmosphereManager()->getSunDirection().y);

    _skyX->update(evt.timeSinceLastFrame);
#if 0
    if(_timeCount > 1.0f || _timeCount == 0.0f)
    {
        for(size_t i = 0; i < NUM_OF_BANDS * NUM_OF_BANDS; i++)
        {
            cout << "R,G,B: " << _SHC_R[i] << ", " << _SHC_G[i] << ", " << _SHC_B[i] << endl;
        }

       cout << "theta, phi: " << theta.valueDegrees() << ", " << phi.valueDegrees() << endl;
       cout << "sun direction: " << xyz << endl;
       if(_timeCount > 0.0f)
           _timeCount = 0.0f;
    }
    _timeCount += evt.timeSinceLastFrame;
#endif
    return true;
}

bool
    GraphicsController::onDestroy()
{
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

