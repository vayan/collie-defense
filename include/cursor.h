#ifndef COLLIE_DEFENCE_GBA_CURSOR_H
#define COLLIE_DEFENCE_GBA_CURSOR_H

#include "bn_affine_bg_item.h"
#include "bn_affine_bg_items_range.h"
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
#include "bn_regular_bg_ptr.h"
#include "bn_size.h"
#include "bn_sprite_animate_actions.h"
#include "bn_sprite_builder.h"
#include "bn_sprite_items_bear.h"
#include "bn_sprite_items_cursor.h"
#include "bn_sprite_ptr.h"
#include "bn_timer.h"
#include "bn_unique_ptr.h"
#include "bn_vector.h"
#include "generated/world_config.h"
#include "level.h"
#include "shop.h"
#include "utils.h"

namespace cd
{
    class Game;
    class Cursor
    {
    public:
        Cursor(bn::camera_ptr camera);
        ~Cursor();
        void on_tick(Game *Game);
        void enable();
        void disable();
        void update_camera(bn::regular_bg_ptr map);
        bn::fixed_rect get_hitbox();

        void remove_current_selection(bool hard_clean);
        void set_selection(TowerType type);
        void hide_shop();
        void show_shop();
        void check_screen_bounds();
        void hide();
        void show();

    private:
        bool can_build(Level *level);
        bn::optional<bn::sprite_ptr>
            sprite;
        bool targeting_buildable_grid = false;
        bn::fixed_point position;
        bn::camera_ptr camera;
        bn::optional<bn::affine_bg_ptr> range;
        bn::optional<Tower *> current_selection;
        bn::optional<Shop>
            shop;
    };
}

#endif
