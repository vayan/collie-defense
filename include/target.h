#ifndef COLLIE_DEFENCE_GBA_TARGET_H
#define COLLIE_DEFENCE_GBA_TARGET_H

#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_item.h"
#include "bn_sprite_builder.h"
#include "bn_fixed.h"
#include "bn_optional.h"
#include "bn_core.h"
#include "bn_fixed_point.h"
#include "bn_camera_ptr.h"
#include "bn_vector.h"
#include "bn_memory.h"
#include "bn_format.h"
#include "bn_log.h"
#include "bn_math.h"
#include "bn_size.h"
#include "bn_unique_ptr.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_items_sheep.h"
#include "bn_timer.h"

namespace cd
{
    class Target
    {
    public:
        virtual bn::fixed_point get_position();
        virtual bn::fixed_rect get_hitbox();
        virtual bn::fixed get_reward();

        virtual void hit();
        virtual ~Target() = default;
    };
}

#endif
