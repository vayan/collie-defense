#include "game.h"

using namespace cd;

Game::Game(Menu *_menu) : menu(_menu)
{
    camera = bn::camera_ptr::create(0, 0);
    save = new Save();
    player = new Player(camera.value());

    log("game manager created!");

    log("BN_CFG_SPRITES_MAX_SORT_LAYERS value is", BN_CFG_SPRITES_MAX_SORT_LAYERS);
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

    if (current_level.has_value())
    {
        delete current_level.value();
        current_level.reset();
    }

    player.value()->on_reset_store();

    cd::log("loading level number", level_index);

    current_level = levels[level_index]->copy();

    if (get_game_mode() == GameMode::Single)
    {
        player.value()->reset();
        player.value()->set_money(current_level.value()->get_start_money());
    }

    player.value()->activate();

    current_level.value()->init(camera.value());
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
            player.value()->on_reset_store();
            current_level.value()->reset();
            return MenuScreen::Start;
        }

        if (!is_paused)
        {
            current_level.value()->tick(this);
            player.value()->on_tick(this);
        }

        bn::core::update();

        if (player.value()->is_dead())
        {
            cd::log("game over");
            player.value()->on_reset_store();
            current_level.value()->reset();
            save->save_story_progress(0, fallback_money, 100);

            return MenuScreen::GameOver;
        }
        else if (current_level_index >= cd::number_of_levels)
        {
            cd::log("end of game");
            current_level.value()->reset();
            return MenuScreen::Win;
        }
        else if (current_level.value()->is_won() && get_game_mode() == GameMode::Story)
        {
            cd::log("story level finished");
            player.value()->disable();
            current_level_index += 1;
            save->save_story_progress(current_level_index, player.value()->get_money(), player.value()->get_life());
            return MenuScreen::LevelWin;
        }
        else if (current_level.value()->is_won() && get_game_mode() == GameMode::Single)
        {
            cd::log("score level finished");
            // add a timer or keep life left for scoring?? mhhh
            save->save_level_score(
                current_level_index,
                player.value()->get_life().safe_division(10).round_integer());

            player.value()->disable();
            player.value()->reset();

            return MenuScreen::LevelWin;
        }
    }
    player.value()->on_reset_store();

    return MenuScreen::Start;
}

int Game::start_main_loop()
{
    log("starting main loop");

    while (true)
    {
        player.value()->disable();
        start_menu_screen_loop();

        current_level_index = menu->get_selected_level().integer();

        bn::core::update();
        MenuScreen transition_to = start_level_loop();
        menu->set_current_screen(transition_to);
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
        player.value()->activate();
        stop_pause();

        if (bn::music::paused() && bn::music::playing())
        {
            bn::music::resume();
        }
    }
    else
    {
        if (!bn::music::paused() && bn::music::playing())
        {
            bn::music::pause();
        }
        player.value()->disable();
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
    return player.value();
}

Level *Game::get_current_level()
{
    return current_level.value();
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
