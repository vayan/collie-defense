#ifndef COLLIE_DEFENCE_GBA_HUD_H
#define COLLIE_DEFENCE_GBA_HUD_H

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
#include "bn_sprite_builder.h"
#include "bn_sprite_items_player_life.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_text_generator.h"
#include "bn_unique_ptr.h"
#include "bn_vector.h"
#include "debug.h"
#include "text.h"

namespace cd
{
    class Game;
    class HUD
    {
    public:
        HUD();
        ~HUD();

        void on_tick(Game *game);

        bool is_paused();

    private:
        bn::optional<bn::sprite_ptr>
            player_life_sprite;
        bn::optional<bn::sprite_text_generator> text_generator;
        bn::vector<bn::sprite_ptr, 32> text_sprites;

        bool paused = false;
    };
}

#endif
