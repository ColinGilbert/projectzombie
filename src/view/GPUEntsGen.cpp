/*
* GPUEntsGen.cpp
*
*  Created on: Sep 18, 2008
*      Author: bey0nd
*/
//#include <string>
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include <stdexcept>
#else
#include <logic_error>
#endif

using namespace std;
#include <boost/random.hpp>
#include <OgreBitwise.h>
#include "GPUEntsGen.h"
#include "ZEntity.h"
#include "GPUEntities.h"
#include "Imposter.h"
#include "ImposterGen.h"
#include "GPUEntsGenProps.h"
#include "GPUEntsDistributor.h"
#include "EngineView.h"
using namespace ZGame;
using namespace Ogre;

size_t GPUEntsGen::_uniqueId = 0;

GPUEntsGen::GPUEntsGen(const boost::shared_ptr<Entities::ZEntity> &ent, auto_ptr<
                       GPUEntsGenProps> props) :
_output(0), _ent(ent), _props(props), _gpuEntsName("gpuEnts")
{
}

GPUEntsGen::~GPUEntsGen()
{
}

auto_ptr<GPUEntities>
GPUEntsGen::getOutput()
{
    if (_output.get() == 0)
        throw logic_error("GPUEntsGen output is null! Try calling build first.");
    return _output;
}

void
GPUEntsGen::build()
{

    LogManager* lm = LogManager::getSingletonPtr();
    lm->logMessage(Ogre::LML_TRIVIAL, "In GPUENtsGen::build()");
    try
    {
        genImposters(); //gen the imposters
    }
    catch (Ogre::Exception e)
    {
        ostringstream oss;
        oss
            << "Ogre exception thrown when generating imposters. GPUEntsGen::build()"
            << endl;
        throw Ogre::Exception(e.getNumber(), e.getDescription() + oss.str(),
            e.getSource());
    }
    lm->logMessage(Ogre::LML_NORMAL, "Finished generating imposters.");
    try
    {
        loadPositions(); //load the positions into the state texture
    }
    catch (Ogre::Exception e)
    {
        ostringstream oss;
        oss
            << " Ogre exception thrown when loading positions. GPUEntsGen::build()"
            << endl;
        throw Ogre::Exception(e.getNumber(), e.getDescription() + oss.str(),
            e.getSource());
    }
    lm->logMessage(Ogre::LML_NORMAL, "Finished loading state texture.");
    try
    {
        loadDirections(); //load the directions into direction texture.
    }
    catch (Ogre::Exception e)
    {
        ostringstream oss;
        oss
            << "Ogre exception thrown when loading directions. GPUEntsGen::build()"
            << endl;
        throw Ogre::Exception(e.getNumber(), e.getDescription() + oss.str(),
            e.getSource());
    }
    try
    {
        genEntsGeom();
    }
    catch (Ogre::Exception e)
    {
        ostringstream oss;
        oss
            << "Ogre exception thrown when generating ents geom. GPUEntsGen::build()"
            << endl;
        throw Ogre::Exception(e.getNumber(), e.getDescription() + oss.str(),
            e.getSource());
    }
    lm->logMessage(Ogre::LML_NORMAL, "Finished generating entities geometry");
    _output.reset(new GPUEntities(_gpuEntsName, _stateTex->getName().c_str(),
        _dirTex->getName().c_str(), _imposterTex->getName().c_str(), _props)); //now we have enough information to build output.
    lm->logMessage(LML_NORMAL,
        "Finished generating GPU Entities. Output is now availible.");
}

void
GPUEntsGen::genImposters()
{
    Ogre::LogManager::getSingleton().logMessage(Ogre::LML_TRIVIAL,
        "In GPUEntsGen::genImposters");
    Imposter imposter(_ent->getMeshName());
    imposter.init();
    ImposterGen impGen;
    impGen.setInput(&imposter);
    impGen.build();
    _imposterTex = imposter.getTextures();
}

