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
            bn::fixed_point **arr_points_1,
            bn::fixed arr_points_1_size,
            bn::string<10> string_10_1,
            bn::fixed number_1);

        virtual ~Entity();
        bn::fixed_point get_position() const;
        EntityType get_type() const;
        bn::fixed get_number_1() const;
        bn::fixed_point **get_arr_points_1() const;
        bn::fixed get_arr_points_1_size() const;

    private:
        const bn::fixed id;
        EntityType type;
        bn::fixed_point **arr_points_1;
        bn::fixed arr_points_1_size;
        bn::fixed number_1;
        bn::string<10> string_10_1;
        bn::fixed_point position;
    };
}

#endif
