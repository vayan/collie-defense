#pragma once

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
#include "bn_music.h"
#include "bn_music_items.h"
#include "bn_optional.h"
#include "bn_regular_bg_ptr.h"
#include "bn_size.h"
#include "bn_sound_items.h"
#include "bn_sprite_items_confirm_loader.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_text_generator.h"
#include "bn_unique_ptr.h"
#include "bn_utility.h"
#include "debug.h"
#include "player.h"
#include "qrcode.h"
#include "text.h"

namespace cd
{

    class MenuConfirm
    {
    public:
        MenuConfirm(bn::fixed_point _pos);
        ~MenuConfirm();

        bool accepted();

    private:
        bn::optional<bn::sprite_animate_action<18>> select_confirm_loading;
        bn::optional<bn::sprite_ptr> select_confirm;
    };
}
