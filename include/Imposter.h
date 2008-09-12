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
  typedef vector<Ogre::TexturePtr> ImposterTexVec;

  class Imposter
  {
  public:
    static const size_t TEXDIM = 256;
    static const size_t SEGTHETA = 12;
    static const size_t SEGPHI = 4;

    Imposter(const string meshName);
    ~Imposter();

    const string getMeshName(){return _meshName;}
    vector<ImposterTexVec>* getTextures(){return &_texs;}

  protected:
    string _meshName;
    vector<ImposterTexVec> _texs;

    void setupTextures();

  private:
    static int _id; //global unique id for imposter instances
    int _curId; //current id for this imposter

    const string TYPE_NAME;

  };
}


#endif /* IMPOSTER_H_ */