void
GPUEntsGen::loadPositions()
{
    LogManager* lm = LogManager::getSingletonPtr();
    lm->logMessage(Ogre::LML_TRIVIAL, "In GPUEntsGen::loadPositions");
    createStateTexture();
    lm->logMessage(LML_TRIVIAL, "Finished creating textures");
    boost::minstd_rand rng;
    Vector3 min = _props->getExtents().getMinimum();
    Vector3 max = _props->getExtents().getMaximum();

    boost::uniform_int<> xDist(static_cast<int>(min.x), static_cast<int>(max.x));
    boost::uniform_int<> zDist(static_cast<int>(min.z), static_cast<int>(max.z));
    GPUEntsDistributor<boost::minstd_rand, boost::uniform_int<> > dist(rng,
        xDist, zDist);

    Vector3 pos;
    HardwarePixelBufferSharedPtr pixBuffer = _stateTex->getBuffer();
    pixBuffer->lock(HardwareBuffer::HBL_DISCARD);
    const PixelBox &pixBox = pixBuffer->getCurrentLock();
    Real* pDest = static_cast<Real*> (pixBox.data);
    size_t pitchY = pixBox.rowPitch;
    size_t pitchX = pixBox.slicePitch;
    Ogre::Log::Stream oss = Ogre::LogManager::getSingleton().stream(Ogre::LML_TRIVIAL);
    oss << "rowPitch,slicePitch: " << pitchY << "," << pitchX << "\n";
    size_t w = pixBox.getWidth();
    size_t h = pixBox.getHeight();
    oss << "width,height: " << w << "," << h << "\n";
    //for each texel in the state texture. ASSUMING FLOAT_RGBA. We need to address this assumption later, so we do NOT ASSUME it.
    for (size_t j = 0; j < w; j++)
    {
        for (size_t i = 0; i < h; i++)
        {
            dist.nextPosition(pos); //get randomly distributed position
            //PixelUtil::packColour((float)pos.x,(float)pos.y,0.0,0.0f,Ogre::PF_FLOAT32_RGBA,(void*)(&pDest[pitchY*j+i]));
            *pDest++ = pos.z; //b
            *pDest++ = pos.x;  //g
            *pDest++ = pos.y; //r
            *pDest++ = 0.0f; //a
        }
    }
    pixBuffer->unlock();
}

void
GPUEntsGen::loadDirections()
{
    LogManager* lm = LogManager::getSingletonPtr();
    lm->logMessage(Ogre::LML_TRIVIAL, "In GPUEntsGen::loadDirections()");
    createDirTexture();

    lm->logMessage(LML_TRIVIAL, "Finished creating textures");
    boost::minstd_rand rng;
    boost::uniform_real<> thetaDist(0.0, Math::TWO_PI);
    boost::variate_generator<boost::minstd_rand&, boost::uniform_real<> >
        randTheta(rng, thetaDist);
    Vector3 dir;
    dir.y = 0; //on the xz plane
    HardwarePixelBufferSharedPtr pixBuffer = _dirTex->getBuffer();
    pixBuffer->lock(HardwareBuffer::HBL_DISCARD);
    const PixelBox &pixBox = pixBuffer->getCurrentLock();
    Real* pDest = static_cast<Real*> (pixBox.data);
    size_t w = pixBox.getWidth();
    size_t h = pixBox.getHeight();
    for (size_t j = 0; j < h; ++j)
    {
        for (size_t i = 0; i < w; ++i)
        {
            //use dir = cos(theta)i + sin(theta)j
            Real theta = static_cast<Real>(randTheta());
            dir.x = Math::Cos(theta);
            dir.z = Math::Sin(theta);
            *pDest++ = dir.x;
            *pDest++ = dir.z;
            *pDest++ = 0.0f;
            *pDest++ = theta;
        }
    }
    pixBuffer->unlock();

}

/**
* This is a place holder for testing.
*/
void
GPUEntsGen::createStateTexture()
{

    ostringstream oss;
    oss << _gpuEntsName << "statetex" << nextId();
    Ogre::String texName = oss.str();

    _stateTex = TextureManager::getSingleton().createManual(texName,
        ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME, TEX_TYPE_2D,
        _props->getTexWidth(), _props->getTexHeight(), 0, Ogre::PF_FLOAT32_RGBA,
        Ogre::TU_RENDERTARGET);

}

void
GPUEntsGen::createDirTexture()
{
    ostringstream oss;
    oss << _gpuEntsName << "dirtex" << nextId();
    Ogre::String texName = oss.str();
    _dirTex = TextureManager::getSingleton().createManual(texName,
        ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME, TEX_TYPE_2D,
        _props->getTexWidth(), _props->getTexHeight(), 0, Ogre::PF_FLOAT32_RGBA,
        Ogre::TU_RENDERTARGET);

}

void
GPUEntsGen::genEntsGeom()
{

}

size_t
GPUEntsGen::nextId()
{
    return _uniqueId++;
}
