/*
 * GPUEntsLoader.h
 * This is a controller class which loads a bunch of GPU entities into a GPU entities container.
 *  Created on: Sep 18, 2008
 *      Author: bey0nd
 */

#ifndef GPUENTSLOADER_H_
#define GPUENTSLOADER_H_
#include <memory>
using namespace std;

#include <Ogre.h>

namespace ZGame
{
  class GPUEntities;
  class ZEntity;
  class GPUEntsGenProps;
  class GPUEntsGen
  {
  public:
    GPUEntsGen(ZEntity* ent,auto_ptr<GPUEntsGenProps> props);
    virtual ~GPUEntsGen();

    void build();

    auto_ptr<GPUEntities> getOutput(); //returns GPUEntities. You know, the thing that this generator generates.

  protected:

    GPUEntities* _output;
    ZEntity* _ent;
    auto_ptr<GPUEntsGenProps> _props;
    Ogre::TexturePtr _stateTex;
    Ogre::TexturePtr _imposterTex;
    void genImposters();
    void loadPositions();
    void genEntsGeom();//generate the entities' geometry
    void createStateTexture(); //Place-holder method for creating GPU entities state texture.
    size_t nextId(); //get the next availible id.

    const string _gpuEntsName;

    static size_t _uniqueId; //a unqiue id

  private:
  };
}


#endif /* GPUENTSLOADER_H_ */
