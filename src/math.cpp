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

bn::fixed_point cd::move_to(bn::fixed_point start, bn::fixed_point end, bn::fixed delta)
{
    bn::fixed delta_x = end.x() - start.x();
    bn::fixed delta_y = end.y() - start.y();
    bn::fixed dist = sqrt(delta_x * delta_x + delta_y * delta_y);

    if (dist == 0)
    {
        return start;
    }

    start.set_x(start.x() + (delta_x.safe_multiplication(delta.safe_division(dist))));
    start.set_y(start.y() + (delta_y.safe_multiplication(delta.safe_division(dist))));

    return start;
}

bn::fixed cd::to_360_angle(bn::fixed deg)
{
    if (deg < 0)
    {
        deg += 360;
    }

    return deg;
}

bn::fixed cd::distance(bn::fixed_point from, bn::fixed_point to)
{
    bn::fixed delta_x = from.x() - to.x();
    bn::fixed delta_y = from.y() - to.y();
    return sqrt(delta_x * delta_x + delta_y * delta_y);
}

bn::fixed cd::degrees_atan2(bn::fixed_point from, bn::fixed_point to)
{
    int delta_y = (from.y() - to.y()).integer();

    if (delta_y < -32767 || delta_y > 32767)
    {
        return bad_deg;
    }

    int delta_x = (from.x() - to.x()).integer();

    if (delta_x < -32767 || delta_x > 32767)
    {
        return bad_deg;
    }

    return bn::degrees_atan2(delta_y, delta_x);
}
