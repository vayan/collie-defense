#ifndef COLLIE_DEFENCE_GBA_LEVEL_H
#define COLLIE_DEFENCE_GBA_LEVEL_H

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
#include "bn_log.h"
#include "bn_math.h"
#include "bn_size.h"
#include "bn_unique_ptr.h"

#include "enemy.h"
#include "entity.h"
#include "path.h"

namespace cd
{
    class Level
    {
    public:
        Level(
            bn::regular_bg_item _load_bg,
            const int *_int_grid,
            bn::fixed _world_x,
            bn::fixed _world_y,
            const Entity **_entities,
            bn::fixed _number_of_entities);

        ~Level();
        void init(bn::camera_ptr camera);
        void tick(bn::camera_ptr camera);

    private:
        bn::regular_bg_item load_bg;
        const int *int_grid;
        const Entity **entities;
        bn::fixed number_of_entities;
        bn::fixed_point world_position;

        bn::optional<bn::regular_bg_ptr> bg;
    };
}

#endif
