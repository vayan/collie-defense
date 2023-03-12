#pragma once

#include "bn_affine_bg_item.h"
#include "bn_affine_bg_ptr.h"
#include "bn_blending.h"
#include "bn_camera_ptr.h"
#include "bn_core.h"
#include "bn_display.h"
#include "bn_fixed.h"
#include "bn_fixed_point.h"
#include "bn_fixed_rect.h"
#include "bn_format.h"
#include "bn_log.h"
#include "bn_math.h"
#include "bn_memory.h"
#include "bn_optional.h"
#include "bn_regular_bg_item.h"
#include "bn_regular_bg_items_overlay_bg.h"
#include "bn_regular_bg_items_store.h"
#include "bn_regular_bg_items_store_select.h"
#include "bn_regular_bg_ptr.h"
#include "bn_size.h"
#include "bn_sound_items.h"
#include "bn_sprite_animate_actions.h"
#include "bn_sprite_builder.h"
#include "bn_sprite_items_bear.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_text_generator.h"
#include "bn_timer.h"
#include "bn_unique_ptr.h"
#include "bn_utility.h"
#include "bn_vector.h"
#include "generated/world_config.h"
#include "level.h"
#include "text.h"
#include "tower_fire.h"
#include "utils.h"

namespace cd
{
    class Game;
    class Shop
    {
    public:
        Shop(bn::camera_ptr camera, bn::vector<TowerType, 4> available_towers);
        ~Shop();
        void on_tick(Game *game);

        bn::optional<TowerType> get_purchase();
        bool is_closed();
        void refresh_money(Player *player);

    private:
        void display_level_count(bn::fixed current_level_index, bn::fixed total_level_count);
        void display_tower_info();

        bn::camera_ptr camera;
        bn::optional<bn::regular_bg_ptr> store;
        bn::optional<bn::regular_bg_ptr>
            select_highlight;
        bn::optional<bn::regular_bg_ptr>
            overlay_bg;
        bool open = true;
        bn::optional<TowerType> purchase;
        bn::vector<Tower, 4> elements;
        int current_element = 0;
        bn::optional<bn::sprite_text_generator> text_generator;
        bn::vector<bn::sprite_ptr, 32> text_sprites;
        bn::vector<bn::sprite_ptr, 32> text_sprites_level;
        bn::vector<bn::sprite_ptr, 32> text_sprites_tower;
    };
}
