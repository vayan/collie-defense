#pragma once

#include "bn_camera_ptr.h"
#include "bn_core.h"
#include "bn_fixed.h"
#include "bn_fixed_point.h"
#include "bn_format.h"
#include "bn_log.h"
#include "bn_math.h"
#include "bn_memory.h"
#include "bn_optional.h"
#include "bn_regular_bg_item.h"
#include "bn_regular_bg_ptr.h"
#include "bn_size.h"
#include "bn_sprite_builder.h"
#include "bn_sprite_items_bear.h"
#include "bn_sprite_ptr.h"
#include "bn_timer.h"
#include "bn_unique_ptr.h"
#include "bn_vector.h"
#include "debug.h"
#include "enemy.h"
#include "enemy_bear.h"
#include "enemy_fox.h"
#include "enemy_tiger.h"
#include "entity.h"
#include "math.h"

namespace cd
{
    class Game;
    class Level;
    class Wave
    {
    public:
        Wave(
            const bn::fixed _id,
            const bn::fixed_point _from,
            bn::camera_ptr _camera,
            const bn::fixed fire_pause_sec,
            const bn::fixed_point **steps,
            const bn::fixed steps_number,
            const bn::fixed wave_order,
            const bn::fixed wave_duration_sec,
            const EnemyType _enemy_type);
        ~Wave();

        void on_tick(Game *game);

        bn::vector<Enemy *, 20> *get_enemies();

        const bn::fixed get_wave_order();
        bool to_be_removed();
        bn::fixed generate_enemy_id();

    private:
        const bn::fixed id;
        bn::fixed_point position;
        bn::fixed_point from;
        bn::fixed_point to;
        bn::camera_ptr camera;
        const bn::fixed fire_pause_sec;
        const bn::fixed_point **steps;
        const bn::fixed steps_number;
        const bn::fixed wave_order;
        const bn::fixed wave_duration_sec;

        bn::optional<bn::sprite_ptr>
            sprite;
        bn::fixed progress = 0;
        bn::fixed delta = 0.02;
        bn::fixed current_step = 0;
        bn::fixed wave_ = 0;
        bn::fixed frame_elapsed_since_last_fire = 0;
        bn::fixed frame_elapsed_since_create = 0;
        bn::optional<bn::random>
            rand;

        bool is_finished = false;

        bn::vector<Enemy *, 20> enemies;
        EnemyType enemy_type;
    };
}
