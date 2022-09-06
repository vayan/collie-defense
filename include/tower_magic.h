#ifndef COLLIE_DEFENCE_GBA_MAGIC_TOWER_H
#define COLLIE_DEFENCE_GBA_MAGIC_TOWER_H

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
#include "bn_sprite_items_tower_magic.h"
#include "bn_sprite_ptr.h"
#include "bn_unique_ptr.h"
#include "bn_vector.h"
#include "bullet.h"
#include "debug.h"
#include "generated/world_config.h"
#include "math.h"
#include "target.h"
#include "tower.h"

namespace cd
{
    class TowerMagic : public Tower
    {
    public:
        TowerMagic(bn::camera_ptr _camera, bn::fixed_point _position);
        ~TowerMagic();

        bn::fixed_rect get_hitbox();
        void set_animation_shoot_right();
        void set_animation_shoot_left();
        void set_animation_shoot_down();
        void set_animation_shoot_up();
        void set_position(bn::fixed x, bn::fixed y);
    };
}

#endif
