#ifndef COLLIE_DEFENCE_GBA_ENEMY_H
#define COLLIE_DEFENCE_GBA_ENEMY_H

#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_item.h"
#include "bn_fixed.h"
#include "bn_optional.h"
#include "bn_core.h"
#include "bn_fixed_point.h"
#include "bn_camera_ptr.h"
#include "bn_vector.h"
#include "bn_memory.h"
#include "bn_format.h"
#include "bn_vector.h"
#include "bn_log.h"
#include "bn_math.h"
#include "bn_size.h"
#include "bn_unique_ptr.h"
#include "bn_sprite_builder.h"
#include "bn_sprite_ptr.h"

#include "bn_sprite_items_sheep.h"

#include "math.h"

namespace cd
{
    class Enemy
    {
    public:
        Enemy(
            bn::camera_ptr camera,
            bn::fixed_point origin,
            bn::fixed_point **steps,
            bn::fixed steps_number);

        ~Enemy();

        void on_tick();

        bool is_dead();

    private:
        bn::fixed_point position;
        bn::camera_ptr camera;
        bn::fixed_point origin;
        bn::fixed_point **steps;
        bn::fixed steps_number;
        bn::fixed progress = 0;
        bn::fixed delta = 0.02;
        bn::fixed current_step = 0;
        bn::optional<bn::sprite_ptr>
            sprite;
        bn::fixed_point from;
        bn::fixed_point to;
        bool dead = false;
    };
}

#endif
