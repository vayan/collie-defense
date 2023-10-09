#include "menu.h"
#include "game.h"

using namespace cd;

Menu::Menu()
{
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
    current_screen = Title;
    title_menu_select.reset();
    text_sprites_level.clear();
    text_sprites.clear();
    text_sprites_qrcode.clear();
    collie_title.reset();
    collie_title_anim.reset();
    victory_banner_sprites.clear();
    victory_banner_animations.clear();
    dancing_animals.clear();
    dancing_animals_animation.clear();
    menu_elements.clear();
    config_sprites.clear();
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
        victory_banner_sprites.push_back(bn::sprite_items::victory_banner_left.create_sprite(-48, -30));
        victory_banner_sprites.push_back(bn::sprite_items::victory_banner_middle.create_sprite(16, -30));
        victory_banner_sprites.push_back(bn::sprite_items::victory_banner_right.create_sprite(80, -30));
        victory_banner_animations.push_back(bn::create_sprite_animate_action_forever(
            victory_banner_sprites.at(0),
            8,
            bn::sprite_items::victory_banner_left.tiles_item(),
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9));

        victory_banner_animations.push_back(bn::create_sprite_animate_action_forever(
            victory_banner_sprites.at(1),
            8,
            bn::sprite_items::victory_banner_middle.tiles_item(),
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9));

        victory_banner_animations.push_back(bn::create_sprite_animate_action_forever(
            victory_banner_sprites.at(2),
            8,
            bn::sprite_items::victory_banner_right.tiles_item(),
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9));

        dancing_animals.push_back(bn::sprite_items::dancing_sheep_0.create_sprite(-72, 30));
        dancing_animals.push_back(bn::sprite_items::dancing_sheep_1.create_sprite(-40, 30));
        dancing_animals.push_back(bn::sprite_items::dancing_collie.create_sprite(0, 30));
        dancing_animals.push_back(bn::sprite_items::dancing_sheep_2.create_sprite(40, 30));
        dancing_animals.push_back(bn::sprite_items::dancing_sheep_3.create_sprite(72, 30));

        dancing_animals_animation.push_back(bn::create_sprite_animate_action_forever(
            dancing_animals.at(0),
            4,
            bn::sprite_items::dancing_sheep_0.tiles_item(),
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9));
        dancing_animals_animation.push_back(bn::create_sprite_animate_action_forever(
            dancing_animals.at(1),
            4,
            bn::sprite_items::dancing_sheep_1.tiles_item(),
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9));
        dancing_animals_animation.push_back(bn::create_sprite_animate_action_forever(
            dancing_animals.at(2),
            4,
            bn::sprite_items::dancing_collie.tiles_item(),
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9));
        dancing_animals_animation.push_back(bn::create_sprite_animate_action_forever(
            dancing_animals.at(3),
            4,
            bn::sprite_items::dancing_sheep_2.tiles_item(),
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9));
        dancing_animals_animation.push_back(bn::create_sprite_animate_action_forever(
            dancing_animals.at(4),
            4,
            bn::sprite_items::dancing_sheep_3.tiles_item(),
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9));
        break;
    case MenuScreen::LevelSelect:
        log("start leveling selection");
        // need to clear any leftover level because level select is quite memory hungry
        // we need leftover level sometimes to display the win screen for example
        game->reset_current_level();
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
        try_play_menu_music();
        game->reset_current_level();
        bg = bn::regular_bg_items::menu_qrcode.create_bg(0, 0);
        text_generator.value()
            .generate(bn::fixed_point(-70, 0), "loading...", text_sprites);
        bn::core::update(); // trigger a refresh for the background to show first. the QRcode takes a while to load
        generate_qrcode(game->get_save()->encode_for_qrcode());
        text_sprites.clear();
        break;
    }
    case MenuScreen::Play:
        log("start play menu");
        try_play_menu_music();
        game->get_camera().set_position(0, 0);
        title_menu_select = bn::regular_bg_items::menu_select_item_highlight.create_bg(0, 0);
        bg = bn::regular_bg_items::menu_playmode.create_bg(0, 0);
        current_selection_index = 0;
        menu_elements.emplace_back(MenuScreen::StoryItem, bn::fixed_point(0, 0));
        menu_elements.emplace_back(MenuScreen::LevelSelect, bn::fixed_point(0, 24));
        menu_elements.emplace_back(MenuScreen::Start, bn::fixed_point(0, 49));
        break;
    case MenuScreen::Config:
        log("start config menu");
        try_play_menu_music();
        game->get_camera().set_position(0, 0);
        title_menu_select = bn::regular_bg_items::menu_select_item_highlight.create_bg(0, 0);
        bg = bn::regular_bg_items::menu_config.create_bg(0, 0);
        current_selection_index = 5;
        menu_elements.emplace_back(MenuScreen::MusicToggle, bn::fixed_point(0, -72));
        menu_elements.emplace_back(MenuScreen::SoundsToggle, bn::fixed_point(0, -48));
        menu_elements.emplace_back(MenuScreen::ResetSave, bn::fixed_point(0, -24));
        menu_elements.emplace_back(MenuScreen::ResetStory, bn::fixed_point(0, 0));
        menu_elements.emplace_back(MenuScreen::Credit, bn::fixed_point(0, 24));
        menu_elements.emplace_back(MenuScreen::Start, bn::fixed_point(0, 49));

        config_sprites.push_back(bn::sprite_items::checkbox_button.create_sprite(28, -63));
        config_sprites.push_back(bn::sprite_items::checkbox_button.create_sprite(28, -38));

        config_sprites.at(0).set_bg_priority(0);
        config_sprites.at(1).set_bg_priority(0);

        break;
    case MenuScreen::LevelWin:
        log("start level win screen");
        if (bn::music::playing())
        {
            bn::music::stop();
        }
        play_sfx(bn::sound_items::win, 1);
        bg = bn::regular_bg_items::level_win_bg.create_bg(0, 0);
        bg->set_priority(1);
        bg->set_blending_enabled(true);
        game->get_camera().set_position(0, 0);
        victory_banner_sprites.push_back(bn::sprite_items::victory_banner_left.create_sprite(-48, -30));
        victory_banner_sprites.push_back(bn::sprite_items::victory_banner_middle.create_sprite(16, -30));
        victory_banner_sprites.push_back(bn::sprite_items::victory_banner_right.create_sprite(80, -30));

        victory_banner_sprites.at(0).set_bg_priority(0);
        victory_banner_sprites.at(1).set_bg_priority(0);
        victory_banner_sprites.at(2).set_bg_priority(0);

        victory_banner_animations.push_back(bn::create_sprite_animate_action_forever(
            victory_banner_sprites.at(0),
            8,
            bn::sprite_items::victory_banner_left.tiles_item(),
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9));

        victory_banner_animations.push_back(bn::create_sprite_animate_action_forever(
            victory_banner_sprites.at(1),
            8,
            bn::sprite_items::victory_banner_middle.tiles_item(),
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9));

        victory_banner_animations.push_back(bn::create_sprite_animate_action_forever(
            victory_banner_sprites.at(2),
            8,
            bn::sprite_items::victory_banner_right.tiles_item(),
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9));
        break;
    case MenuScreen::Start:
        log("start screen");
        try_play_menu_music();
        game->get_camera().set_position(0, 0);
        title_menu_select = bn::regular_bg_items::menu_select_item_highlight.create_bg(0, 0);
        bg = bn::regular_bg_items::menu_title.create_bg(0, 0);
        current_selection_index = 0;
        menu_elements.emplace_back(MenuScreen::Play, bn::fixed_point(0, 0));
        menu_elements.emplace_back(MenuScreen::Share, bn::fixed_point(0, 24));
        menu_elements.emplace_back(MenuScreen::Config, bn::fixed_point(0, 49));
        break;
    case MenuScreen::Credit:
        log("credit screen");
        try_play_menu_music();
        game->get_camera().set_position(0, 0);
        bg = bn::regular_bg_items::menu_credit.create_bg(0, 0);
        current_selection_index = 0;
        menu_elements.emplace_back(MenuScreen::Config, bn::fixed_point(0, 0));
        break;
    default:
        log("start tilte screen");
        try_play_menu_music();
        game->get_camera().set_position(0, 0);
        collie_title = bn::sprite_items::collie_title.create_sprite(1, 14);
        collie_title_anim = bn::create_sprite_animate_action_forever(
            collie_title.value(),
            8,
            bn::sprite_items::collie_title.tiles_item(),
            0, 1, 2, 3, 4, 5);
        collie_title->set_bg_priority(1);
        bg = bn::regular_bg_items::menu_title_screen.create_bg(0, 0);
        current_selection_index = 0;
        break;
    }

    if (bg.has_value())
    {
        bg->set_priority(2);
    }
    if (title_menu_select.has_value())
    {
        title_menu_select->set_priority(1);
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
        qr_sprite.set_x(qr_sprite.x() - 113);
    }
    display_memory_left();
}

