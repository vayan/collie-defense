#ifndef COLLIE_DEFENCE_GBA_TOWER_H
#define COLLIE_DEFENCE_GBA_TOWER_H

#include "bn_camera_ptr.h"
#include "bn_core.h"
#include "bn_fixed.h"
#include "bn_fixed_point.h"
#include "bn_fixed_rect.h"
#include "bn_format.h"
#include "bn_keypad.h"
#include "bn_log.h"
#include "bn_math.h"
#include "bn_memory.h"
#include "bn_optional.h"
#include "bn_regular_bg_item.h"
#include "bn_regular_bg_ptr.h"
#include "bn_size.h"
#include "bn_sprite_animate_actions.h"
#include "bn_sprite_builder.h"
#include "bn_sprite_items_tower_ballista.h"
#include "bn_sprite_items_tower_basic.h"
#include "bn_sprite_items_tower_magic.h"
#include "bn_sprite_ptr.h"
#include "bn_unique_ptr.h"
#include "bn_vector.h"
#include "bullet.h"
#include "debug.h"
#include "generated/world_config.h"
#include "math.h"
#include "target.h"

namespace cd
{
    class Player;
    class Level;
    class Tower
    {
    public:
        Tower(bn::camera_ptr camera, bn::fixed_point position);
        virtual ~Tower() = default;

        void on_tick(Level *level, Player *player);
        void fire(Target *target);
        bn::vector<Bullet, 2> *get_bullets();
        virtual bn::fixed_rect get_hitbox();

        bn::fixed get_aggro_range();
        bn::fixed get_cost();

        virtual void set_animation_shoot_right();
        virtual void set_animation_shoot_left();
        virtual void set_animation_shoot_down();
        virtual void set_animation_shoot_up();
        void update_animation(Target *target);

        void place();
        virtual void set_position(bn::fixed x, bn::fixed y);
        void set_position(bn::fixed_point pos);

    protected:
        bn::optional<bn::sprite_ptr>
            sprite;
        bn::optional<bn::sprite_animate_action<8>>
            animation;
        bn::fixed_point position;
        bn::fixed_point bullet_start_position_offset = bn::fixed_point(0, 0);
        bn::fixed aggro_range = 0;
        bn::fixed damage = 0;
        bn::fixed bullet_speed = 0;
        bn::fixed cost = 0;
        bn::fixed fire_rate_per_sec = 0;
        bn::optional<bn::sprite_item>
            bullet_sprite;

    private:
        bn::camera_ptr camera;

        bn::fixed frame_elapsed_since_last_fire = 0;
        bn::vector<Bullet, 2> bullets;
    };
}

#endif
