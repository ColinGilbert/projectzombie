/*
 * PingPongShaders.h
 * This class models the abstract concept of ping-ponging two fragment shader for the purpose of some shader based
 * simulation. The user must supply the material applied to the render to textures operation during ping-ponging.
 *  Created on: Oct 8, 2008
 *      Author: bey0nd
 */

#ifndef PINGPONGSHADERS_H_
#define PINGPONGSHADERS_H_
#include <iostream>
#include <string>
#include <memory>
#include "Ogre.h"
namespace ZGame
{

  /**
   * This class models the abstract concept of ping pong fragment shaders in order to carry out some simulation.
   * The shader takes in an set of coordinates, such that, it creates an camera at this set of coordinate, in such a way
   * that a 2D rectangle will cover it's view, and thus carrying out fragment operation which does some type of
   * simulation. It is assumed that the input coordinate are such that it is far enough away from the main scene, so that,
   * the main scene will be culled away, hence only the 2D rectangle is rendered (i.e. culls the main scene from further
   * processing).
   * It is assumed that the input render to texture DOES NOT currently have a view port assigned to it.
   */
  class PingPongShaders
  {
  public:
    PingPongShaders(Ogre::TexturePtr tex, Ogre::MaterialPtr mat,
		const Ogre::Real camZDepth);
    virtual
    ~PingPongShaders();
    //This method does the ping pong.
    void
    pingPong();
  protected:
	Ogre::TexturePtr _input;
	Ogre::TexturePtr _output;
	Ogre::MaterialPtr _mat;
	Ogre::Real _camZDepth; //the z depth where we place our camera
	static const Ogre::Real _NEARPLANE;// = 1.0;
	static const Ogre::Real _FARPLANE;// = 2.0;
	static Ogre::uint32 _ID;
	Ogre::uint32 _curId;
	std::string _pingPongName;
	Ogre::Camera* _cam;
	std::auto_ptr<Ogre::Rectangle2D> _fsQuad;
	Ogre::Vector3 _camCoords;

    void
    init();

    void
	setupRTT(Ogre::TexturePtr &mat, Ogre::Camera* cam);

    void
    duplicateInput();
  };

}

#endif /* PINGPONGSHADERS_H_ */
