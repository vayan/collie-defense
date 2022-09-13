#ifndef COLLIE_DEFENCE_GBA_BALLISTA_TOWER_H
#define COLLIE_DEFENCE_GBA_BALLISTA_TOWER_H

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
#include "bn_sprite_items_bullet_arrow.h"
#include "bn_sprite_items_tower_ballista.h"
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
    class TowerBallista : public Tower
    {
    public:
        TowerBallista(bn::camera_ptr _camera, bn::fixed_point _position);
        ~TowerBallista();

        void set_animation_shoot_right();
        void set_animation_shoot_left();
        void set_animation_shoot_down();
        void set_animation_shoot_up();
    };
}

#endif
