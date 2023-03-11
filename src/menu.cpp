#include "menu.h"
#include "game.h"

using namespace cd;

Menu::Menu()
{
    bn::music_items::takeawalk.play(0.2);
    bn::blending::set_transparency_alpha(0.6);
    bn::sprite_text_generator _sprite_text_generator(cd::fixed_font_8x8);
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
    text_sprites.clear();
    text_sprites_qrcode.clear();
}

void Menu::switch_screen(MenuScreen screen, Game *game)
{
    clear();
    switch (screen)
    {
    case MenuScreen::GameOver:
        log("start game over screen");
        game->get_camera().set_position(0, 0);
        bg = bn::regular_bg_items::gameover.create_bg(0, 0);
        break;

    case MenuScreen::Win:
        log("start win screen");
        game->get_camera().set_position(0, 0);
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
    case MenuScreen::Share:
    {
        log("start sharing menu");
        bg = bn::regular_bg_items::bg_qrcode.create_bg(0, 0);

        generate_qrcode("HTTP://TD.GY/CRE66I9R"); // TODO get real data

        break;
    }
    default:
        log("start title screen");
        game->get_camera().set_position(0, 0);
        collie_select = bn::sprite_items::dog.create_sprite(-85, 71);
        bg = bn::regular_bg_items::launch_background.create_bg(0, 0);
        break;
    }

    current_screen = screen;
}

/*
   for the message, limit yourself to these chars:
   uppercase letters (A-Z)
   numbers (0-9)
   space ( )
   dollar sign ($)
   percent sign (%)
   asterisk (*)
   plus (+)
   minus (-)
   decimal point (.)
   slash (/)
   colon (:)
*/
void Menu::generate_qrcode(const char *message)
{
    if (qrcode.has_value())
    {
        qrcode.reset();
    }

    qrcode = QRCode();

    uint8_t qrcodeBytes[256];

    qrcode_initText(&qrcode.value(), qrcodeBytes, 1, ECC_LOW, message);

    bn::string<100> data = "";

    bn::sprite_text_generator text_gen(cd::fake_qrcode_font);

    text_gen.set_one_sprite_per_character(false);
    text_gen.set_left_alignment();

    for (int y = 0; y < qrcode->size; y++)
    {
        for (int x = 0; x < qrcode->size; x++)
        {
            if (qrcode_getModule(&qrcode.value(), x, y))
            {
                data = data + "y";
            }
            else
            {
                data = data + " ";
            }
        }

        // 7 distorded the qrcode (should be 8)
        // but it's still scannable and fit on the screen
        text_gen.generate(0, y * 7, data, text_sprites_qrcode);

        data = "";
    }

    for (bn::sprite_ptr qr_sprite : text_sprites_qrcode)
    {
        qr_sprite.set_visible(true);
        qr_sprite.set_bg_priority(0);
        qr_sprite.set_z_order(-1);
        qr_sprite.put_above();
        qr_sprite.set_y(qr_sprite.y() - 70);
        qr_sprite.set_x(qr_sprite.x() - 87);
    }
    display_memory_left();
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
        bn::sound_items::select.play();
    }

    if (bn::keypad::left_pressed())
    {
        selected_menu_item = MenuScreen::Start;
        collie_select->set_position(-85, 71);
        bn::sound_items::select.play();
    }

    if (bn::keypad::select_pressed())
    {
        switch_screen(MenuScreen::Share, game);
    }

    if (bn::keypad::start_pressed() || bn::keypad::a_pressed())
    {
        collie_select.reset();

        if (selected_menu_item == MenuScreen::Start)
        {
            game->set_game_mode(GameMode::Story);
            save_data data = game->get_save()->get_data();
            selected_level = data.latest_story_level.integer();
            game->get_player()->set_money(data.story_money);
            game->get_player()->set_life(data.story_life);

            return false;
        }
        if (selected_menu_item == MenuScreen::LevelSelect)
        {
            switch_screen(MenuScreen::LevelSelect, game);
        }
    }

    return true;
}

bn::string<10> Menu::human_readable_score(bn::fixed score)
{
    switch (score.integer())
    {
    case 1:
        return "F";
    case 2:
        return "D";
    case 3:
        return "C";
    case 4:
        return "B-";
    case 5:
        return "B";
    case 6:
        return "B+";
    case 7:
        return "A-";
    case 8:
        return "A";
    case 9:
        return "A+";
    case 10:
        return "A++";
    default:
        return "-";
    }
}

bool Menu::handle_gameover_menu(Game *game)
{
    if (bn::keypad::start_pressed() || bn::keypad::a_pressed())
    {
        switch_screen(MenuScreen::Start, game);
    }
    return true;
}

bool Menu::handle_level_select_menu(Game *game)
{
    if (bn::keypad::up_pressed())
    {
        bn::sound_items::select.play();
        selected_level = selected_level - 2;
    }

    if (bn::keypad::down_pressed())
    {
        bn::sound_items::select.play();
        selected_level = selected_level + 2;
    }

    if (bn::keypad::right_pressed())
    {
        bn::sound_items::select.play();
        selected_level = selected_level + 1;
    }

    if (bn::keypad::left_pressed())
    {
        bn::sound_items::select.play();
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

    game->get_camera().set_y(-186 + (81 * (selected_level.safe_division(2).floor_integer())));
    select_highlight->set_x(-55 + (115 * (selected_level.integer() % 2)));

    bn::string<50> text = bn::format<50>("STAGE {}", selected_level + 1);

    bn::fixed score = game->get_save()->get_data().score_per_level[selected_level.integer()];

    bn::string<50>
        text_score = bn::format<50>("RANK {}", human_readable_score(score));

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

    if (current_screen == MenuScreen::GameOver)
    {
        return handle_gameover_menu(game);
    }

    if (current_screen == MenuScreen::Share)
    {

        if (bn::keypad::b_pressed())
        {
            switch_screen(MenuScreen::Start, game);
        }
    }

    return true;
}

void Menu::set_current_screen(MenuScreen screen)
{
    current_screen = screen;
}
