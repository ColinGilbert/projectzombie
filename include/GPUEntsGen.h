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
#include <boost/weak_ptr.hpp>
#include <Ogre.h>

namespace ZGame
{
  class GPUEntities;
  class ZEntity;
  class GPUEntsGenProps;
  class GPUEntsGen
  {
  public:
    GPUEntsGen(const boost::shared_ptr<ZEntity> &ent,
        auto_ptr<GPUEntsGenProps> props);
    virtual
    ~GPUEntsGen();

    void
    build();

    auto_ptr<GPUEntities>
    getOutput(); //returns GPUEntities. You know, the thing that this generator generates.

  protected:

    auto_ptr<GPUEntities> _output;
    //auto_ptr<GPUEntities> _output;
    boost::shared_ptr<ZEntity> _ent;
    auto_ptr<GPUEntsGenProps> _props;
    Ogre::TexturePtr _stateTex;
    Ogre::TexturePtr _imposterTex;
    Ogre::TexturePtr _dirTex;
    void
    genImposters();
    void
    loadPositions();
    void
    genEntsGeom();//generate the entities' geometry
    void
    createStateTexture(); //Place-holder method for creating GPU entities state texture.
    size_t
    nextId(); //get the next availible id.
    void
    loadDirections();
    void
    createDirTexture();
    const Ogre::String _gpuEntsName;

    static size_t _uniqueId; //a unqiue id

  private:
  };
}

#endif /* GPUENTSLOADER_H_ */
