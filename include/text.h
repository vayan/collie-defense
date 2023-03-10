#pragma once

#include "bn_core.h"
#include "bn_sprite_font.h"
#include "bn_sprite_items_fixed_8x8_font.h"
#include "bn_sprite_items_qrcode_font.h"

namespace cd
{
    constexpr const bn::sprite_font fixed_font_8x8 = bn::sprite_font(bn::sprite_items::fixed_8x8_font);
    constexpr const bn::sprite_font fake_qrcode_font = bn::sprite_font(bn::sprite_items::qrcode_font);
}
