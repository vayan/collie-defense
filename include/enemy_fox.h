#ifndef COLLIE_DEFENCE_GBA_ENEMY_FOX_H
#define COLLIE_DEFENCE_GBA_ENEMY_FOX_H

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
#include "bn_sprite_items_fox.h"
#include "bn_sprite_items_life_bar.h"
#include "bn_sprite_ptr.h"
#include "bn_unique_ptr.h"
#include "bn_vector.h"
#include "enemy.h"
#include "math.h"

namespace cd
{
    class EnemyFox : public Enemy
    {
    public:
        EnemyFox(
            const bn::fixed _id,
            bn::camera_ptr _camera,
            const bn::fixed_point _origin,
            const bn::fixed_point **_steps,
            const bn::fixed _steps_number);
        ~EnemyFox();

        virtual void set_animation_right_walk();
        virtual void set_animation_left_walk();
        virtual void set_animation_down_walk();
        virtual void set_animation_up_walk();
    };
}

#endif
