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
using namespace std;

#include <Ogre.h>

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
    void setInput(const string meshName);

  protected:

    static const int _segTheta = 10; //number of segements along theta.
    static const int _segPhi = 10; //number of segements aglong phi.

    Ogre::uint16 _texId;
    string _meshName;

    Ogre::Camera* _cam;
    Ogre::SceneManager* _scnMgr;

    vector<Ogre::TexturePtr> _impTex[_segPhi];

    void loadMesh();
    void setupRTT();
    void setupCam();

  private:
    string _CAMERA_NAME;
    static const Ogre::Real _NEAR_CLIP = 0.2f;
    static const Ogre::Real _FAR_CLIP = 10.0f;
    static const Ogre::Real _TEXDIM = 256.0f;
    static const Ogre::Real _ASPECT_RATIO = 1.0f; //tex width / tex_height, in our case is 1 since our texture is square.
  };

}

#endif /* IMPOSTERGEN_H_ */
