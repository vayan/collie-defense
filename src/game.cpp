#include "game.h"

using namespace cd;

Game::Game(Menu _menu) : menu(_menu)
{
    camera = bn::camera_ptr::create(0, 0);
    current_level = cd::levels[0];
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

    current_level = cd::levels[level_index];
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
        else if (current_level->is_won())
        {
            current_level_index += 1;

            start_level(current_level_index);
        }
    }
    player->on_reset_store();

    return MenuScreen::Start;
}

int Game::start_main_loop()
{
    log("starting main loop");

    MenuScreen menu_state = MenuScreen::Start;

    while (true)
    {
        start_menu_screen_loop();

        player = cd::Player(camera.value());
        current_level_index = menu.get_selected_level().integer();
        bn::core::update();
        menu_state = start_level_loop();
        // TODO fix death
        player.reset();
        bn::core::update();
    }
}

void Game::start_menu_screen_loop()
{
    while (menu.on_tick(this))
    {
        bn::core::update();
    }
    menu.clear();
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
