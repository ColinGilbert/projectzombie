#ifndef _ZGAME_ENTITIES_DEFS_H
#define _ZGAME_ENTITIES_DEFS_H

#include <Ogre.h>

#define VTFINST
//#define CROWD


namespace ZGame
{
    namespace Entities
    {
        class ZEntity;

        typedef std::vector<ZEntity*> ZENTITY_VEC;
        typedef ZENTITY_VEC::iterator ZENT_ITER;
        typedef Ogre::String ZENT_KEY;
        enum ZNET_ENTITY_TYPES
        {
            ZNET_ENTITY_TYPES_BEGIN,
            PLAYER_TYPE,
            MONSTER_TYPE,
            ZNET_ENTITY_TYPES_END
        };
        static void getZNetEntityType(Ogre::String &type)
        {
            Ogre::StringStream types;
            types << PLAYER_TYPE << "TYPE";
            type = types.str();
        }

    }
}

#endif
