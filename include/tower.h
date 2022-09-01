#ifndef COLLIE_DEFENCE_GBA_TOWER_H
#define COLLIE_DEFENCE_GBA_TOWER_H

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
#include "bn_keypad.h"
#include "bn_fixed_rect.h"

#include "bn_sprite_items_tower_basic.h"
#include "bn_sprite_items_tower_aoe.h"
#include "bn_sprite_items_tower_sticky.h"

#include "generated/world_config.h"
#include "math.h"
#include "bullet.h"
#include "target.h"
#include "debug.h"

namespace cd
{
    class Player;
    class Level;
    class Tower
    {
    public:
        Tower(
            TowerType _type,
            bn::camera_ptr camera,
            bn::fixed_point position);

        ~Tower();

        void on_tick(Level *level, Player *player);
        void fire(Target *target);
        bn::vector<Bullet, 1> *get_bullets();
        bn::fixed_rect get_hitbox();

        static bn::sprite_item get_sprite(TowerType type);
        static bn::fixed get_aggro_range(TowerType type);
        static bn::fixed get_damage(TowerType type);
        static bn::fixed get_attack_speed(TowerType type);
        static bn::fixed get_cost(TowerType type);

        bn::fixed get_aggro_range();
        bn::fixed get_damage();
        bn::fixed get_attack_speed();
        bn::fixed get_cost();

    private:
        TowerType type;
        bn::camera_ptr camera;
        bn::fixed_point position;
        bn::optional<bn::sprite_ptr>
            sprite;
        bn::optional<bn::timer> last_fire_timer;
        bn::vector<Bullet, 1> bullets;
    };
}

#endif
