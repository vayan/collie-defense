#ifndef COLLIE_DEFENCE_GBA_GAME_H
#define COLLIE_DEFENCE_GBA_GAME_H

#include "bn_camera_ptr.h"
#include "bn_core.h"
#include "bn_fixed.h"
#include "bn_fixed_point.h"
#include "bn_fixed_rect.h"
#include "bn_format.h"
#include "bn_keypad.h"
#include "bn_log.h"
#include "bn_math.h"
#include "bn_memory.h"
#include "bn_optional.h"
#include "bn_regular_bg_item.h"
#include "bn_regular_bg_ptr.h"
#include "bn_size.h"
#include "bn_sprite_builder.h"
#include "bn_sprite_items_player_life.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_text_generator.h"
#include "bn_unique_ptr.h"
#include "bn_vector.h"
#include "debug.h"
#include "generated/levels_intgrid.h"
#include "generated/world_config.h"
#include "level.h"
#include "player.h"
#include "text.h"
#include "ui.h"

namespace cd
{
    class Game
    {
    public:
        Game();
        ~Game();
        int start_main_loop();
        void start_level(int level_index);
        void start_level_loop();
        UI *get_ui();
        Player *get_player();
        Level *get_current_level();
        bn::camera_ptr get_camera();

    private:
        int current_level_index = -1;
        bn::optional<bn::camera_ptr> camera;
        Level *current_level;
        Player *player;
        UI *ui;
    };

}

#endif
