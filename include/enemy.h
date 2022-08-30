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
#include "bn_fixed_rect.h"

#include "bn_sprite_items_sheep.h"
#include "bn_sprite_items_life_bar.h"

#include "math.h"
#include "target.h"

namespace cd
{
    class Player;
    class Level;
    class Enemy : public Target
    {
    public:
        Enemy(
            bn::camera_ptr camera,
            bn::fixed_point origin,
            bn::fixed_point **steps,
            bn::fixed steps_number);

        ~Enemy();

        void on_tick(Level *level, Player *player);

        bool is_dead();

        bn::fixed_point get_position();
        void hit(bn::fixed dmg);
        bn::fixed_rect get_hitbox();
        bn::fixed get_reward();
        bn::fixed get_strenght();

    private:
        bn::fixed_point position;
        bn::camera_ptr camera;
        bn::fixed_point origin;
        bn::fixed_point **steps;
        bn::fixed steps_number;
        bn::fixed progress = 0;
        bn::fixed delta = 0.01;
        bn::fixed current_step = 0;
        bn::optional<bn::sprite_ptr>
            sprite;
        bn::optional<bn::sprite_ptr>
            life_bar;
        bn::fixed_point from;
        bn::fixed_point to;
        bn::fixed max_life = 100;
        bn::fixed life = 100;
        bool dead = false;
    };
}

#endif
