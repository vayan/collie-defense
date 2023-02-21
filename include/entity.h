#ifndef COLLIE_DEFENCE_GBA_ENTITY_H
#define COLLIE_DEFENCE_GBA_ENTITY_H

#include "bn_fixed.h"
#include "bn_fixed_point.h"
#include "bn_format.h"
#include "bn_log.h"
#include "bn_optional.h"
#include "bn_regular_bg_item.h"
#include "bn_regular_bg_ptr.h"
#include "bn_string.h"
#include "bn_utility.h"
#include "generated/world_config.h"

namespace cd
{
    class Entity
    {
    public:
        Entity(
            const bn::fixed id,
            const EntityType _type,
            const bn::fixed x,
            const bn::fixed y,
            const bn::fixed_point **arr_points_1,
            const bn::fixed arr_points_1_size,
            const EnemyType enemy_type,
            const bn::fixed number_1,
            const bn::fixed number_2,
            const bn::fixed number_3);

        virtual ~Entity();
        bn::fixed_point get_position() const;
        EntityType get_type() const;
        bn::fixed get_number_1() const;
        bn::fixed get_number_2() const;
        bn::fixed get_number_3() const;
        const bn::fixed_point **get_arr_points_1() const;
        bn::fixed get_arr_points_1_size() const;
        bn::fixed get_id() const;
        EnemyType get_enemy_type() const;

    private:
        const bn::fixed id;
        const EntityType type;
        const bn::fixed_point **arr_points_1;
        const bn::fixed arr_points_1_size;
        const EnemyType enemy_type;
        const bn::fixed number_1;
        const bn::fixed number_2;
        const bn::fixed number_3;
        bn::fixed_point position;
    };
}

#endif