bn::fixed Menu::get_selected_level()
{
    return selected_level;
}

bool Menu::handle_start_menu(Game *game)
{
    if (bn::keypad::down_pressed())
    {
        current_selection_index += 1;
        play_sfx(bn::sound_items::select, 1);
    }

    if (bn::keypad::up_pressed())
    {
        current_selection_index -= 1;
        play_sfx(bn::sound_items::select, 1);
    }

    if (current_selection_index < 0)
    {
        current_selection_index = menu_elements.size() - 1;
    }

    if (current_selection_index >= menu_elements.size())
    {
        current_selection_index = 0;
    }

    title_menu_select->set_position(menu_elements.at(current_selection_index).second);

    if (bn::keypad::start_pressed() || bn::keypad::a_pressed())
    {
        switch (menu_elements.at(current_selection_index).first)
        {
        case MenuScreen::Play:
            switch_screen(MenuScreen::Play, game);
            break;
        case MenuScreen::Share:
            switch_screen(MenuScreen::Share, game);
            break;
        case MenuScreen::Config:
            switch_screen(MenuScreen::Config, game);
            break;
        default:
            break;
        }
    }

    return true;
}

bool Menu::handle_config_menu(Game *game)
{
    if (bn::keypad::down_pressed())
    {
        current_selection_index += 1;
        play_sfx(bn::sound_items::select, 1);
    }

    if (bn::keypad::up_pressed())
    {
        current_selection_index -= 1;
        play_sfx(bn::sound_items::select, 1);
    }

    if (current_selection_index < 0)
    {
        current_selection_index = menu_elements.size() - 1;
    }

    if (current_selection_index >= menu_elements.size())
    {
        current_selection_index = 0;
    }

    config_sprites.at(0).set_item(bn::sprite_items::checkbox_button, game->get_save()->get_data().music_off ? 1 : 0);
    config_sprites.at(1).set_item(bn::sprite_items::checkbox_button, game->get_save()->get_data().sounds_off ? 1 : 0);

    title_menu_select->set_position(menu_elements.at(current_selection_index).second);

    if (bn::keypad::start_pressed() || bn::keypad::a_pressed())
    {
        MenuScreen selectedMenu = menu_elements.at(current_selection_index).first;

        switch (selectedMenu)
        {
        case MenuScreen::Start:
            switch_screen(MenuScreen::Start, game);
            break;
        case MenuScreen::ResetSave:
            game->get_save()->reset();
            switch_screen(MenuScreen::Title, game);
            break;
        case MenuScreen::ResetStory:
            game->get_save()->save_story_progress(0, fallback_money, 100);
            switch_screen(MenuScreen::Start, game);
            break;
        case MenuScreen::SoundsToggle:
            game->get_save()->toggle_sounds();
            break;
        case MenuScreen::MusicToggle:
            game->get_save()->toggle_music();
            break;
        case MenuScreen::Credit:
            switch_screen(MenuScreen::Credit, game);
            break;
        default:
            break;
        }
    }

    return true;
}

