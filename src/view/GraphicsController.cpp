/*
 * GraphicsController.cpp
 *
 *  Created on: Aug 20, 2010
 *      Author: beyzend
 */
#include "GraphicsController.h"
#include "EngineView.h"
#include "ogre/SSAOLogic.h"
#include "ogre/DeferredLightCP.h"
#include "ogre/GBufferSchemeHandler.h"
#include "ogre/NullSchemeHandler.h"


using ZGame::GraphicsController;
using namespace Ogre;

GraphicsController::GraphicsController()
{
}

GraphicsController::~GraphicsController()
{
}

bool
GraphicsController::onInit()
{
  Ogre::LogManager* lm = Ogre::LogManager::getSingletonPtr();
  lm->logMessage(Ogre::LML_TRIVIAL, "In GraphicsController::onInit()");
  SceneManager* scnMgr = EngineView::getSingleton().getSceneManager();
  Viewport* vp = EngineView::getSingleton().getCurrentCamera()->getViewport();
  lm->logMessage(Ogre::LML_NORMAL, "Adding compositor bloom");

  CompositorManager &compMan = CompositorManager::getSingleton();

  compMan.addCompositor(vp, "Bloom");
  compMan.setCompositorEnabled(vp, "Bloom", true);


  scnMgr->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED);
  scnMgr->setShadowColour(Ogre::ColourValue(0.5, 0.5, 0.5));
  scnMgr->setShadowFarDistance(10000);

  scnMgr->setShadowDirectionalLightExtrusionDistance(75);
  //scnMgr->setShadowTextureSettings(512, 2);

  Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(
      Ogre::TFO_ANISOTROPIC);
  Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(7);

  /*
  //lm->logMessage(Ogre::LML_NORMAL, "Adding deffered shading and SSAO.");

  MaterialManager::getSingleton().addListener(new GBufferSchemeHandler, "GBuffer");
  MaterialManager::getSingleton().addListener(new NullSchemeHandler, "NoGBuffer");


  compMan.registerCompositorLogic("SSAOLogic", new SSAOLogic());
  compMan.registerCustomCompositionPass("DeferredLight", new DeferredLightCompositionPass);

  _gBufferInstance = compMan.addCompositor(vp, "DeferredShading/GBuffer");
  _gBufferInstance->setEnabled(true);
  _ssaoInstance = compMan.addCompositor(vp, "DeferredShading/SSAO");
  _ssaoInstance->setEnabled(true);
  scnMgr->setShadowTextureCasterMaterial("DeferredShading/Shadows/Caster");

  scnMgr->setShadowTextureCount(1);
  scnMgr->setShadowTextureSize(512);
  scnMgr->setShadowTexturePixelFormat(PF_FLOAT16_R);
  */
}

bool
GraphicsController::onUpdate()
{

}

bool
GraphicsController::onDestroy()
{

}
