/*
 * Imposter.h
 *
 *This class defines a state class for imposters.
 *  Created on: Sep 11, 2008
 *      Author: bey0nd
 */

#ifndef IMPOSTER_H_
#define IMPOSTER_H_

#include <string>
#include <vector>
using namespace std;

#include <Ogre.h>

namespace ZGame
{

  class Imposter
  {
  public:
    static const size_t TEXDIM = 512;
    static const size_t SEGTHETA = 12;
    static const size_t SEGPHI = 5;

    Imposter(const string meshName);
    ~Imposter();

    const string getMeshName(){return _meshName;}
    Ogre::TexturePtr getTextures(){return _texture;}

    int getWidth(){return SEGTHETA*TEXDIM;}
    int getHeight(){return SEGPHI*TEXDIM;}
    int getDim(){return TEXDIM;}


  protected:
    string _meshName;
    Ogre::TexturePtr _texture;

    void setupTextures();

  private:
    static int _id; //global unique id for imposter instances
    int _curId; //current id for this imposter

    const string TYPE_NAME;


  };
}


#endif /* IMPOSTER_H_ */