bool Menu::handle_play_menu(Game *game)
{
    if (collie_title_anim.has_value())
    {
        collie_title_anim->update();
    }

    if (bn::keypad::down_pressed())
    {
        current_selection_index += 1;
        play_sfx(bn::sound_items::select, 1);
    }

    if (bn::keypad::up_pressed())
    {
        current_selection_index -= 1;
        play_sfx(bn::sound_items::select, 1);
    }

    if (current_selection_index < 0)
    {
        current_selection_index = menu_elements.size() - 1;
    }

    if (current_selection_index >= menu_elements.size())
    {
        current_selection_index = 0;
    }

    title_menu_select->set_position(menu_elements.at(current_selection_index).second);

    if (bn::keypad::start_pressed() || bn::keypad::a_pressed())
    {
        MenuScreen selectedMenu = menu_elements.at(current_selection_index).first;

        if (selectedMenu == MenuScreen::StoryItem)
        {
            game->set_game_mode(GameMode::Story);
            save_data data = game->get_save()->get_data();
            selected_level = data.latest_story_level.integer();
            game->get_player()->set_money(data.story_money);
            game->get_player()->set_life(data.story_life);
            return false;
        }

        switch (selectedMenu)
        {
        case MenuScreen::LevelSelect:
            switch_screen(MenuScreen::LevelSelect, game);
            break;
        case MenuScreen::Start:
            switch_screen(MenuScreen::Start, game);
            break;
        default:
            break;
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
        return "";
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
        play_sfx(bn::sound_items::select, 1);
        selected_level = selected_level - 2;
    }

    if (bn::keypad::down_pressed())
    {
        play_sfx(bn::sound_items::select, 1);
        selected_level = selected_level + 2;
    }

    if (bn::keypad::right_pressed())
    {
        play_sfx(bn::sound_items::select, 1);
        selected_level = selected_level + 1;
    }

    if (bn::keypad::left_pressed())
    {
        play_sfx(bn::sound_items::select, 1);
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

    if (selected_level >= cd::number_of_levels - 2)
    {
        game->get_camera().set_y(204);
        select_highlight->set_y(-1);
    }
    else
    {
        game->get_camera().set_y(-186 + (81 * (selected_level.safe_division(2).floor_integer())));
        select_highlight->set_y(-17);
    }

    select_highlight->set_x(-55 + (115 * (selected_level.integer() % 2)));

    bn::string<50> text = bn::format<50>("STAGE {}", selected_level + 1);

    bn::fixed score = game->get_save()->get_data().score_per_level[selected_level.integer()];

    bn::string<10> score_value = human_readable_score(score);

    bn::string<50> text_score;

    if (score_value.length() == 0)
    {
        text_score = bn::format<50>("NEW GAME");
    }
    else
    {
        text_score = bn::format<50>("RANK {}", score_value);
    }

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
        switch_screen(MenuScreen::Play, game);
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
    if (current_screen == MenuScreen::Restart)
    {
        log("skipping the menu");
        return false;
    }

    if (!bg.has_value())
    {
        switch_screen(current_screen, game);
    }

    bg->set_camera(game->get_camera());

    if (current_screen == MenuScreen::Title)
    {
        if (collie_title_anim.has_value())
        {
            collie_title_anim->update();
        }

        if (bn::keypad::start_pressed() || bn::keypad::a_pressed())
        {
            bn::core::update();
            switch_screen(MenuScreen::Start, game);
        }
    }

    if (current_screen == MenuScreen::Start)
    {
        return handle_start_menu(game);
    }

    if (current_screen == MenuScreen::Credit)
    {
        if (bn::keypad::start_pressed() || bn::keypad::a_pressed())
        {
            switch_screen(MenuScreen::Config, game);
        }
    }

    if (current_screen == MenuScreen::LevelSelect)
    {
        return handle_level_select_menu(game);
    }

    if (current_screen == MenuScreen::GameOver)
    {
        return handle_gameover_menu(game);
    }

    if (current_screen == MenuScreen::Play)
    {
        return handle_play_menu(game);
    }

    if (current_screen == MenuScreen::Config)
    {
        return handle_config_menu(game);
    }

    if (current_screen == MenuScreen::Share)
    {

        if (bn::keypad::any_pressed())
        {
            switch_screen(MenuScreen::Start, game);
        }
    }

    if (current_screen == MenuScreen::Win)
    {
        victory_banner_animations.at(0).update();
        victory_banner_animations.at(1).update();
        victory_banner_animations.at(2).update();

        dancing_animals_animation.at(0).update();
        dancing_animals_animation.at(1).update();
        dancing_animals_animation.at(2).update();
        dancing_animals_animation.at(3).update();
        dancing_animals_animation.at(4).update();
    }

    if (current_screen == MenuScreen::LevelWin)
    {
        victory_banner_animations.at(0).update();
        victory_banner_animations.at(1).update();
        victory_banner_animations.at(2).update();

        if (bn::keypad::start_pressed() || bn::keypad::a_pressed())
        {
            if (game->get_game_mode() == GameMode::Single)
            {
                game->get_current_level()->reset();
                switch_screen(MenuScreen::LevelSelect, game);
            }
            else
            {
                selected_level = game->get_current_level_index();
                return false;
            }
        }
    }

    return true;
}

void Menu::set_current_screen(MenuScreen screen)
{
    current_screen = screen;
}

void Menu::try_play_menu_music()
{
    if (!bn::music::playing() || bn::music::paused())
    {
        play_music(bn::music_items::takeawalk, 0.2);
    }
}
