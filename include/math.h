#ifndef COLLIE_DEFENCE_GBA_MATH_H
#define COLLIE_DEFENCE_GBA_MATH_H

#include "bn_fixed_point.h"
#include "bn_log.h"
#include "bn_math.h"

namespace cd
{
    bn::fixed distance(bn::fixed_point from, bn::fixed_point to);
    bn::fixed lerp(bn::fixed start, bn::fixed end, bn::fixed progress);
    bn::fixed_point lerp_points(bn::fixed_point start, bn::fixed_point end, bn::fixed progress);
    const bn::fixed frame_per_sec = 60;
    bn::fixed degrees_atan2(bn::fixed_point from, bn::fixed_point to);
    const bn::fixed bad_deg = 6969;
}

#endif
