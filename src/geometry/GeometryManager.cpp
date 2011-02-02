/**
Permission is hereby granted by Fdastero LLC, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
**/
/**
*author: beyzend 
*email: llwijk@gmail.com
**/

#include "geometry/GeometryManager.h"
#include "ZInitPacket.h"
#include "Procedural.h"

using namespace ZGame::Geometry;

size_t GeometryManager::_geoNumber = 0;

GeometryManager::GeometryManager()
    : _CUBE_MESH_HANDLE(0)
{
   
}

GeometryManager::~GeometryManager()
{
}

bool
    GeometryManager::onInit(ZGame::ZInitPacket* initPacket)
{
    Procedural::Root::getInstance()->sceneManager = initPacket->sceneManager;
    _scnMgr = initPacket->sceneManager;
    _cam = initPacket->initialCamera;
     _createMeshes();
    return true;
}

bool
    GeometryManager::onUpdate(const Ogre::FrameEvent& evt)
{
    return true;
}

bool
    GeometryManager::onDestroy()
{
    return true;
}

void
    GeometryManager::_createMeshes()
{
    _CUBE_MESH_HANDLE = Procedural::BoxGenerator().setSizeX(1.0f).setSizeY(1.0f).setSizeZ(1.0f).setEnableNormals(true)
            .realizeMesh("GEOCUBEMESH")->getHandle();

}



Ogre::SceneNode*
    GeometryManager::createCube(Ogre::Vector3 size, Ogre::String name)
{
    Ogre::String cubeName("geocube_");
    cubeName += name;
    cubeName.append(Ogre::StringConverter::toString(_geoNumber++));
    
 
    Ogre::Entity* cubeEnt = _scnMgr->createEntity(Ogre::MeshManager::getSingleton().getByHandle(_CUBE_MESH_HANDLE)->getName());
    Ogre::SceneNode* node = _scnMgr->getRootSceneNode()->createChildSceneNode(cubeName);
    node->attachObject(cubeEnt);
    //Resize the cube. We're really doing a scale here, may not be correct.
    node->scale(size);
    return node;
}