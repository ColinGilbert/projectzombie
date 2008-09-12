/*
 * ImposterGen.h
 *
 *  Created on: Sep 4, 2008
 *      Author: bey0nd
 */

#ifndef IMPOSTERGEN_H_
#define IMPOSTERGEN_H_


#include <string>
#include <vector>
#include <map>
using namespace std;

#include <Ogre.h>
#include "Imposter.h"

/**
 * This class defines a generator for Imposters. This class will generate a set of Imposters in a spherical coordinate framework,
 * such that the textures are defined in segements along theta and phi.
 */

namespace ZGame
{
  class ImposterGen
  {
  public:
    ImposterGen();
    ~ImposterGen();

    void build();
    void setInput(Imposter* input);

    void rotateLeft();
    void rotateUp();
    void rotateRight();
    void rotateDown();


  protected:

    Ogre::SceneNode* _imposterNode;

    Ogre::Camera* _cam;
    Ogre::SceneNode* _camNode;
    Ogre::SceneManager* _scnMgr;


    void loadMesh();
    void setupRTT();
    void setupCam();
    void renderToTextures(); //render to the imposter to textures

    void camRotateTheta(Ogre::Radian& rot); //position the camera on theta
    void camRotatePhi(Ogre::Radian& rot); //position the camera on phi

  private:

    static int _id;
    int _curId;

    map<string,string> _imposterKeys;

    Imposter* _imposter;

    static const Ogre::Real _NEAR_CLIP = 1.0f;
    static const Ogre::Real _FAR_CLIP = 2000.0f;
    static const Ogre::Real _ASPECT_RATIO = 1.0f; //tex width / tex_height, in our case is 1 since our texture is square.

    Ogre::Real _rotVal;
    Ogre::Vector3 _cen;
    Ogre::Real _camOffset;

    void fitImposterExtent(Ogre::Entity* ent,Ogre::SceneNode* node,Ogre::Real defaultZ); //method to fit the imposter's bounding extend to screen.

  };

}

#endif /* IMPOSTERGEN_H_ */
