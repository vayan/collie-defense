#include "menu.h"
#include "game.h"

using namespace cd;

Menu::Menu()
{
    bn::music_items::takeawalk.play();
    bn::blending::set_transparency_alpha(0.6);
    bn::sprite_text_generator _sprite_text_generator(as::fixed_font_8x8);
    text_generator = _sprite_text_generator;
}

Menu::~Menu()
{
}

void Menu::clear()
{
    bg.reset();
    select_highlight.reset();
    current_screen = Start;
    collie_select.reset();
    text_sprites_level.clear();
}

void Menu::switch_screen(MenuScreen screen, Game *game)
{
    clear();
    switch (screen)
    {
    case MenuScreen::GameOver:
        log("start game over screen");
        bg = bn::regular_bg_items::gameover.create_bg(0, 0);
        break;

    case MenuScreen::Win:
        log("start win screen");
        bg = bn::regular_bg_items::gamewin.create_bg(0, 0);
        break;
    case MenuScreen::LevelSelect:
        log("start leveling selection");
        bg = bn::regular_bg_items::all_levels.create_bg(0, 0);
        select_highlight = bn::regular_bg_items::level_select.create_bg(0, -17);
        select_highlight->set_priority(0);
        select_highlight->set_blending_enabled(true);

        selected_level = 0;
        game->get_camera()
            .set_position(0, -174);
        break;

    default:
        log("start title screen");
        collie_select = bn::sprite_items::dog.create_sprite(-85, 71);
        bg = bn::regular_bg_items::launch_background.create_bg(0, 0);
        break;
    }

    current_screen = screen;
}

bn::fixed Menu::get_selected_level()
{
    return selected_level;
}

bool Menu::handle_start_menu(Game *game)
{
    if (bn::keypad::right_pressed())
    {
        selected_menu_item = MenuScreen::LevelSelect;
        collie_select->set_position(2, 71);
    }

    if (bn::keypad::left_pressed())
    {
        selected_menu_item = MenuScreen::Start;
        collie_select->set_position(-85, 71);
    }

    if (bn::keypad::start_pressed() || bn::keypad::a_pressed())
    {
        collie_select.reset();

        if (selected_menu_item == MenuScreen::Start)
        {
            game->set_game_mode(GameMode::Story);
            selected_level = game->get_save()->get_data().latest_level.integer();
            return false;
        }
        if (selected_menu_item == MenuScreen::LevelSelect)
        {
            switch_screen(MenuScreen::LevelSelect, game);
        }
    }

    return true;
}

bool Menu::handle_level_select_menu(Game *game)
{
    if (bn::keypad::up_pressed())
    {
        selected_level = selected_level - 2;
    }

    if (bn::keypad::down_pressed())
    {
        selected_level = selected_level + 2;
    }

    if (bn::keypad::right_pressed())
    {
        selected_level = selected_level + 1;
    }

    if (bn::keypad::left_pressed())
    {
        selected_level = selected_level - 1;
    }

    if (selected_level < 0)
    {
        selected_level = 0;
    }

    if (selected_level > cd::number_of_levels - 1)
    {
        selected_level = cd::number_of_levels - 1;
    }

    game->get_camera().set_y(-186 + (80 * (selected_level.safe_division(2).floor_integer())));
    select_highlight->set_x(-55 + (115 * (selected_level.integer() % 2)));

    bn::string<50> text = bn::format<50>("STAGE {}", selected_level + 1);
    bn::string<50> text_score = bn::format<50>("SCORE {}", "A++"); // TODO dynamic scores

    text_sprites_level.clear();
    text_generator.value()
        .generate(bn::fixed_point(select_highlight->x() - 30, select_highlight->y() - 44), text, text_sprites_level);
    text_generator.value()
        .generate(bn::fixed_point(select_highlight->x() - 38, select_highlight->y() + 47), text_score, text_sprites_level);

    for (bn::sprite_ptr text_sprite : text_sprites_level)
    {
        text_sprite.set_bg_priority(0);
    }

    if (bn::keypad::b_pressed())
    {
        clear();
        return true;
    }

    if (bn::keypad::start_pressed() || bn::keypad::a_pressed())
    {
        game->set_game_mode(GameMode::Single);
        return false;
    }

    return true;
}

bool Menu::on_tick(Game *game)
{
    if (!bg.has_value())
    {
        switch_screen(current_screen, game);
    }

    bg->set_camera(game->get_camera());

    if (current_screen == MenuScreen::Start)
    {
        return handle_start_menu(game);
    }

    if (current_screen == MenuScreen::LevelSelect)
    {
        return handle_level_select_menu(game);
    }

    return true;
}

void Menu::set_current_screen(MenuScreen screen)
{
    current_screen = screen;
}
