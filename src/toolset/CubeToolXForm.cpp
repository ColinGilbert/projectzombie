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
    cout << "Computing CubeToolXForm: scaleApply" << endl; //Goddamnit start using Logging again.
    using Ogre::Plane;
    using Ogre::Vector3;
    //A cursor's position relative to the node's position,
    //signifies which quadrant the cursor belongs to. I.E:
    //I quadrant => UpperRight. II quadrant => UpperLeft. III quadrant => LowerLeft. IV => LowerRight
    Plane xFacing(Ogre::Vector3::UNIT_X, anchorPos);
    Plane zFacing(Ogre::Vector3::UNIT_Z, anchorPos);
    Plane yFacing(Ogre::Vector3::UNIT_Y, anchorPos);

    //determine quadrant
    Vector3 leftMostExtent, rightMostExtent;
 
    Plane::Side xf = xFacing.getSide(cursorPos);
    Plane::Side zf = zFacing.getSide(cursorPos);
    Plane::Side yf = yFacing.getSide(cursorPos);

    if(xf == Plane::POSITIVE_SIDE && zf == Plane::NEGATIVE_SIDE)
    {
        leftMostExtent = anchorPos;
        rightMostExtent = cursorPos;
    }
    else if(xf == Plane::NEGATIVE_SIDE && zf == Plane::NEGATIVE_SIDE)
    {
        leftMostExtent = cursorPos;
        leftMostExtent.z = anchorPos.z;
        rightMostExtent = anchorPos;
        rightMostExtent.z = cursorPos.z;
    }
    else if(xf == Plane::POSITIVE_SIDE && zf == Plane::POSITIVE_SIDE)
    {
        leftMostExtent = anchorPos;
        leftMostExtent.z = cursorPos.z;
        rightMostExtent = cursorPos;
        rightMostExtent = anchorPos.z;
    }
    else if(xf == Plane::NEGATIVE_SIDE && zf == Plane::POSITIVE_SIDE)
    {
        leftMostExtent = cursorPos;
        rightMostExtent = anchorPos;
    }
    /*
    else
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, "Invalid cursorPosition. Cannot find a side which the cursor belongs to.", 
            "CubeToolXForm::scaleApply");
    }*/
    //Set the Ys
    if(yf == Plane::POSITIVE_SIDE)
    {
        leftMostExtent.y = anchorPos.y;
        rightMostExtent.y = cursorPos.y;
    }
    else
    {
        leftMostExtent.y = cursorPos.y;
        rightMostExtent.y = anchorPos.y;
    }
  
    //Now that left and right extents are set, we can compute and apply the scale on the given scene node.
    //Assuming that the given coordinates are in CUBE coordinates: Then the scale is simple the distance 
    //along an axis of a given cube as defined by leftMost and rightMost extents.
    Ogre::Real scaleX = rightMostExtent.x - leftMostExtent.x;
    Ogre::Real scaleY = rightMostExtent.y - leftMostExtent.y;
    Ogre::Real scaleZ = rightMostExtent.z - leftMostExtent.z;
    
    cout << "scale x, y, z" << scaleX << ", " << scaleY << ", " << scaleZ << endl;

    //if(scaleX < 1.0f || scaleY < 1.0f || scaleZ < 1.0f)
    //{
      //  return *this; //do nothing because this means cursor is on anchor.
    //}
    //translate to left most extent then scale.
    node->scale(1.0f, 1.0f, 1.0f); //return to unit scale. Maye this can be eliminated by figure out offset scales.
    node->setScale(scaleX, scaleY, scaleZ);
    node->setPosition(leftMostExtent);
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


