#pragma once

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
#include "bn_regular_bg_items_pause.h"
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
#include "menu.h"
#include "player.h"
#include "save.h"
#include "text.h"

namespace cd
{
    enum GameMode
    {
        Story,
        Single
    };

    class Game
    {
    public:
        Game(Menu *_menu, Save *_save);
        ~Game();
        int start_main_loop();
        void start_level(int level_index);
        MenuScreen start_level_loop();
        void start_menu_screen_loop();

        Player *get_player();
        Level *get_current_level();
        int get_current_level_index();

        bn::camera_ptr get_camera();
        void toggle_pause();
        GameMode get_game_mode();
        void set_game_mode(GameMode mode);
        Save *get_save();
        void reset_current_level();

    private:
        void stop_pause();
        bool is_paused = false;
        bn::optional<bn::regular_bg_ptr> pause_bg;
        bn::optional<bn::regular_bg_ptr> overlay_bg;

        int current_level_index = 0;
        bn::optional<bn::camera_ptr> camera;
        bn::optional<Level *> current_level;
        bn::optional<Player *> player;
        Menu *menu;
        Save *save;
        GameMode game_mode = GameMode::Story;
        bn::optional<bn::regular_bg_ptr> menu_select;
        int current_selection_index = 0;
        bn::vector<bn::pair<MenuScreen, bn::fixed_point>, 3> menu_elements;
    };

}
