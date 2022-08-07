#include "math.h"

using namespace cd;

bn::fixed cd::lerp(bn::fixed start, bn::fixed end, bn::fixed progress)
{
    return start * (1 - progress) + end * progress;
}

bn::fixed_point cd::lerp_points(bn::fixed_point start, bn::fixed_point end, bn::fixed progress)
{
    return bn::fixed_point(lerp(start.x(), end.x(), progress), lerp(start.y(), end.y(), progress));
}
