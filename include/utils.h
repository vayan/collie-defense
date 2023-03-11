#pragma once

#include "bn_fixed_point.h"
#include "bn_format.h"
#include "bn_string.h"
#include "debug.h"
#include <string>

namespace cd
{
    bn::fixed_point to_ldtk_point(bn::fixed_point point);
    bn::fixed_point from_ldtk_point(bn::fixed_point point);
    std::string convertToBase36(int num);
    constexpr const char base36_digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
}
