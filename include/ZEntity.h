/*
* ZEntity.h
*
*  Created on: Sep 18, 2008
*      Author: bey0nd
*/

#ifndef ZENTITY_H_
#define ZENTITY_H_

//#include <string>

//using namespace std;
#include <Ogre.h>
namespace ZGame
{
    class ZEntity
    {
    public:
        ZEntity(const Ogre::String &entName,const Ogre::String &meshName);
        ~ZEntity();

        const Ogre::String getEntityName() {return _entName;}
        const Ogre::String getMeshName() {return _meshName;}

    protected:
    private:
        Ogre::String _entName;
        Ogre::String _meshName;

    };
}

#endif /* ZENTITY_H_ */
