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
#include "bn_sprite_animate_actions.h"
#include "bn_sprite_builder.h"
#include "bn_sprite_items_sheep.h"
#include "bn_sprite_ptr.h"
#include "bn_unique_ptr.h"
#include "bn_vector.h"
#include "debug.h"
#include "math.h"
#include "target.h"

namespace cd
{
    class Game;
    class Sheep
    {
    public:
        Sheep(
            const bn::fixed _id,
            bn::camera_ptr camera,
            const bn::fixed_point origin,
            const bn::fixed_point **steps,
            const bn::fixed steps_number);

        ~Sheep();

        void on_tick();

        void set_animation_right_walk();
        void set_animation_left_walk();
        void set_animation_down_walk();
        void set_animation_up_walk();
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

    private:
        bn::fixed_point position;
        const bn::fixed_point **steps;
        const bn::fixed steps_number;
        bn::fixed progress = 0;
        bn::fixed speed = 10;
        bn::fixed current_step = 0;
    };
}
