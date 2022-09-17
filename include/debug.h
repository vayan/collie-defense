#ifndef COLLIE_DEFENCE_GBA_DEBUG_H
#define COLLIE_DEFENCE_GBA_DEBUG_H

#include "bn_camera_ptr.h"
#include "bn_core.h"
#include "bn_fixed.h"
#include "bn_fixed_point.h"
#include "bn_format.h"
#include "bn_log.h"
#include "bn_math.h"
#include "bn_memory.h"
#include "bn_optional.h"
#include "bn_regular_bg_item.h"
#include "bn_regular_bg_ptr.h"
#include "bn_size.h"
#include "bn_sprite_builder.h"
#include "bn_sprite_ptr.h"
#include "bn_timer.h"
#include "bn_unique_ptr.h"
#include "bn_vector.h"

namespace cd
{
    void log(bn::string<50> label, bn::fixed number);
    void log(bn::string<20> label, bn::fixed_point point);
    void log(bn::string<50> message);
    void display_memory_left();
}

#endif
