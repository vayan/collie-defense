#pragma once

#include "bn_fixed_point.h"
#include "bn_format.h"
#include "bn_music_item.h"
#include "bn_sound_item.h"
#include "bn_string.h"
#include "debug.h"
#include "save.h"
#include <string>

namespace cd
{
    bn::fixed_point to_ldtk_point(bn::fixed_point point);
    bn::fixed_point from_ldtk_point(bn::fixed_point point);
    std::string convertToBase36(int num);
    void play_music(bn::music_item music, bn::fixed volume);
    void play_sfx(bn::sound_item sfx, bn::fixed volume);
    constexpr const char base36_digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
}
