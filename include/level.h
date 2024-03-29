#pragma once

#include "bn_camera_ptr.h"
#include "bn_core.h"
#include "bn_fixed.h"
#include "bn_fixed_point.h"
#include "bn_format.h"
#include "bn_log.h"
#include "bn_math.h"
#include "bn_memory.h"
#include "bn_music.h"
#include "bn_music_items.h"
#include "bn_optional.h"
#include "bn_regular_bg_item.h"
#include "bn_regular_bg_ptr.h"
#include "bn_size.h"
#include "bn_unique_ptr.h"
#include "bn_vector.h"
#include "debug.h"
#include "enemy.h"
#include "entity.h"
#include "hud.h"
#include "sheep.h"
#include "tower.h"
#include "tower_ballista.h"
#include "tower_basic.h"
#include "tower_magic.h"
#include "wave.h"

namespace cd
{
    class Player;
    class Game;
    class Level
    {
    public:
        Level(
            bn::regular_bg_item _load_bg,
            const int *_int_grid,
            const Entity **_entities,
            const bn::fixed _number_of_entities,
            bn::optional<bn::music_item>
                _music,
            const bn::fixed start_money,
            const TowerType *_available_towers,
            const int _available_towers_number);

        ~Level();
        void init(bn::camera_ptr camera);
        void tick(Game *game);
        bn::vector<Wave *, 10> *get_waves();
        bn::vector<Tower *, 20> *get_towers();
        bn::optional<bn::regular_bg_ptr> get_bg();
        GridTileType get_map_cell(bn::fixed x, bn::fixed y);
        bool add_tower(Tower *tower);
        bool is_won();
        bool is_lost();
        void reset();
        void clear_towers();
        void clear_sheeps();
        void clear_waves();
        void stop_music();
        void clear_bullets_with_invalid_target();
        bn::fixed get_current_wave();
        bn::fixed get_number_of_waves();
        const bn::fixed get_start_money();
        Level *copy() const;
        bn::vector<TowerType, 4> get_available_towers();
        HUD *get_hud();

    private:
        GridTileType
        get_tile_type(int tile_index);

        bn::regular_bg_item load_bg;
        const int *int_grid;
        const Entity **entities;
        const bn::fixed number_of_entities;
        bn::optional<bn::music_item> music;
        const bn::fixed start_money;
        const TowerType *available_towers;
        const int available_towers_number;

        bn::optional<bn::regular_bg_ptr>
            bg;
        bn::optional<HUD *> hud;
        bn::vector<Wave *, 10> waves;
        bn::vector<Sheep *, 10> sheeps;
        bn::vector<Tower *, 20> towers;
        bn::fixed current_wave = 0;
        bn::fixed number_of_waves = 0;

        bool all_waves_finished = false;
    };
}
