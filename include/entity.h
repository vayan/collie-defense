#ifndef COLLIE_DEFENCE_GBA_ENTITY_H
#define COLLIE_DEFENCE_GBA_ENTITY_H

#include "generated/world_config.h"
#include "bn_fixed.h"
#include "bn_fixed_point.h"
#include "bn_optional.h"
#include "bn_regular_bg_item.h"
#include "bn_regular_bg_ptr.h"
#include "bn_log.h"
#include "bn_utility.h"
#include "bn_string.h"
#include "bn_format.h"
#include "bn_optional.h"

namespace cd
{
    class Entity
    {
    public:
        Entity(
            const bn::fixed id,
            EntityType _type,
            bn::fixed x,
            bn::fixed y,
            const bn::pair<bn::string<10>, bn::fixed_point> **_coords = nullptr,
            const bn::pair<bn::string<10>, bn::string<string_metadata_max_size>> **_strings = nullptr,
            bn::fixed _coords_size = 0,
            bn::fixed _strings_size = 0);

        virtual ~Entity();

    private:
        EntityType type;
        bn::fixed_point position;
        const bn::pair<bn::string<10>, bn::fixed_point> **coords;
        const bn::pair<bn::string<10>, bn::string<string_metadata_max_size>> **strings;
        bn::fixed coords_size;
        bn::fixed strings_size;
        const bn::fixed id;
    };
}

#endif
