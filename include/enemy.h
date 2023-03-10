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
#include "bn_random.h"
#include "bn_regular_bg_item.h"
#include "bn_regular_bg_ptr.h"
#include "bn_size.h"
#include "bn_sound_items.h"
#include "bn_sprite_animate_actions.h"
#include "bn_sprite_builder.h"
#include "bn_sprite_items_bear.h"
#include "bn_sprite_items_life_bar.h"
#include "bn_sprite_ptr.h"
#include "bn_unique_ptr.h"
#include "bn_vector.h"
#include "math.h"
#include "target.h"

namespace cd
{
    class Game;
    class Player;
    class Level;
    class Enemy : public Target
    {
    public:
        Enemy(
            const bn::fixed _id,
            bn::camera_ptr camera,
            const bn::fixed_point origin,
            const bn::fixed_point **steps,
            const bn::fixed steps_number);

        ~Enemy();

        void on_tick(Game *game);

        bool is_dead();

        bn::fixed_point get_position();
        void hit(bn::fixed dmg, Player *player);
        bn::fixed_rect get_hitbox();
        bn::fixed get_reward();
        bn::fixed get_strenght();
        bn::fixed get_id();
        bool is_active();
        virtual void set_animation_right_walk();
        virtual void set_animation_left_walk();
        virtual void set_animation_down_walk();
        virtual void set_animation_up_walk();
        void update_animation();

    private:
        const bn::fixed id;

    protected:
        bn::optional<bn::sprite_animate_action<8>>
            animation;
        bn::optional<bn::sprite_ptr>
            sprite;
        bn::camera_ptr camera;
        const bn::fixed_point origin;
        bn::fixed_point from;
        bn::fixed_point to;
        bn::fixed speed = 15;
        bn::fixed reward = 10;
        bn::fixed armor = 0;

    private:
        bn::fixed_point position;
        const bn::fixed_point **steps;
        const bn::fixed steps_number;
        bn::fixed progress = 0;
        bn::fixed current_step = 0;

        bn::optional<bn::sprite_ptr>
            life_bar;

        bn::fixed max_life = 100;
        bn::fixed life = 100;
        bool dead = false;
        bn::optional<bn::sound_item> hit_sfx;
        bn::optional<bn::sound_item> death_sfx;
    };
}
