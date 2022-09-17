#include "game.h"

using namespace cd;

Game::Game()
{
    camera = bn::camera_ptr::create(0, 0);
    player = new cd::Player(camera.value());
    current_level = cd::levels[0];
    ui = new cd::UI();
    log("game manager created!");
}

Game::~Game()
{
}

void Game::start_level(int level_index)
{
    if (level_index >= cd::number_of_levels)
    {
        cd::log("end of game");
        current_level_index = -1;
        // TODO show end screen - restart for now
        return;
    }

    current_level->reset();

    cd::log("loading level number", level_index);

    current_level = cd::levels[level_index];
    current_level->init(camera.value());
    display_memory_left();
}

void Game::start_level_loop()
{
    start_level(current_level_index);
    while (current_level_index > -1)
    {
        if (ui->is_paused())
        {
        }
        else
        {
            current_level->tick(this);
            player->on_tick(this);
        }

        ui->on_tick(this);

        bn::core::update();

        if (player->is_dead())
        {
            cd::log("game over");

            current_level_index = -1;

            // TODO implement gameover screen - restart for now
            return;
        }
        else if (current_level->is_won())
        {
            current_level_index += 1;

            start_level(current_level_index);
        }
    }
}

int Game::start_main_loop()
{
    log("starting main loop");

    while (true)
    {
        current_level_index = 0;
        start_level_loop();

        bn::core::update();
    }
}

UI *Game::get_ui()
{
    return ui;
}
Player *Game::get_player()
{
    return player;
}
Level *Game::get_current_level()
{
    return current_level;
}
bn::camera_ptr Game::get_camera()
{
    return camera.value();
}
