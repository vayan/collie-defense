#ifndef COLLIE_DEFENCE_SAVE_H
#define COLLIE_DEFENCE_SAVE_H

#include "bn_core.h"
#include "bn_fixed.h"
#include "bn_fixed_point.h"
#include "bn_format.h"
#include "bn_list.h"
#include "bn_log.h"
#include "bn_sram.h"
#include "debug.h"

namespace cd
{
    const bn::fixed SAVE_STATUS_VALID_CODE = 11;

    struct save_data
    {
        bn::fixed status;

        // we need to keep the data simple to save it in the sram.
        // so forget maps, list, etc.. :)
        bn::fixed latest_level;
    };

    class Save
    {
    private:
        save_data cart_save_data;

    public:
        Save();
        ~Save();
        void read();
        void write();
        void reset();
        save_data get_data();
    };
}

#endif
