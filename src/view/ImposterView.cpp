/*
 * ImposterView.cpp
 *
 *  Created on: Sep 11, 2008
 *      Author: bey0nd
 */
#include <vector>
#include <ostream>
#include <stringstream>
using namespace std;
#include "Imposter.h"
#include "ImposterView.h"
#include "EngineView.h"
using namespace ZGame;

ImposterView::ImposterView():_imposter(0),_texNode(0),_TYPENAME("IMPOSTERVIEW"),
_texEnt(0),_curFlipTheta(0),_curFlipPhi(0),_MATERIALNAME("ZGame/Imposter")
{

}

ImposterView::~ImposterView()
{
  using namespace Ogre;
  SceneManager* scnMgr = EngineView::getSingleton().getSceneManager();
  scnMgr->destroyEntity(_texEnt);
  scnMgr->destroySceneNode(_texNode->getName());
}
void ImposterView::flip()
{
  //switchTexture();
}
void ImposterView::setInput(Imposter* input)
{
  _imposter = input;
  setupView();
  attachMaterial();
  switchTexture();
}

void ImposterView::setupView()
{
  using namespace Ogre;
  Plane plane(Vector3::UNIT_Y,0);
  string name = _TYPENAME+"PLANE";
  MeshManager::getSingleton().createPlane(name.c_str(),
      ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,plane,1500,1500,1,1,true,1,1,1,Vector3::UNIT_Z);
  SceneManager* scnMgr = EngineView::getSingleton().getSceneManager();
  string planeName = name+"ENTITY";
  _texEnt = scnMgr->createEntity(planeName,name);
  name.clear(); name = _TYPENAME+"PLANENODE";
  _texNode = scnMgr->getRootSceneNode()->createChildSceneNode(name,Vector3(0.0f,-1.0f,0.0f));
  _texNode->attachObject(_texEnt);
}

void ImposterView::switchTexture()
{
  using namespace Ogre;
  MaterialPtr mat = getMaterial();
  TextureUnitState *texState = mat->getTechnique(0)->getPass(0)->getTextureUnitState(0);
  texState->setTextureName(_imposter->getTextures()->getName());
  /*
  using namespace Ogre;
  ostringstream ss;
  ss << "In ImposterView::setupTexture()" << endl;
   = _imposter->getTextures();
  //NOTE: FOr testing right now we are going to attach multiple textures to an object.
  //Later, after making sure everything works, we need to switch over to using a
  //animated texture. I.E. a single texture that contains multiple sub-textures. For optimal performance.
  //attach the first texture to the material
  MaterialPtr mat = getMaterial();
  TextureUnitState *texState = mat->getTechnique(0)->getPass(0)->getTextureUnitState(0);
  TexturePtr tex = texs->at(_curFlipPhi)[_curFlipTheta++];
  const String texName = tex->getName();
  ss << "Initial Texture name: " << texName << endl;
  texState->setTextureName(tex->getName());
  ss << "flipPhi,flipTheta: " << _curFlipPhi << "," << _curFlipTheta << endl;
  if(_curFlipTheta >= texs->at(_curFlipPhi).size())
    {
      _curFlipTheta = 0; //start over on theta
      _curFlipPhi++; //goto next phi value
    }
  if(_curFlipPhi >= texs->size())
    _curFlipPhi = 0; //start over on phi
  Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL,ss.str());
  */
}

Ogre::MaterialPtr ImposterView::getMaterial()
{
  return Ogre::MaterialManager::getSingleton().getByName(_MATERIALNAME.c_str());
}

void ImposterView::attachMaterial()
{
  using namespace Ogre;
  MaterialPtr mat = getMaterial();
  TextureUnitState *texState = mat->getTechnique(0)->getPass(0)->getTextureUnitState(0);
  _texEnt->setMaterialName(_MATERIALNAME.c_str());
}


