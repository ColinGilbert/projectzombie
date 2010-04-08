#ifndef _ZGAME_ENTITIES_DEFS_H
#define _ZGAME_ENTITIES_DEFS_H




namespace ZGame
{
    namespace Entities
    {
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