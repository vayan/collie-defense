#ifndef COLLIE_DEFENCE_GBA_SHOP_H
#define COLLIE_DEFENCE_GBA_SHOP_H

#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_item.h"
#include "bn_affine_bg_ptr.h"
#include "bn_affine_bg_item.h"
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
#include "bn_sprite_animate_actions.h"
#include "bn_display.h"
#include "bn_fixed_rect.h"
#include "bn_blending.h"

#include "bn_regular_bg_items_tower_select_ui.h"
#include "bn_sprite_items_tower_select_ui_highlight.h"
#include "bn_regular_bg_items_overlay_bg.h"

#include "generated/world_config.h"
#include "level.h"
#include "utils.h"

namespace cd
{
    class Shop
    {
    public:
        Shop(bn::camera_ptr camera);
        ~Shop();
        void on_tick(Level *level);

        bn::optional<TowerType> get_purchase();
        bool is_closed();

    private:
        bn::camera_ptr camera;
        bn::optional<bn::regular_bg_ptr> tower_select_ui;
        bn::optional<bn::sprite_ptr>
            select_highlight;
        bn::optional<bn::regular_bg_ptr>
            overlay_bg;
        bool open = true;
        bn::optional<TowerType> purchase;
    };
}

#endif
