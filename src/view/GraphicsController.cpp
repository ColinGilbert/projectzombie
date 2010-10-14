/*
 * GraphicsController.cpp
 *
 *  Created on: Aug 20, 2010
 *      Author: beyzend
 */
#include <iostream>
#include "GraphicsController.h"
#include "EngineView.h"
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
      _ADAPT_SCALE(3), _GLARE_STRENGTH(0.1), _STAR_STRENGTH(0.1)
{
}

GraphicsController::~GraphicsController()
{
  //delete _hdrCompositor;
}

bool
GraphicsController::onInit()
{

  Ogre::LogManager* lm = Ogre::LogManager::getSingletonPtr();
  lm->logMessage(Ogre::LML_TRIVIAL, "In GraphicsController::onInit()");
  _scnMgr = EngineView::getSingleton().getSceneManager();
  _vp = EngineView::getSingleton().getCurrentCamera()->getViewport();
  lm->logMessage(Ogre::LML_NORMAL, "Adding compositor bloom");

  //First build the lights.
  Vector3 lightDir(1.0f, -0.3f, 0.0f);
  lightDir.normalise();
  //_scnMgr->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));
  Light* light = _scnMgr->createLight("terrainLight");
  light->setType(Light::LT_DIRECTIONAL);
  light->setDirection(lightDir);
  light->setDiffuseColour(ColourValue(1.0, 1.0, 1.0));
  light->setSpecularColour(ColourValue(1.0f, 1.0f, 1.0f));

  _bloomInstance = CompositorManager::getSingleton().addCompositor(_vp, "Bloom");
  //CompositorManager::getSingleton().setCompositorEnabled(_vp,"Bloom",true);
  _ssaoListener.setCamera(EngineView::getSingleton().getCurrentCamera());
  Ogre::ColourValue fadeColour(1.0f, 0.35f, 0.0f);
  _scnMgr->setFog(Ogre::FOG_LINEAR, fadeColour, 0.0, 500.0f, 600.0f);
  _vp->setBackgroundColour(fadeColour);
 
  this->_parseHDRConfig();
  _initHDR();
  _initSSAO();
  _initShadows();
  _ssaoInstance->setEnabled(true);
  _hdrCompositor->Enable(true);
   _scnMgr->setSkyBox(true, "Examples/MorningSkyBox", 10, true);
  return true;
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
           
            if(typeName.compare("TONEMAPPER") == 0)
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

  Ogre::CompositorManager::getSingleton().registerCompositorLogic("HDR", logic);
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
  if (evt.key == OIS::KC_M)
    {
      std::cout << "ssao toggle!" << std::endl;
      _ssaoInstance->setEnabled(!_ssaoInstance->getEnabled());
    }
  if (evt.key == OIS::KC_B)
    {
      //_bloomInstance->setEnabled(!_bloomInstance->getEnabled());
      _hdrCompositor->Enable(!_hdrCompositor->IsEnabled());
    }
  if (evt.key == OIS::KC_N)
    {
      _bloomInstance->setEnabled(!_bloomInstance->getEnabled());
    }
  return true;
}

bool
GraphicsController::onKeyDown(const OIS::KeyEvent &evt)
{
    return true;
}

bool
GraphicsController::onUpdate(const Ogre::FrameEvent &evt)
{
  _hdrCompositor->SetFrameTime(evt.timeSinceLastFrame);
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
  // we'll be self shadowing
  _scnMgr->setShadowTextureSelfShadow(false);
  //_scnMgr->setShadow

  // our caster material
  _scnMgr->setShadowTextureCasterMaterial("shadow_caster");
  // note we have no "receiver".  all the "receivers" are integrated.

  // get the shadow texture count from the cfg file
  //std::string tempData = _scnMgr.cfgFile.getSetting("shadowTextureCount", "renderer");
  std::string tempData = "256";
  // (each light needs a shadow texture)
  _scnMgr->setShadowTextureCount(Ogre::StringConverter::parseInt(tempData));

  // the size, too (1024 looks good with 3x3 or more filtering)
  //tempData = _scnMgr.cfgFile.getSetting("shadowTextureRes", "renderer");
  tempData = "256";
  _scnMgr->setShadowTextureSize(Ogre::StringConverter::parseInt(tempData));

  // float 16 here.  we need the R and G channels.
  // float 32 works a lot better with a low/none VSM epsilon (wait till the shaders)
  // but float 16 is good enough and supports bilinear filtering on a lot of cards
  // (we should use _GR, but OpenGL doesn't really like it for some reason)
  _scnMgr->setShadowTexturePixelFormat(Ogre::PF_FLOAT16_RGB);

  // big NONO to render back faces for VSM.  it doesn't need any biasing
  // so it's worthless (and rather problematic) to use the back face hack that
  // works so well for normal depth shadow mapping (you know, so you don't
  // get surface acne)
  _scnMgr->setShadowCasterRenderBackFaces(false);

  const unsigned numShadowRTTs = _scnMgr->getShadowTextureCount();
  for (unsigned i = 0; i < numShadowRTTs; ++i)
    {
      Ogre::TexturePtr tex = _scnMgr->getShadowTexture(i);
      Ogre::Viewport *vp = tex->getBuffer()->getRenderTarget()->getViewport(0);
      vp->setBackgroundColour(Ogre::ColourValue(1, 1, 1, 1));
      vp->setClearEveryFrame(true);
    }

  // enable integrated additive shadows
  // actually, since we render the shadow map ourselves, it doesn't
  // really matter whether they are additive or modulative
  // as long as they are integrated v(O_o)v
  //_scnMgr->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED);
  _scnMgr->setShadowTechnique(Ogre::SHADOWTYPE_NONE);

  // and add the shader listener
  _scnMgr->addListener(&_shadowListener);
}

void
GraphicsController::_initSSAO()
{
  _ssaoInstance = Ogre::CompositorManager::getSingleton().addCompositor(_vp, "ssao");
  _ssaoInstance->addListener(&_ssaoListener);
  
}

