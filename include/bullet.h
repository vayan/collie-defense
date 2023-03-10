#pragma once

#include "bn_camera_ptr.h"
#include "bn_core.h"
#include "bn_fixed.h"
#include "bn_fixed_point.h"
#include "bn_fixed_rect.h"
#include "bn_format.h"
#include "bn_log.h"
#include "bn_math.h"
#include "bn_memory.h"
#include "bn_optional.h"
#include "bn_regular_bg_item.h"
#include "bn_regular_bg_ptr.h"
#include "bn_size.h"
#include "bn_sprite_builder.h"
#include "bn_sprite_items_bullet_canon.h"
#include "bn_sprite_ptr.h"
#include "bn_timer.h"
#include "bn_unique_ptr.h"
#include "bn_vector.h"
#include "debug.h"
#include "math.h"
#include "target.h"

namespace cd
{
    class Game;
    class Level;
    class Player;
    class Bullet
    {
    public:
        Bullet(
            bn::camera_ptr camera,
            bn::fixed_point position,
            Target *target,
            bn::fixed speed_delta,
            bn::fixed damage,
            bn::sprite_item _sprite);

        ~Bullet();

        void on_tick(Game *game);
        bool to_be_destroyed();
        bn::fixed_rect get_hitbox();

    private:
        bn::camera_ptr camera;
        bn::fixed_point position;
        Target *target;
        bn::optional<bn::sprite_ptr>
            sprite;

        bool destroyed = false;
        bn::fixed delta;
        bn::fixed damage;
        bn::fixed target_id = 0;
    };
}
