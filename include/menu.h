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
#include "bn_music.h"
#include "bn_music_items.h"
#include "bn_optional.h"
#include "bn_regular_bg_item.h"
#include "bn_regular_bg_items_all_levels.h"
#include "bn_regular_bg_items_gameover.h"
#include "bn_regular_bg_items_gamewin.h"
#include "bn_regular_bg_items_level_select.h"
#include "bn_regular_bg_items_level_win_bg.h"
#include "bn_regular_bg_items_menu_config.h"
#include "bn_regular_bg_items_menu_credit.h"
#include "bn_regular_bg_items_menu_playmode.h"
#include "bn_regular_bg_items_menu_qrcode.h"
#include "bn_regular_bg_items_menu_select_item_highlight.h"
#include "bn_regular_bg_items_menu_title.h"
#include "bn_regular_bg_items_menu_title_screen.h"
#include "bn_regular_bg_ptr.h"
#include "bn_size.h"
#include "bn_sound_items.h"
#include "bn_sprite_builder.h"
#include "bn_sprite_items_checkbox_button.h"
#include "bn_sprite_items_collie_title.h"
#include "bn_sprite_items_confirm_loader.h"
#include "bn_sprite_items_dancing_collie.h"
#include "bn_sprite_items_dancing_sheep_0.h"
#include "bn_sprite_items_dancing_sheep_1.h"
#include "bn_sprite_items_dancing_sheep_2.h"
#include "bn_sprite_items_dancing_sheep_3.h"
#include "bn_sprite_items_dog.h"
#include "bn_sprite_items_player_life.h"
#include "bn_sprite_items_victory_banner_left.h"
#include "bn_sprite_items_victory_banner_middle.h"
#include "bn_sprite_items_victory_banner_right.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_text_generator.h"
#include "bn_unique_ptr.h"
#include "bn_utility.h"
#include "bn_vector.h"
#include "debug.h"
#include "level.h"
#include "menu_confirm.h"
#include "player.h"
#include "qrcode.h"
#include "text.h"

namespace cd
{
    enum MenuScreen
    {
        Start = 0,
        GameOver = 1,
        Win = 2,
        LevelSelect = 3,
        Share = 4,
        Play = 5,
        Config = 6,
        StoryItem = 7,
        LevelWin = 8,
        Title = 9,
        Restart = 10,
        Cancel = 11,
        MusicToggle = 12,
        SoundsToggle = 13,
        ResetSave = 14,
        ResetStory = 15,
        Credit = 16,
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
        void generate_qrcode(const char *message);

    private:
        void switch_screen(MenuScreen screen, Game *game);
        bool handle_start_menu(Game *game);
        bool handle_level_select_menu(Game *game);
        bool handle_gameover_menu(Game *game);
        bool handle_play_menu(Game *game);
        bool handle_config_menu(Game *game);
        void try_play_menu_music();

        bn::string<10> human_readable_score(bn::fixed score);
        bn::optional<QRCode> qrcode;

        bn::optional<bn::regular_bg_ptr> select_highlight;
        bn::optional<bn::regular_bg_ptr>
            title_menu_select;
        bn::optional<bn::sprite_ptr> collie_title;
        bn::optional<bn::sprite_animate_action<6>>
            collie_title_anim;
        bn::optional<bn::regular_bg_ptr> bg;
        bn::vector<bn::sprite_ptr, 2> config_sprites;
        bn::vector<bn::sprite_ptr, 3> victory_banner_sprites;
        bn::vector<bn::sprite_ptr, 5> dancing_animals;
        bn::vector<bn::sprite_animate_action<10>, 3> victory_banner_animations;
        bn::vector<bn::sprite_animate_action<10>, 5> dancing_animals_animation;

        bn::fixed selected_level = 0;
        MenuScreen selected_menu_item = MenuScreen::Start;
        MenuScreen current_screen = MenuScreen::Title;
        bn::optional<bn::sprite_text_generator> text_generator;
        bn::vector<bn::sprite_ptr, 32> text_sprites;
        bn::vector<bn::sprite_ptr, 32> text_sprites_level;
        bn::vector<bn::sprite_ptr, 120> text_sprites_qrcode;
        int current_selection_index = 0;
        bn::vector<bn::pair<MenuScreen, bn::fixed_point>, 6> menu_elements;
    };
}
