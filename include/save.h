#pragma once

#include "bn_core.h"
#include "bn_fixed.h"
#include "bn_fixed_point.h"
#include "bn_format.h"
#include "bn_list.h"
#include "bn_log.h"
#include "bn_music.h"
#include "bn_sram.h"
#include "const.h"
#include "debug.h"
#include "utils.h"
#include <string>

namespace cd
{
    const bn::fixed SAVE_STATUS_VALID_CODE = 11;
    const int MAX_LEVEL_COUNT = 30; // can switch to use cd::number_of_levels if needed

    struct save_data
    {
        bn::fixed status;

        // we need to keep the data simple to save it in the sram.
        // so forget maps, list, etc.. :)

        bn::fixed latest_story_level;
        bn::fixed story_money;
        bn::fixed story_life;
        bool music_off;
        bool sounds_off;

        bn::fixed score_per_level[MAX_LEVEL_COUNT];
    };

    inline bool global_music_off = false;
    inline bool global_sounds_off = false;

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
        const char *encode_for_qrcode();
        save_data get_data();
        void save_story_progress(int level_index, bn::fixed money, bn::fixed life);
        void save_level_score(int level_index, bn::fixed score);
        void toggle_music();
        void toggle_sounds();
    };
}
