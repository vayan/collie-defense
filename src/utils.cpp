#include "utils.h"

using namespace cd;

bn::fixed_point cd::to_ldtk_point(bn::fixed_point point)
{
    return bn::fixed_point(point.x() + (256 / 2), point.y() + (256 / 2));
}

bn::fixed_point cd::from_ldtk_point(bn::fixed_point point)
{
    return bn::fixed_point(point.x() - (256 / 2), point.y() - (256 / 2));
}

std::string cd::convertToBase36(int num)
{
    std::string base36 = "";
    if (num == 0)
    {
        return "0";
    }
    while (num > 0)
    {
        int remainder = num % 36;
        base36 += base36_digits[remainder];
        num /= 36;
    }
    reverse(base36.begin(), base36.end());
    return base36;
}

void cd::play_music(bn::music_item music, bn::fixed volume)
{
    if (global_music_off)
    {
        log("music disabled, cannot play it");
        return;
    }
    music.play(volume);
}

void cd::play_sfx(bn::sound_item sfx, bn::fixed volume)
{
    if (global_sounds_off)
    {
        log("sfx disabled, cannot play it");
        return;
    }
    sfx.play(volume);
}
