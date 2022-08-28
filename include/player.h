#ifndef COLLIE_DEFENCE_GBA_PLAYER_H
#define COLLIE_DEFENCE_GBA_PLAYER_H

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

#include "level.h"
#include "cursor.h"
#include "debug.h"

namespace cd
{
    class Player
    {
    public:
        Player(bn::camera_ptr _camera);
        ~Player();
        void on_tick(Level *level);
        void on_target_killed(Target *target);
        void on_hit(Enemy *enemy);

        bn::fixed get_life();
        bn::fixed get_money();

    private:
        bn::optional<Cursor>
            cursor;
        bn::fixed life = 100;
        bn::fixed money = 0;
    };
}

#endif
