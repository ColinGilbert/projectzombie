#include "toolset/CubeToolXForm.h"

using std::cout; using std::endl;
using namespace ZGame;
using namespace ZGame::Toolset;

CubeToolXForm::CubeToolXForm()
{
}

CubeToolXForm::~CubeToolXForm()
{
}
/**
* \note This method assuems that cursorPos are in integral CUBE units (i.e. on integers).
*
**/
ToolXForm&
    CubeToolXForm::scaleApply(Ogre::SceneNode* node, 
    const Ogre::Vector3 &anchorPos,
    const Ogre::Vector3 &cursorPos)
{
    using Ogre::Vector3;
    using Ogre::Math;
    using Ogre::Real;
    
    Vector3 leftMostExtent = anchorPos;
    Vector3 rightMostExtent = cursorPos;
         
    //Now that left and right extents are set we can compute the scale factor.
    //Here we assume that the positions are defined to be CUBE centers. And also that
    //the unit we're working with are unit cubes--a cube of size one. 
    //We also assume that the unit cube's origin is at it's center.
    Vector3 span = rightMostExtent - leftMostExtent;
    Real scaleX = Math::Abs(span.x);
    Real scaleY  = Math::Abs(span.y);
    Real scaleZ = Math::Abs(span.z);
    
    Ogre::Vector3 scale(scaleX + 1.0f, scaleY + 1.0f, scaleZ + 1.0f); //Plus one because we are working in 
    //number of cubes as a unit. So one cube is a unit.
    node->setScale(scale); //scale by number of cubes.0.5 *
    node->setPosition(anchorPos + (span * 0.5));
    return *this;
}

ToolXForm&
    CubeToolXForm::translateApply(Ogre::SceneNode* node, 
    const Ogre::Vector3 &anchorPos,
    const Ogre::Vector3 &cursorPos)
{
    return *this;
}

ToolXForm&
    CubeToolXForm::setLowerLeft(Ogre::SceneNode* node, 
    const Ogre::Vector3 &anchorPos,
    const Ogre::Vector3 &cursorPos)
{
    return *this;
}

ToolXForm&
    CubeToolXForm::setUpperRight(Ogre::SceneNode* node, 
    const Ogre::Vector3 &anchorPos,
    const Ogre::Vector3 &cursorPos)
{
    return *this;
}


