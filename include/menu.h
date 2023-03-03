#ifndef COLLIE_DEFENCE_GBA_MENU_H
#define COLLIE_DEFENCE_GBA_MENU_H

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
#include "bn_music.h"
#include "bn_music_items.h"
#include "bn_optional.h"
#include "bn_regular_bg_item.h"
#include "bn_regular_bg_items_all_levels.h"
#include "bn_regular_bg_items_gameover.h"
#include "bn_regular_bg_items_gamewin.h"
#include "bn_regular_bg_items_launch_background.h"
#include "bn_regular_bg_items_level_select.h"
#include "bn_regular_bg_ptr.h"
#include "bn_size.h"
#include "bn_sound_items.h"
#include "bn_sprite_builder.h"
#include "bn_sprite_items_dog.h"
#include "bn_sprite_items_player_life.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_text_generator.h"
#include "bn_unique_ptr.h"
#include "bn_vector.h"
#include "debug.h"
#include "level.h"
#include "player.h"
#include "text.h"

namespace cd
{
    enum MenuScreen
    {
        Start = 0,
        GameOver = 1,
        Win = 2,
        LevelSelect = 3
    };

    class Game;
    class Menu
    {
    public:
        Menu();
        ~Menu();

        bool on_tick(Game *game);
        void clear();
        bn::fixed get_selected_level();
        void set_current_screen(MenuScreen screen);

    private:
        void switch_screen(MenuScreen screen, Game *game);
        bool handle_start_menu(Game *game);
        bool handle_level_select_menu(Game *game);
        bool handle_gameover_menu(Game *game);
        bn::string<10> human_readable_score(bn::fixed score);

        bn::optional<bn::regular_bg_ptr> select_highlight;
        bn::optional<bn::sprite_ptr>
            collie_select;
        bn::optional<bn::regular_bg_ptr> bg;
        bn::fixed selected_level = 0;
        MenuScreen selected_menu_item = MenuScreen::Start;
        MenuScreen current_screen = MenuScreen::Start;
        bn::optional<bn::sprite_text_generator> text_generator;
        bn::vector<bn::sprite_ptr, 32> text_sprites;
        bn::vector<bn::sprite_ptr, 32> text_sprites_level;
    };
}

#endif
