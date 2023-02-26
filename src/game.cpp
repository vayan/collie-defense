#include "game.h"

using namespace cd;

Game::Game(Menu *_menu) : menu(_menu)
{
    camera = bn::camera_ptr::create(0, 0);
    current_level = levels[0];
    save = new Save();

    log("game manager created!");
}

Game::~Game()
{
}

void Game::start_level(int level_index)
{
    if (level_index >= cd::number_of_levels)
    {
        return;
    }

    player->on_reset_store();

    current_level->reset();

    cd::log("loading level number", level_index);

    current_level = levels[level_index];

    if (get_game_mode() == GameMode::Single)
    {
        player->reset();
        player->set_money(current_level->get_start_money());
    }

    current_level->init(camera.value());
    display_memory_left();
}

MenuScreen Game::start_level_loop()
{
    start_level(current_level_index);
    while (true)
    {
        if (bn::keypad::start_pressed())
        {
            log("paused the game");
            toggle_pause();
        }

        if (bn::keypad::select_pressed() && is_paused)
        {
            log("restart the game");
            stop_pause();
            player->on_reset_store();
            current_level->reset();
            return MenuScreen::Start;
        }

        if (!is_paused)
        {
            current_level->tick(this);
            player->on_tick(this);
        }

        bn::core::update();

        if (player->is_dead())
        {
            cd::log("game over");
            player->on_reset_store();
            current_level->reset();

            return MenuScreen::GameOver;
        }
        else if (current_level_index >= cd::number_of_levels)
        {
            cd::log("end of game");
            current_level->reset();
            return MenuScreen::Win;
        }
        else if (current_level->is_won() && get_game_mode() == GameMode::Story)
        {
            current_level_index += 1;
            save->set_latest_level(current_level_index);

            start_level(current_level_index);
        }
        else if (current_level->is_won() && get_game_mode() == GameMode::Single)
        {
            cd::log("level finished");
            bn::sound_items::win.play();
            current_level->reset();

            // add a timer or keep life left for scoring?? mhhh
            save->set_level_score(
                current_level_index,
                player->get_life().safe_division(10).round_integer());

            player->on_reset_store();
            player->reset();

            return MenuScreen::LevelSelect;
        }
    }
    player->on_reset_store();

    return MenuScreen::Start;
}

int Game::start_main_loop()
{
    log("starting main loop");

    while (true)
    {
        start_menu_screen_loop();

        player = cd::Player(camera.value());
        current_level_index = menu->get_selected_level().integer();

        bn::core::update();
        MenuScreen transition_to = start_level_loop();
        menu->set_current_screen(transition_to);
        player.reset();
        bn::core::update();
    }
}

void Game::start_menu_screen_loop()
{
    while (menu->on_tick(this))
    {
        bn::core::update();
    }
    menu->clear();
}

void Game::stop_pause()
{
    pause_bg.reset();
    is_paused = false;
}

void Game::toggle_pause()
{
    if (is_paused)
    {
        stop_pause();
    }
    else
    {
        player->on_reset_store();
        is_paused = true;
        pause_bg = bn::regular_bg_items::pause.create_bg(0, 0);
        pause_bg->set_camera(camera);
        pause_bg->set_visible(true);
        pause_bg->set_priority(1);
        pause_bg->set_blending_enabled(true);
        bn::core::update();
    }
}

Player *Game::get_player()
{
    return &player.value();
}

Level *Game::get_current_level()
{
    return current_level;
}

int Game::get_current_level_index()
{
    return current_level_index;
}

bn::camera_ptr Game::get_camera()
{
    return camera.value();
}

GameMode Game::get_game_mode()
{
    return game_mode;
}

void Game::set_game_mode(GameMode mode)
{
    game_mode = mode;
}

Save *Game::get_save()
{
    return save;
}
