/*
* ImposterView.h
* This class provides a view for an imposter.
*  Created on: Sep 11, 2008
*      Author: bey0nd
*/

#ifndef IMPOSTERVIEW_H_
#define IMPOSTERVIEW_H_
//#include <string>
using namespace std;
#include "Ogre.h"

namespace ZGame
{
    class Imposter;
    class ImposterView
    {
    public:
        ImposterView();
        ~ImposterView();

        void setInput(Imposter* input);

        //test functions for iterating through the different textures
        void flip(); //flip to the next texture

    protected:
        Imposter* _imposter;
        Ogre::SceneNode* _texNode;
        Ogre::Entity* _texEnt;
        //used for testing
        size_t _curFlipTheta; //current index to the theta texture in the 'flip' book
        size_t _curFlipPhi; //current index to the phi texture in the flip book.

        void setupView();
        void switchTexture();
        void attachMaterial();

        Ogre::MaterialPtr getMaterial();
    private:

        const Ogre::String _TYPENAME;
        const Ogre::String _MATERIALNAME;


    };
}

#endif /* IMPOSTERVIEW_H_ */
