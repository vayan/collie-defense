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
#include "bn_regular_bg_items_gameover.h"
#include "bn_regular_bg_items_gamewin.h"
#include "bn_regular_bg_items_launch_background.h"
#include "bn_regular_bg_ptr.h"
#include "bn_size.h"
#include "bn_sprite_builder.h"
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
        Start,
        GameOver,
        Win
    };

    class Game;
    class Menu
    {
    public:
        Menu();
        ~Menu();

        bool on_tick(Game *game, MenuScreen screen);
        void clear();

    private:
        void switch_screen(MenuScreen screen);

        bn::optional<bn::regular_bg_ptr> bg;
        MenuScreen current_screen = MenuScreen::Start;
    };
}

#endif
