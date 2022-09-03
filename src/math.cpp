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

bn::fixed cd::distance(bn::fixed_point from, bn::fixed_point to)
{
    bn::fixed delta_x = from.x() - to.x();
    bn::fixed delta_y = from.y() - to.y();
    return sqrt(delta_x * delta_x + delta_y * delta_y);
}
