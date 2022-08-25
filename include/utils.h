#ifndef COLLIE_DEFENCE_GBA_UTILS_H
#define COLLIE_DEFENCE_GBA_UTILS_H

#include "bn_fixed_point.h"

namespace cd
{
    bn::fixed_point to_ldtk_point(bn::fixed_point point);
    bn::fixed_point from_ldtk_point(bn::fixed_point point);
}

#endif
