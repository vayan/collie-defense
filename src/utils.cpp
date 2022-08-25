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
